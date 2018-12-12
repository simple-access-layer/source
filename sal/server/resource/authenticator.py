from flask_restful import Resource, request, current_app, abort

from sal.core import exception
from sal.server.auth import auth_required, generate_token


class Authenticator(Resource):
    """
    Implements basic authentication endpoint that returns access tokens.

    Presents a challenge to clients, requesting username and password via basic
    authentication. A time limited access token will be returned if valid
    credentials are supplied. The token is required to gain access to the data
    and permissions endpoints on an authenticated server.

    .. warning::
        The server *MUST* use HTTPS to encrypt the connection as the password is
        sent as encoded, but not encrypted, clear text.
    """

    def __init__(self):
        self.authentication_provider = current_app.config['SAL']['AUTHENTICATION']

    def get(self):

        # deny access if authentication not required
        if not auth_required():
            abort(403)

        # have credentials been supplied?
        auth = request.authorization
        if not auth or not self._is_authenticated(auth.username, auth.password):
            return self._request_login()

        # generate authentication token
        return self._token_response(auth.username)

    def _is_authenticated(self, username, password):
        """
        Checks the username and password to identify a valid user.

        :param username: Username string.
        :param password: Password string.
        :return:
        """

        # check for local admin account
        if current_app.config['SAL']['ADMIN_USER_ENABLED']:

            admin_username = current_app.config['SAL']['ADMIN_USERNAME']
            admin_password = current_app.config['SAL']['ADMIN_PASSWORD']

            if username == admin_username and password == admin_password:
                return True

        # check if another account
        return self.authentication_provider.authenticate(username, password)

    def _request_login(self):
        """
        Sends a 401 response requesting basic authentication.
        """

        message = {
            'message': exception.AuthenticationFailed.message,
            'status': 401,
            'exception': 'AuthenticationFailed'
        }

        headers = {
            'WWW-Authenticate': 'Basic realm="Login Required"'
        }

        return message, 401, headers

    def _token_response(self, username):
        return {
            'authorisation':
            {
                'user': username,
                'token': generate_token(username)
            }
        }

