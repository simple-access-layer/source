def test_auth_required_with_authenticator():

    """
    Tests whether the authentication requirement is correctly determined
    
    GIVEN
        An authentication provider is defined
    WHEN
        Whether authentication is required is requested  
    THEN
        Whether authentication is required is returned
    """

    pass


def test_verify_valid_token():

    """
    Tests that a token is correctly verified

    GIVEN
        A server with a token secret
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

    pass


def test_verify_expired_token():

    """
    Tests that an expired token is correctly verified

    GIVEN
        A server with a token secret
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

    pass


def test_verify_token_different_ip():

    """
    Tests that a token with a different IP is correctly verified

    GIVEN
        A server with a token secret
        AND a server with a token lifetime
        AND a request from a IP address
        AND a token generated using a different IP
        AND a token generated more than the token lifetime ago
        AND a token generated using the secret key
        AND a token generated with a username
    WHEN
        The token is verified
    THEN
        None is returned
    """

    pass


def test_verify_invalid_token():

    """
    Tests that an invalid token (i.e., random, or generated with a different
    secret key) is correctly verified

    GIVEN
        A server with a token secret
        AND a server with a token lifetime
        AND a request from a IP address
        AND a token generated using a different IP
        AND a token generated more than the token lifetime ago
        AND a token generated using the secret key
        AND a token generated with a username
    WHEN
        The token is verified
    THEN
        None is returned
    """

    pass


def test_authentication_wrapper_without_auth():

    """
    Tests that a function can be wrapped so that the username is added to the
    **kwargs when authentication is not required

    GIVEN
        A server which does not required authentication
        AND a function which returns the username
    WHEN
        The function is wrapped with ``authenticated_endpoint``
        AND the function is called
    THEN
        None is returned
    """

    pass

def test_extracting_token_from_header():

    """
    GIVEN
        A request with an authorization header with "Bearer" followed by the
        token
    WHEN
        The token is extracted from the header 
    THEN
        The token is returned
    """

    pass