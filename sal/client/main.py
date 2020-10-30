"""
Simple Access Layer (SAL) Python Client
"""

import os
import sys
import warnings
import getpass
import configparser
import requests
from urllib.parse import urlparse

from sal.core.serialise import serialise, deserialise
from sal.core.path import decompose
from sal.core.object import Branch, DataObject
from sal.core import exception
from sal.core.version import VERSION
from sal.dataclass import *

__all__ = ['SALClient']

# Supported API version.
_API_VERSION = 2

# Authentication URLS.
_AUTH_URL = '{host}/auth'

# Operation URLS.
_LIST_URL = '{host}/data/{path}?revision={revision}'
_GET_URL = '{host}/data/{path}?object={object}&revision={revision}'
_PUT_URL = '{host}/data/{path}'
_DELETE_URL = '{host}/data/{path}'
_COPY_URL = '{host}/data/{path}?source={source_path}&source_revision={source_revision}'

# Content type recognised by SAL.
_MIME_JSON = 'application/json'

# Error to exception mapping table.
_EXCEPTION_MAP = {
    'InvalidRequest': exception.InvalidRequest,
    'InvalidPath': exception.InvalidPath,
    'NodeNotFound': exception.NodeNotFound,
    'PermissionDenied': exception.PermissionDenied,
    'UnsupportedOperation': exception.UnsupportedOperation,
    'InternalError': exception.InternalError,
    'AuthenticationFailed': exception.AuthenticationFailed
}

# The path to the default authentication credentials file.
_AUTH_DEFAULT_CREDENTIALS_PATH = '.sal/credentials'

# The path to the cached authentication tokens.
_AUTH_TOKEN_CACHE = '.sal/tokens'


