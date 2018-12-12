"""
Utility functions for generating and validating authentication tokens.
"""

from functools import wraps
from flask_restful import reqparse, request, current_app
from itsdangerous import URLSafeTimedSerializer, SignatureExpired, BadSignature
from sal.core.exception import AuthenticationFailed


def auth_required():
    """
    Checks if the route is disabled.

    :return: True/False
    """

    authentication_provider = current_app.config['SAL']['AUTHENTICATION']
    if not authentication_provider:
        return False
    return authentication_provider.required()


def generate_token(user):
    """
    Produces URL-safe cryptographically signed authentication token.

    The token contains the user name of the user that requested the token.

    :param user: The user name string.
    :return: A URL-safe authorisation token string.
    """

    secret = current_app.config['SAL']['TOKEN_SECRET']
    serializer = URLSafeTimedSerializer(secret)
    return serializer.dumps((user, request.remote_addr))


def verify_token(token):
    """
    Checks if the token is valid.

    If the token is valid the user name associated with the token will be
    decoded and returned. If the token has expired or is otherwise invalid,
    this function will return None.

    :param token: A token string.
    :return: The user name if valid, else None.
    """

    secret = current_app.config['SAL']['TOKEN_SECRET']
    lifetime = current_app.config['SAL']['TOKEN_LIFETIME']
    serializer = URLSafeTimedSerializer(secret)

    # extract data from token
    try:
        data = serializer.loads(token, max_age=lifetime)
    except SignatureExpired:
        # valid token, but expired
        return None
    except BadSignature:
        # invalid token
        return None

    # unpack contents
    try:
        user, ip = data
    except ValueError:
        return None

    # check client ip is the same as the token
    if ip != request.remote_addr:
        return None

    return user


def authenticated_endpoint(f):
    @wraps(f)
    def wrapper(*args, **kwargs):

        # if there is no authentication provider, authentication is disabled
        if not auth_required():
            kwargs.update({'user': None})
            return f(*args, **kwargs)

        # take the first token found when inspecting header and then the query args (header takes precedence)
        token = _extract_token_header() or _extract_token_query()

        # access is forbidden if there is no token
        if not token:
            raise AuthenticationFailed('Invalid authentication token supplied.')

        # check token is valid
        user = verify_token(token)
        if not user:
            raise AuthenticationFailed('Authentication token has expired.')

        # add user to arguments and call
        kwargs.update({'user': user})
        return f(*args, **kwargs)

    return wrapper


_header_parser = reqparse.RequestParser()
_header_parser.add_argument('Authorization', location='headers', type=str, case_sensitive=True, default=None)


def _extract_token_header():

    # obtain Authorization header content
    try:
        headers = _header_parser.parse_args()
        content = headers['Authorization']
    except KeyError:
        return None

    # do we have content?
    if not content:
        return None

    # check the content string has only two sections
    content = content.strip().split()
    if len(content) > 2:
        return None

    auth_type, token = content

    # we are expecting a bearer token.
    if auth_type != 'Bearer':
        return None
    return token


_query_parser = reqparse.RequestParser()
_query_parser.add_argument('auth', location='args', dest='token', type=str, case_sensitive=True, default=None)


def _extract_token_query():
    try:
        args = _query_parser.parse_args()
        token = args['token']
    except KeyError:
        token = None
    return token

