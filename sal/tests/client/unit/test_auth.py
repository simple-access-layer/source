"""
Tests of how the client manages authentication
"""

from contextlib import contextmanager
from pathlib import Path
from unittest.mock import Mock, patch

import pytest

from sal.client.main import (SALClient, _AUTH_DEFAULT_CREDENTIALS_PATH,
                             configparser, exception)


UN = 'username'
PW = 'password'


@pytest.fixture
def host():

    return 'https://sal.testing'

@pytest.fixture
def token(server_auth_response):

    return server_auth_response.json()['authorisation']['token']


@pytest.fixture
def patched_client(host, server_root_response):

    """
    Patches SALClient host setting (which normally connects to a host server)
    so that SALClient can be initialized.
    """

    def host_setter(self, url):
        self._host = url

    patched_host = SALClient.host.setter(host_setter)

    with patch.object(SALClient, 'host', patched_host):
        return SALClient(host)


@pytest.fixture
def patched_client_with_auth(patched_client):

    """
    A SALClient connected to a server which requires authentication
    """

    patched_client.auth_required = True
    return patched_client


@pytest.fixture
def patch_get(server_auth_response):

    return patch('sal.client.main.requests.get',
                 return_value=server_auth_response)


@pytest.mark.xfail(reason='User is not warned')
def test_authenticate_not_required(patched_client):

    """
    GIVEN
        A client with a host URL
        AND the host does not require authentication
    WHEN
        The client attempts to authenticate
    THEN
        The client is warned that authentication is not required
    """

    patched_client.auth_required = False
    with pytest.warns(UserWarning):
        SALClient.authenticate(patched_client)


def test_authenticate_with_credentials(patched_client_with_auth,
                                       server_auth_response,
                                       host,
                                       token,
                                       patch_get):

    """
    GIVEN
        A client with a host URL
        AND the host requires authentication
        AND a username passed to the client
        AND a password passed to the client
    WHEN
        The client authenticates with the host
    THEN
        The client stores a token received from the host 
    """

    with patch_get as requests_get:
        check_authentication(patched_client_with_auth,
                             requests_get,
                             token,
                             auth_args=[UN, PW],
                             request_args=['{}/auth'.format(host)],
                             request_kwargs={'auth': (UN, PW),
                                             'verify': True})

        # patched_client_with_auth.authenticate(UN, PW)
        # requests_get.assert_called_with('{}/auth'.format(host),
        #                                 auth=(UN, PW),
        #                                 verify=True)
        # assert patched_client_with_auth.auth_token == token


def check_authentication(client, requests_get, token, auth_args=[],
                         auth_kwargs={}, request_args=[], request_kwargs={}):

    client.authenticate(*auth_args, **auth_kwargs)
    requests_get.assert_called_with(*request_args, **request_kwargs)
    assert client.auth_token == token


def test_authenticate_prompt_password(patched_client_with_auth,
                                      server_auth_response,
                                      host,
                                      token,
                                      patch_get):

    """
    GIVEN
        A client with a host URL
        AND a username passed to the client
    WHEN
        The client authenticates with the host
    THEN
        The user is prompted for a password
        AND the client stores a token received from the host
    """

    with patch_get as requests_get:
        with patch('sal.client.main.getpass.getpass', return_value=PW) as gp:
            check_authentication(patched_client_with_auth,
                                requests_get,
                                token,
                                auth_args=[UN],
                                request_args=['{}/auth'.format(host)],
                                request_kwargs={'auth': (UN, PW),
                                                'verify': True})
            
            gp.assert_called()





def test_authenticate_prompt_credentials(patched_client_with_auth,
                                         server_auth_response,
                                         host,
                                         token,
                                         patch_get):

    """
    GIVEN
        A client with a host URL
    WHEN
        The client authenticates with the host
    THEN
        The user is prompted for a username
        AND the user is prompted for a password
        AND the client stores a token received from the host
    """

    with patch_get as requests_get:
        with patch('sal.client.main.getpass.getpass', return_value=PW) as gp:
            with patch('sal.client.main.input',return_value=UN) as inpt:
                patched_client_with_auth.authenticate()
            
                inpt.assert_called()
                gp.assert_called()
                requests_get.assert_called_with('{}/auth'.format(host),
                                                auth=(UN, PW),
                                                verify=True)
                assert patched_client_with_auth.auth_token == token