class SALClient:
    """
    The Simple Access Layer (SAL) python client.

    To connect to a SAL server, the server URL must be supplied via the
    host attribute. The host string must be a valid url, including the
    scheme::

      https://sal.server.local

    A port number may be included::

      https://sal.server.local:5000

    The client will attempt a connection to the specified server to confirm
    the url points to a valid server. If a SAL server is not found, or an
    incompatible version of the server is identified (for example if the
    server is using a newer version of the API), a ConnectionError
    exception will be raised.

    If the server requires authentication this may be discovered by inspecting
    the value of the auth_required attribute. If the server requires
    authentication this will be set to True.

    By default, authentication credentials will be request as needed by the
    client. Using an access method that requires authentication will result
    in the client requesting a username and password. The authentication
    token will be stored and used across sessions until it expires.

    If more explicit control over authentication is required, the user can
    explicitly call the authenticate method. Password prompting can be
    disabled by setting the attribute prompt_for_password to False.

    If authentication is successful, an authentication token will be returned
    by the server. The token grants access to the server for a limited time
    window, determined by the server administrator. Once the token expires the
    user will need to re-authenticate. The attribute auth_token will contain
    the token for the current session. The token is tied to the IP address of
    the machine it was requested with.

    While not recommended, a credentials file may be created containing the
    username and password to pass to a server. The password in the file is
    clear-text and therefore must be protected by the user. The format of the
    credentials file is as follows:

        [HOSTNAME]
        user=USERNAME
        password=PASSWORD

    For example:

        [https://sal.server1.com]
        user=admin
        password=secret

        [https://sal.server2.com]
        user=datauser
        password=secret2

    By default the client will look for a file called $HOME/.sal/credentials.
    If this file is found to contain valid data for a server, the password
    prompt will not be shown. Alternate credential file paths can be passed
    via arguments to the authenticate method.

    For developers, when instancing the client it is possible to disable SSL
    certificate checks by setting the verify_https_cert argument to False .

    :param host: The SAL server URL.
    :param verify_https_cert: Perform SSL certificate validation (default=True).
    :raises ConnectionError: If the client fails to connect to a SAL server.
    """

    #: The client version string.
    version = VERSION

    def __init__(self, host, verify_https_cert=True):

        # authentication attributes
        self.auth_required = False
        self.auth_token = None
        self.credentials_file = None
        self.prompt_for_password = True
        self.verify_https_cert = verify_https_cert

        # set and inspect host
        self.host = host

    @property
    def host(self):
        """
        The SAL server host URL.

        This attribute can be used to obtain the current host URL or set a new
        host url.
        """
        return self._host

    @host.setter
    def host(self, url):
        if not self._is_valid_url(url):
            raise ValueError("The supplied host string does not contain a valid URL.")
        self._check_host(url)
        self._host = url

        # reset authentication state
        self.auth_token = None
        self.credentials_file = None

    def _check_host(self, url):
        """
        Inspects the specified server to identify if it is a SAL server.

        If a SAL server is identified, relevant configuration values are stored
        e.g. is authentication enabled.

        :param url: Host URL
        """

        # request server information from server root
        response = self._make_get_request(url)

        # parse response
        try:
            self._validate_response(response, valid_code=200)
            content = response.json()
        except requests.ConnectionError:
            raise ConnectionError('Could not identify a SAL server on the specified host: \'{}\''.format(url))

        if content['service']['name'] != 'Simple Access Layer (SAL) Server':
            raise ConnectionError('Could not identify a SAL server on the specified host: \'{}\''.format(url))

        if content['api']['version'] > _API_VERSION:
            raise ConnectionError('The server is using a newer API than the client recognises, please update your client.')

        self.auth_required = content['api']['requires_auth']

    def authenticate(self, user=None, password=None, credentials=None):
        """
        Authenticates the client with the server.

        Authentication may be performed by passing a username/password to the
        method or via a credentials file.

        If the method is called without the necessary arguments, and password
        prompting is enabled, it will prompt the user for a username and/or
        password.

        If prompting is disabled and no arguments are supplied, the client will
        attempt to identify credentials from the default credentials file:
        $HOME/.sal/credentials.

        :param user: Username string.
        :param password: Password string.
        :param credentials: Credentials file path.
        :raises AuthenticationFailed: If authentication with the server fails.
        """

        # reset authentication state
        self.auth_token = None
        self.credentials_file = None

        # escape if authentication is not required
        if not self.auth_required:
            return

        # user supplied but not password
        if user and not password and self.prompt_for_password:
            print('The server [{}] requires authentication.'.format(self.host))
            password = getpass.getpass('Password: ')

        # user and password supplied
        if user and password:
            self._obtain_auth_token(user, password)
            return

        # explore credentials file (tries default location if credentials are not supplied)
        user, password = self._read_auth_credentials(credentials)
        if user and password:
            self._obtain_auth_token(user, password)
            return

        # prompt for user and password if enabled
        if self.prompt_for_password:
            print('The server [{}] requires authentication.'.format(self.host))
            user = input('User: ')
            password = getpass.getpass('Password: ')
            self._obtain_auth_token(user, password)
            return

        # all attempts at obtaining credentials have failed!
        raise exception.AuthenticationFailed("Could not locate valid credentials to send to the server.")

    def _read_auth_credentials(self, path=None):
        """
        Reads authentication credentials from a file.

        A configuration section corresponding to the host URL is identified
        and the credential entries in that section are read. The user name
        and password are returned in a tuple: (user, password). If the
        credentials file is invalid or there is no entry for the host URL then
        both fields of the tuple will be set to None: (None, None).

        If a specific credentials file has not been specified, a default path
        is searched.

        :param path: The path to the credentials file.
        :return: A tuple containing the username and password.
        """

        # use the default path if path is not supplied
        path = path or os.path.join(os.path.expanduser('~'), _AUTH_DEFAULT_CREDENTIALS_PATH)

        # read configuration, not if the file is missing read silently ignores the file and remains empty
        c = configparser.ConfigParser()
        c.read(path)
        try:
            user = c[self.host]['user']
            password = c[self.host]['password']
        except KeyError:
            self.credentials_file = None
            return None, None

        # store location of credentials for user information
        self.credentials_file = path
        return user, password

    def _read_auth_token(self):
        """
        Reads the authentication token from the session cache.

        :return: The authentication token for the current host.
        """

        path = os.path.join(os.path.expanduser('~'), _AUTH_TOKEN_CACHE)

        # read token if available
        c = configparser.ConfigParser()
        c.read(path)
        try:
            self.auth_token = c[self.host]['token']
        except KeyError:
            return False
        return True

    def _write_auth_token(self):
        """
        Writes the current authentication token to the session cache.
        """

        path = os.path.join(os.path.expanduser('~'), _AUTH_TOKEN_CACHE)

        # read cached tokens
        c = configparser.ConfigParser()
        c.read(path)

        # update/create token
        c[self.host] = {'token': self.auth_token}

        # write tokens, creating file/folder if not present
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w") as f:
            c.write(f)

    def _clear_auth_token(self):
        """
        Clears the authentication token for the current host.
        """

        path = os.path.join(os.path.expanduser('~'), _AUTH_TOKEN_CACHE)

        # read cached tokens
        c = configparser.ConfigParser()
        c.read(path)

        # remove token
        c.remove_section(self.host)

        # write tokens, creating file/folder if not present
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w") as f:
            c.write(f)

    def _obtain_auth_token(self, user, password):
        """
        Requests an authentication token from the server.

        :param user: Username string.
        :param password: Password string.
        """

        response = self._make_get_request(_AUTH_URL.format(host=self.host), auth=(user, password))

        # extract authentication token
        content = response.json()
        try:
            self.auth_token = content['authorisation']['token']
        except KeyError:
            raise exception.InvalidResponse('The server did not return an authentication token.')

    def list(self, path):
        """
        Returns node meta data for the specific path.

        The object returned depends on the type of node pointed to by the path.
        If the path identifies a branch node, a BranchReport object is returned.
        For leaf nodes, a LeafReport object is returned.

        :param path: A valid node path.
        :return: A :class:`~sal.core.object.BranchReport` or :class:`~sal.core.object.LeafReport` object.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        # check path is valid and dismantle
        segments, revision, is_absolute = decompose(path)
        if not is_absolute:
            raise ValueError("The supplied path must be an absolute path.")

        # make request
        url = _LIST_URL.format(host=self.host, path='/'.join(segments), revision=revision)
        response = self._make_get_request(url)

        # de-serialise content
        content = response.json()
        return deserialise(content)

    def get(self, path, summary=False):
        """
        Returns node data for the specific path.

        The object returned depends on the type of node pointed to by the path.
        If the path identifies a branch node, a Branch object is returned. Leaf
        nodes return the data object stored on that node.

        For leaf nodes, users can request the full data object or a reduced
        summary object (containing only the object meta data). If the summary
        argument is set to false (the default) the full data object is
        returned, if true a data summary is returned. The summary argument has
        no effect for branch nodes.

        :param path: A valid node path.
        :param summary: Return a summary object (default: False).
        :return: A :class:`~sal.core.object.Branch`, :class:`~sal.core.object.DataObject`
                 or :class:`~sal.core.object.DataSummary` object.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        # check path is valid and dismantle
        segments, revision, is_absolute = decompose(path)
        if not is_absolute:
            raise ValueError("The supplied path must be an absolute path.")

        # make request
        obj_type = 'summary' if summary else 'full'
        url = _GET_URL.format(host=self.host, path='/'.join(segments), object=obj_type, revision=revision)
        response = self._make_get_request(url)

        # de-serialise content
        content = response.json()
        return deserialise(content)

    def put(self, path, content):
        """
        Creates/updates node data at the specific path.

        Put may be used to create or update branch and leaf nodes. Overwriting
        an existing node with new content will update the existing node. In the
        case of a branch node this will update the branch metadata without
        affecting the descendant nodes. Writing a new data object to a leaf
        node will replace the existing data object.

        To create a branch node, a Branch object must be supplied as the put
        content.

        To create a leaf node, a suitable DataObject subclass must be provided.
        A leaf node will be created to encapsulate the DataObject.

        Put will not create any missing intermediate nodes to the target path,
        these must be created manually before the put operation is performed.
        These cannot be created automatically as the branch metadata, such as
        the description, can not be automatically populated.

        :param path: A valid node path.
        :param content: A :class:`~sal.core.object.Branch` or :class:`~sal.core.object.DataObject` instance.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        # check path is valid and dismantle
        segments, revision, is_absolute = decompose(path)
        if not is_absolute:
            raise ValueError("The supplied path must be an absolute path.")
        if revision > 0:
            raise ValueError("Put operations may only be performed on the head revision.")

        # check content - fail early
        if not isinstance(content, (Branch, DataObject)):
            raise TypeError("The put content must be a DataClass or Branch instance.")

        # TODO: check write permissions before sending content (fail early)

        # make request
        url = _PUT_URL.format(host=self.host, path='/'.join(segments))
        payload = serialise(content)
        self._make_post_request(url, payload=payload)

    def delete(self, path):
        """
        Delete the node specified by the path.

        If a branch node is deleted, all its descendants are also deleted.

        :param path: A valid node path.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        # check path is valid and dismantle
        segments, revision, is_absolute = decompose(path)
        if not is_absolute:
            raise ValueError("The supplied path must be an absolute path.")
        if revision > 0:
            raise ValueError("Put operations may only be performed on the head revision.")

        # make request
        url = _DELETE_URL.format(host=self.host, path='/'.join(segments))
        self._make_delete_request(url)

    def copy(self, target, source):
        """
        Copy the node specified by the source path to the target path location.

        Copy will not create any missing intermediate nodes to the target path,
        these must be created manually before the copy operation is performed.
        These cannot be created automatically as the branch metadata, such as
        the description, cannot be automatically populated.

        :param target: A target node path.
        :param source: A source node path.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        # check source path is valid and dismantle
        source_segments, source_revision, is_absolute = decompose(source)
        if not is_absolute:
            raise ValueError("The supplied source path must be an absolute path.")

        # check target path is valid and dismantle
        target_segments, target_revision, is_absolute = decompose(target)
        if not is_absolute:
            raise ValueError("The supplied target path must be an absolute path.")
        if target_revision > 0:
            raise ValueError("Put operations may only be performed on the head revision.")

        # make request
        url = _COPY_URL.format(
            host=self.host, path='/'.join(target_segments),
            source_path='/'+'/'.join(target_segments), source_revision=source_revision
        )
        self._make_post_request(url)

    def _make_get_request(self, url, valid_code=200, **kwargs):
        """
        Makes a get request and handles errors.

        :param url: Host URL.
        :param valid_code: HTTP return code of a valid response (default=200).
        :return: A response object.
        """

        return self._make_request(requests.get, url, valid_code=valid_code, **kwargs)

    def _make_post_request(self, url, payload=None, valid_code=204, **kwargs):
        """
        Makes a post request and handles errors.

        :param url: Host URL.
        :param payload: Optional JSON payload (default=None).
        :param valid_code: HTTP return code of a valid response (default=204).
        """

        return self._make_request(requests.post, url, valid_code=valid_code, json=payload, **kwargs)

    def _make_delete_request(self, url, valid_code=204, **kwargs):
        """
        Makes a delete request and handles errors.

        :param url: Host URL.
        :param valid_code: HTTP return code of a valid response (default=204).
        """

        return self._make_request(requests.delete, url, valid_code=valid_code, **kwargs)

    def _make_request(self, method, url, *args, valid_code=200, **kwargs):
        """
        Makes a request using the specified requests package method.

        This method performs authentication and error handling for all
        requests. Any additional arguments (args and kwargs) specified in
        calling this method are passed to the request method.

        :param: method: Requests package method (e.g. requests.get).
        :param url: Host URL.
        :param valid_code: HTTP return code of a valid response (default=200).
        :return: A response object.
        """

        def _get_response(url, *args, **kwargs):
            # disable warnings unless enabled at python command line
            # added to prevent SSL cert warnings being output by requests when the user permits invalid SSL certificates
            with warnings.catch_warnings():
                warnings.simplefilter('ignore')
                try:
                    response = method(url, *args, verify=self.verify_https_cert, **kwargs)
                except requests.exceptions.SSLError:
                    raise ConnectionError('The host\'s HTTPS certificate is invalid, please contact the server admin.')
                except requests.exceptions.RequestException:
                    raise ConnectionError('The server did not respond ({}).'.format(self._host))
            return response

        if self.auth_required:

            # handle authentication
            auth_attempted = False
            while True:

                # if we don't have a token we need to obtain one
                if not self.auth_token:

                    # is an auth token stored?
                    if not self._read_auth_token():
                        self.authenticate(credentials=self.credentials_file)
                        self._write_auth_token()
                        auth_attempted = True

                # attempt request
                headers = {'Authorization': 'Bearer {}'.format(self.auth_token)}
                response = _get_response(url, *args, headers=headers, **kwargs)

                # did the request fail due to an expired token?
                if response.status_code == 401:

                    # renew authentication token (may ask user for user/password)
                    self.auth_token = None
                    self._clear_auth_token()
                    if not auth_attempted:
                        # if self.prompt_for_password:
                        #     print('Authorisation token has expired, attempting to reacquire.')
                        continue

                self._validate_response(response, valid_code)
                return response

        else:

            # no authentication handling
            response = _get_response(url, *args, **kwargs)
            self._validate_response(response, valid_code)
            return response

    @staticmethod
    def _is_valid_url(urlstring):
        """
        Parses a url to check if it is valid.

        :param urlstring: The url string to validate.
        :return: True if valid, False otherwise.
        """

        r = urlparse(urlstring)
        if r.scheme and r.netloc:
            return True
        return False

    def _validate_response(self, response, valid_code):
        """
        Checks the server response is valid.

        This methods checks the response content type is correct and handles
        the reporting or errors to the user.

        :param response: A Response object.
        :param valid_code: The http status code expected in the response.
        """

        # response must be json
        if _MIME_JSON not in response.headers['Content-Type'].lower():
            raise exception.InvalidResponse('Server did not return valid data.')

        # handle errors
        if response.status_code != valid_code:
            self._handle_error(response)

    def _handle_error(self, response):
        """
        Parses errors sent from the server to the client.

        Raises exceptions depending on the response sent by the server.

        :param response: A Response object.
        """

        # decode and raise appropriate exception
        try:
            content = response.json()
            raise _EXCEPTION_MAP[content['exception']](message=content['message'])
        except (ValueError, KeyError):
            # catch all for unrecognised errors
            raise exception.InternalError(message='An unidentified error has occurred, please contact your system administrator.')
