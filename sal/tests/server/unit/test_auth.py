"""
Tests relating to token generation and verification

.. note::
    The health check suppression which is applied to some test functions is
    required so that hypothesis plays nicely with pytest: Hypothesis has the
    issue that each example on a test function isn't treated as a separate
    event for any fixtures (i.e., the fixtures aren't rebuilt for each
    example), so errors if the fixture has function scope
"""

from time import sleep
from unittest.mock import patch

import pytest
from hypothesis import HealthCheck, given, settings, strategies as st

from sal.server import auth


def test_auth_required_without_authenticator(server):

    """
    GIVEN
        No authentication provider is defined
    WHEN
        Whether authentication is required is requested  
    THEN
        Whether authentication is required is returned
    """

    assert auth.auth_required() == False


@pytest.mark.parametrize('auth_required', ['True', 'False'])
def test_auth_required_with_authenticator(server_with_auth_prov,
                                          mock_authentication_provider,
                                          auth_required):

    """
    GIVEN
        An authentication provider is defined
    WHEN
        Whether authentication is required is requested  
    THEN
        Whether authentication is required is returned
    """

    mock_authentication_provider.required.return_value = auth_required
    assert auth.auth_required() == auth_required


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(addr=st.ip_addresses(),
       user=st.text(alphabet=st.characters(whitelist_categories=('Lu',
                                                                 'Ll',
                                                                 'Nd',
                                                                 'P'))))
def test_verify_valid_token(addr, user, server_with_auth_prov):

    """
    Uses `generate_token` to create token, as the essential behaviour to test
    is that a generated token can correctly verified 

    GIVEN
        A server with a secret token key
        AND a server with a token lifetime
        AND a request from a IP address
        AND a token generated using the IP
        AND a token generated less than the token lifetime ago
        AND a token generated using the secret key
        AND a token generated with a username
    WHEN
        The token is verified
    THEN
        The username is extracted from the token
    """

    # str cast required because hypothesis generates IPv4Address and
    # IPv6Address objects
    with server_with_auth_prov.test_request_context(
        environ_base={'REMOTE_ADDR':str(addr)}):
        token = auth.generate_token(user)
        assert user == auth.verify_token(token)


def test_verify_expired_token(server_with_auth_prov):

    """
    GIVEN
        A server with a secret token key
        AND a server with a token lifetime
        AND a request from a IP address
        AND a token generated using the IP
        AND a token generated more than the token lifetime ago
        AND a token generated using the secret key
        AND a token generated with a username
    WHEN
        The token is verified
    THEN
        None is returned
    """

    user = 'username'
    server_with_auth_prov.config['SAL']['TOKEN_LIFETIME'] = 0

    with server_with_auth_prov.test_request_context(
        environ_base={'REMOTE_ADDR':'0.0.0.0'}):
        token = auth.generate_token(user)
        sleep(1)
        assert auth.verify_token(token) is None


def test_verify_token_different_ip(server_with_auth_prov):

    """
    GIVEN
        A server with a secret token key
        AND a server with a token lifetime
        AND a request from a IP address
        AND a token generated using a different IP
        AND a token generated less than the token lifetime ago
        AND a token generated using the secret key
        AND a token generated with a username
    WHEN
        The token is verified
    THEN
        None is returned
    """

    token_addr = '0.0.0.0'
    request_addr = '20.313.236.111'
    user = 'username'

    with server_with_auth_prov.test_request_context(
        environ_base={'REMOTE_ADDR':token_addr}):
        token = auth.generate_token(user)

    with server_with_auth_prov.test_request_context(
        environ_base={'REMOTE_ADDR':str(request_addr)}):
        assert auth.verify_token(token) is None


def test_verify_invalid_token(server_with_auth_prov):

    """
    Tests that an invalid token (i.e., random, or generated with a different
    secret key) is correctly verified

    GIVEN
        A server with a secret token key
        AND a server with a token lifetime
        AND a request from a IP address
        AND a token generated using the IP
        AND a token generated less than the token lifetime ago
        AND a token generated using the secret key
        AND a token generated with a username
    WHEN
        The token is verified
    THEN
        None is returned
    """

    with server_with_auth_prov.test_request_context(
        environ_base={'REMOTE_ADDR':str('0.0.0.0')}):
        token = auth.generate_token('username') 
        assert auth.verify_token(token + 'A') is None


def test_authentication_wrapper_without_auth(server):

    """
    Tests that a function can be wrapped so that the username is added to the
    **kwargs when authentication is not required

    GIVEN
        A server which does not require authentication
        AND a function which returns the username
    WHEN
        The function is wrapped with ``authenticated_endpoint``
        AND the function is called
    THEN
        None is returned
    """

    def get_user(*args, **kwargs):
        return kwargs['user']

    with server.test_request_context(): 
        # Check that the unwrapped function does not return the user
        with pytest.raises(KeyError):
            get_user()
        get_user = auth.authenticated_endpoint(get_user)
        assert get_user() is None


def test_authentication_wrapper_with_auth(server_with_auth_prov):

    """
    Tests that a function can be wrapped so that the username is added to the
    **kwargs when authentication is required

    GIVEN
        A server which requires authentication
        AND a function which returns the username
    WHEN
        The function is wrapped with ``authenticated_endpoint``
        AND the function is called
    THEN
        The username is returned
    """

    user = 'username'
    headers = {'Authorization':'Bearer {}'.format('arb')}

    def get_user(*args, **kwargs):
        return kwargs['user']

    with server_with_auth_prov.test_request_context(headers=headers): 
        # Check that the unwrapped function does not return the user
        with pytest.raises(KeyError):
            get_user()

        # authenticated_endpoint gets username by verifying the token
        with patch('sal.server.auth.verify_token', return_value=user):
            get_user = auth.authenticated_endpoint(get_user)
            assert get_user() == user


# Hypothesis set to only use URL safe characters
@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(token=st.text(min_size=10,
                     alphabet=st.characters(whitelist_characters=('_',
                                                                  '-',
                                                                  '.'),
                                            whitelist_categories=('Lu',
                                                                  'Ll',
                                                                  'Nd'))))
def test_extracting_token_from_header(token, server_with_auth_prov):

    """
    GIVEN
        A request with an authorization header with "Bearer" followed by the
        token
    WHEN
        The token is extracted from the header 
    THEN
        The token is returned
    """

    headers = {'Authorization': 'Bearer {}'.format(token)}

    with server_with_auth_prov.test_request_context(headers=headers):
        assert auth._extract_token_header() == token

