from flask import Flask
from flask_restful import Api

from sal.core.version import VERSION as RELEASE_VERSION
from sal.core import exception
from sal.server.interface import PersistenceProvider, AuthenticationProvider, AuthorisationProvider
from sal.server.resource import ServerInfo, DataTree, Authenticator
from sal.dataclass import *

API_VERSION = 2
AUTH_TOKEN_SECRET_MIN_LENGTH = 32


# TODO: add docstring
class SALServer(Flask):
    """


    generate a random 64 char (32 byte) hex string as a secret
    to generate a good key: binascii.hexlify(os.urandom(32)).decode('ascii')
    warning: do not generate dynamically in the configuration script otherwise each worker process will generate a different key!
    """

    def __init__(self, persistence_provider, authentication_provider=None, authorisation_provider=None,
                 auth_token_secret=None, auth_token_lifetime=None, admin_enabled=False, admin_username=None, admin_password=None,
                 *args, **kwargs):

        # pass on flask configuration arguments
        super().__init__(__name__, *args, **kwargs)

        # validate server configuration
        self._validate_persistence(persistence_provider)
        self._validate_auth(authentication_provider, authorisation_provider, auth_token_secret, auth_token_lifetime)
        self._validate_admin(admin_enabled, admin_username, admin_password)

        # add to flask configuration object
        self.config['SAL'] = {
            'PERSISTENCE': persistence_provider,
            'AUTHENTICATION': authentication_provider,
            'AUTHORISATION': authorisation_provider,
            'TOKEN_SECRET': auth_token_secret,
            'TOKEN_LIFETIME': auth_token_lifetime,
            'ADMIN_USER_ENABLED': bool(admin_enabled),
            'ADMIN_USERNAME': admin_username,
            'ADMIN_PASSWORD': admin_password,
            'API_VERSION': API_VERSION
        }

        # exception handling mapping table
        error_map = {
            'InvalidRequest': {
                'message': exception.InvalidRequest.message,
                'status': 400,
                'exception': 'InvalidRequest'
            },
            'AuthenticationFailed': {
                'message': exception.AuthenticationFailed.message,
                'status': 401,
                'exception': 'AuthenticationFailed'
            },
            'PermissionDenied': {
                'message': exception.PermissionDenied.message,
                'status': 403,
                'exception': 'PermissionDenied'
            },
            'InvalidPath': {
                'message': exception.InvalidPath.message,
                'status': 404,
                'exception': 'InvalidPath'
            },
            'NodeNotFound': {
                'message': exception.NodeNotFound.message,
                'status': 404,
                'exception': 'NodeNotFound'
            },
            'UnsupportedOperation': {
                'message': exception.UnsupportedOperation.message,
                'status': 500,
                'exception': 'UnsupportedOperation'
            },
            'InternalError': {
                'message': exception.InternalError.message,
                'status': 500,
                'exception': 'InternalError'
            },
        }

        # build api
        api = Api(self, errors=error_map)
        api.add_resource(ServerInfo, '/')
        api.add_resource(DataTree, '/data', '/data/', '/data/<path:path>')
        api.add_resource(Authenticator, '/auth', '/auth/')

        # todo: enable when permission system is implemented
        # api.add_resource(PermissionsTree, '/permission', '/permission/', '/permission/<path:path>')

        self._print_welcome()

    @staticmethod
    def _validate_persistence(persistence_provider):
        """
        Validates the persistence settings.
        """

        if not isinstance(persistence_provider, PersistenceProvider):
            raise TypeError('The persistence provider must be a subclass of PersistenceProvider.')

    @staticmethod
    def _validate_auth(authentication_provider, authorisation_provider, auth_token_secret, auth_token_lifetime):
        """
        Validates the authentication settings.
        """

        if authentication_provider:

            if not isinstance(authentication_provider, AuthenticationProvider):
                raise TypeError('The authentication provider must be a subclass of AuthenticationProvider.')

            if not auth_token_secret:
                raise ValueError('Authentication token secret has not been set.')

            if len(auth_token_secret) < AUTH_TOKEN_SECRET_MIN_LENGTH:
                raise ValueError('Authentication token secret length is too short,'
                                 ' it must be at least {} characters long.'.format(AUTH_TOKEN_SECRET_MIN_LENGTH))

            if not auth_token_lifetime or auth_token_lifetime <= 0:
                raise ValueError('Authentication token lifetime has not been set.')

        if authorisation_provider:

            if not isinstance(authorisation_provider, AuthorisationProvider):
                raise TypeError('The authorisation provider must be a subclass of AuthorisationProvider.')

            if not authentication_provider:
                raise ValueError('An authentication provider is required if authorisation is required.')

    @staticmethod
    def _validate_admin(admin_enabled, admin_username, admin_password):
        """
        Validates the admin user settings.
        """

        if admin_enabled:

            if not admin_username:
                raise ValueError('Admin account is enabled but no username has been set.')

            if not admin_password:
                raise ValueError('Admin account is enabled but no password has been set.')

    def _print_welcome(self):
        """
        Displays the server welcome text.
        """

        cfg = self.config['SAL']
        persistence = cfg['PERSISTENCE']
        authentication = cfg['AUTHENTICATION']
        authorisation = cfg['AUTHORISATION']
        admin_enabled = cfg['ADMIN_USER_ENABLED']

        s = '\nSimple Access Layer (SAL) Server v{}\n' \
            '{}\n\n' \
            'Configuration:\n' \
            ' * API version: {}\n' \
            ' * Persistence handler: {}\n' \
            ' * Authentication handler: {}\n' \
            ' * Authorisation handler: {}\n' \
            ' * Administration user account: {}{}\n'

        underline = '-' * (34 + len(RELEASE_VERSION))

        persistence_name = '{} (v{})'.format(persistence.NAME, persistence.VERSION)

        if authentication:
            authentication_name = '{} (v{})'.format(authentication.NAME, authentication.VERSION)
        else:
            authentication_name = 'None'

        if authorisation:
            authorisation_name = '{} (v{})'.format(authorisation.NAME, authorisation.VERSION)
        else:
            authorisation_name = 'None'

        if admin_enabled:
            admin_user_state = 'Enabled'
            admin_user_warn = ' - WARNING!'
        else:
            admin_user_state = 'Disabled'
            admin_user_warn = ''

        print(s.format(
            RELEASE_VERSION, underline, API_VERSION,
            persistence_name, authentication_name, authorisation_name,
            admin_user_state, admin_user_warn
        ))

    def __call__(self, environ, start_response):

        # reverse proxy handling
        script_name = environ.get('HTTP_X_SCRIPT_NAME', '')
        if script_name and script_name != '/':

            environ['SCRIPT_NAME'] = script_name
            path_info = environ['PATH_INFO']

            if path_info.startswith(script_name):
                environ['PATH_INFO'] = path_info[len(script_name):]

        scheme = environ.get('HTTP_X_SCHEME', '')
        if scheme:
            environ['wsgi.url_scheme'] = scheme

        server = environ.get('HTTP_X_FORWARDED_SERVER', '')
        if server:
            environ['HTTP_HOST'] = server

        # obtain client address if reverse proxied (first address in HTTP_X_FORWARDED_FOR)
        if environ.get('HTTP_X_FORWARDED_FOR'):
            environ['REMOTE_ADDR'] = environ['HTTP_X_FORWARDED_FOR'].split(',')[0].strip()

        return super().__call__(environ, start_response)





