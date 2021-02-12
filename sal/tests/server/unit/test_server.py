"""
Tests related to SALServer

.. note::
    Testing of API resources (added during server initilisation) is done
    using functional tests
"""

from unittest.mock import Mock

from hypothesis import HealthCheck, given, settings, strategies as st
import pytest

from sal.server.main import AUTH_TOKEN_SECRET_MIN_LENGTH, AuthorisationProvider


# Instantiate some hypothesis strategies that can be used across multiple tests
# This saves on creating an identical strategies 
token_secret = st.text(min_size=AUTH_TOKEN_SECRET_MIN_LENGTH)
token_lifetime = st.floats(min_value=0.1)


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture],
          max_examples=10)
@given(token_secret=token_secret,
       token_lifetime=token_lifetime)
def test_authenticator_validation(token_secret,
                                  token_lifetime,
                                  server_with_auth_prov,
                                  mock_authentication_provider):

    """
    Tests that server with valid authentication is validated

    GIVEN
        A server with an authenticator
        AND a secret key greater than the minimum length 
        AND a non-zero token lifetime
    WHEN
        The server attemped to validate the authentication
    THEN
        The authentication is valid (no exceptions are raised)
    """

    server_with_auth_prov._validate_auth(mock_authentication_provider,
                                         None,
                                         token_secret,
                                         token_lifetime)


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture],
          max_examples=10)
@given(token_secret=st.text(max_size=AUTH_TOKEN_SECRET_MIN_LENGTH - 1),
       token_lifetime=token_lifetime)
def test_authenticator_validation_short_key(token_secret,
                                            token_lifetime,
                                            server_with_auth_prov,
                                            mock_authentication_provider):

    """
    Tests that a server authentication is invalid if the key length is less
    than the minimum

    GIVEN
        A server with an authenticator
        AND a secret key shorter than the minimum length
        AND a non-zero token lifetime
    WHEN
        The server attemped to validate the authentication
    THEN
        The authentication is invalid (ValueError)
    """

    with pytest.raises(ValueError):
        server_with_auth_prov._validate_auth(mock_authentication_provider,
                                             None,
                                             token_secret,
                                             token_lifetime)


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture],
          max_examples=10)
@given(token_secret=token_secret,
       token_lifetime=st.floats(max_value=0))
def test_authenticator_validation_invalid_token_lifetime(
    token_secret,
    token_lifetime,
    server_with_auth_prov,
    mock_authentication_provider):

    """
    Tests that the server authentication is invalid without a non-zero token
    lifetime

    GIVEN
        A server with an authenticator
        AND a secret key longer than the minimum length
        AND a zero or negative token lifetime
    WHEN
        The server attemped to validate the authentication
    THEN
        The authentication is invalid (ValueError)
    """

    with pytest.raises(ValueError):
        server_with_auth_prov._validate_auth(mock_authentication_provider,
                                             None,
                                             token_secret,
                                             token_lifetime)


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture],
          max_examples=10)
@given(token_secret=token_secret,
       token_lifetime=token_lifetime)
def test_authenticator_validation_authorisation_without_authentication(
    token_secret,
    token_lifetime,
    server):

    """
    Tests that the server authentication is invalid if it will authorise but
    not authenticate

    GIVEN
        A server without an authenticator
        AND the server has an authoriser
        AND a secret key longer than the minimum length
        AND a zero or negative token lifetime
    WHEN
        The server attemped to validate the authentication
    THEN
        The authentication is invalid (ValueError)
    """

    with pytest.raises(ValueError):
        server._validate_auth(None,
                              Mock(spec_set=AuthorisationProvider),
                              token_secret,
                              token_lifetime)