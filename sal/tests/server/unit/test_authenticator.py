"""
Tests the authentication endpoint correctly challenges clients and provides
tokens
"""

def get_token_without_auth_required():

    """
    GIVEN
        A server which does not require authentication
    WHEN
        Authentication is requested
    THEN
        A PermissionDenied (403) exception is raised
    """

    pass

def get_token_without_credentials():

    """
    GIVEN
        A server which requires authentication
        AND a request which does not supply credentials
    WHEN
        Authentication is requested
    THEN
        An AuthenticationFailed (401) exception is raised 
    """

    pass

def get_token_without_valid_credentials():

    """
    GIVEN
        A server which requires authentication
        AND a request which supplies invalid credentials
    WHEN
        Authentication is requested
    THEN
        An AuthenticationFailed (401) exception is raised
    """

    pass 

def get_token_with_valid_user_credentials():

    """
    GIVEN
        A server which requires authentication
        AND a request which supplies valid user credentials
    WHEN
        Authentication is requested
    THEN
        A token is returned
    """

    pass

def get_token_with_valid_admin_credentials():

    """
    GIVEN
        A server which requires authentication
        AND a request which supplies valid admin credentials
    WHEN
        Authentication is requested
    THEN
        A token is returned
    """

    pass