def test_authenticate_default_credentials_file(patched_client_with_auth,
                                               server_auth_response,
                                               host,
                                               token,
                                               patch_get):

    """
    GIVEN
        A client with credentials prompting disabled
        AND a credentials file in the default location with the format:
            [host_url]
            user = username
            password = password
    WHEN
        The client attempts to authenticate
    THEN
        A request is sent using credentials from the credentials file 
    """

    cred_file = {'https://sal.testing': {'user': UN,
                                        'password': PW}}
    def_cred_path = str(Path.home().joinpath(_AUTH_DEFAULT_CREDENTIALS_PATH))

    with patch_get as requests_get:
        with patch.object(configparser.ConfigParser,
                          '__getitem__',
                          side_effect = lambda key : cred_file[key]):
            with patch.object(configparser.ConfigParser, 'read') as cpr:
                patched_client_with_auth.authenticate()
        
                cpr.assert_called_with(def_cred_path)
        
                requests_get.assert_called_with('{}/auth'.format(host),
                                                auth=(UN, PW),
                                                verify=True)
                assert patched_client_with_auth.auth_token == token


def test_authenticate_specify_credentials_file(patched_client_with_auth,
                                               server_auth_response,
                                               host,
                                               token,
                                               patch_get):

    """
    GIVEN
        A client with credentials prompting disabled
        AND a credentials file in a specified path 
    WHEN
        The client attempts to authenticate with the credentials file
    THEN
        A request is sent using credentials from the credentials file 
    """

    cred_file = {'https://sal.testing': {'user': UN,
                                        'password': PW}}
    # Specify the credentials file path
    cred_path = '/sal/cred_file'

    with patch_get as requests_get:
        with patch.object(configparser.ConfigParser,
                          '__getitem__',
                          side_effect = lambda key : cred_file[key]):
            with patch.object(configparser.ConfigParser, 'read') as cpr:
                patched_client_with_auth.authenticate(credentials=cred_path)
        
                cpr.assert_called_with(cred_path)
                requests_get.assert_called_with('{}/auth'.format(host),
                                                auth=(UN, PW),
                                                verify=True)
                assert patched_client_with_auth.auth_token == token
                assert patched_client_with_auth.credentials_file == cred_path


def test_authenticate_replace_credentials_file(patched_client_with_auth,
                                               server_auth_response,
                                               host,
                                               token,
                                               patch_get):

    """
    GIVEN
        A client with credentials prompting disabled
        AND a credentials file in a specified path
        AND the credentials file path has previously been passed to the client 
    WHEN
        The client attempts to authenticate and passes a new credentials file
        location
    THEN
        A request is sent using credentials from the new credentials file 
    """

    cred_file = {'https://sal.testing': {'user': UN,
                                        'password': PW}}
    # Specify the credentials file path
    cred_path = '/sal/cred_file'
    patched_client_with_auth.credentials_file = cred_path
    new_cred_path = '/sal/new_cred_file'

    with patch_get as requests_get:
        with patch.object(configparser.ConfigParser,
                          '__getitem__',
                          side_effect = lambda key : cred_file[key]):
            with patch.object(configparser.ConfigParser, 'read') as cpr:
                patched_client_with_auth.authenticate(
                    credentials=new_cred_path)
        
                cpr.assert_called_with(new_cred_path)
                requests_get.assert_called_with('{}/auth'.format(host),
                                                auth=(UN, PW),
                                                verify=True)
                assert patched_client_with_auth.auth_token == token


def test_authenticate_missing_credentials_file(patched_client_with_auth):

    """
    GIVEN
        A client with credentials prompting disabled
        AND no credentials file
    WHEN
        The client attempts to authenticate
    THEN
        An exception is raised 
    """

    patched_client_with_auth.prompt_for_password = False
    with pytest.raises(exception.AuthenticationFailed):
        patched_client_with_auth.authenticate()

