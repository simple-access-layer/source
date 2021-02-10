"""
Tests related to SALServer

.. note::
    Testing of API resources (added during server initilisation) is done
    using functional tests
"""


def test_authenticator_validation():

    """
    Tests that server with valid authentication is validated

    GIVEN
        A server with an authenticator
        AND a secret key greater than the minimum length 
        AND a non-zero token lifetime
    WHEN
        The server attemped to validate the authentication
    THEN
        The authentication is valid
    """

    pass


def test_authenticator_validation_short_key():

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

    pass


def test_authenticator_validation_invalid_token_lifetime():

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

    pass