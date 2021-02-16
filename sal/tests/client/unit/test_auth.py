"""
Tests of how the client manages authentication
"""


def test_authenticate_not_required():
    """
    GIVEN
        A client connected to a server which does not require authentication
    WHEN
        The client attempts to authenticate
    THEN
        No request is sent
    """


def test_authenticate_with_credentials():

    """
    GIVEN

    WHEN

    THEN
    """


def test_authenticate_prompt_password():

    """
    GIVEN

    WHEN

    THEN
    """


def test_authenticate_prompt_credentials():

    """
    GIVEN

    WHEN

    THEN
    """

def test_authenticate_default_credentials_file():

    """
    GIVEN
        A client with credentials prompting disabled
        AND a credentials file in the default location
    WHEN
        The client attempts to authenticate
    THEN
        A request is sent using credentials from the credentials file 
    """


def test_authenticate_passing_credentials_file():

    """
    GIVEN
        A client with credentials prompting disabled
        AND a credentials file
    WHEN
        The client attempts to authenticate with the credentials file
    THEN
        A request is sent using credentials from the credentials file 
    """


def test_authenticate_existing_credentials_file():

    """
    GIVEN
        A client with credentials prompting disabled
        AND a credentials file in the default location
        AND the client has the credentials file location in memory
    WHEN
        The client attempts to authenticate
    THEN
        A request is sent using credentials from the credentials file 
    """

    # This is not the current behaviour, so should be an expected fail


def test_authenticate_missing_credentials_file():

    """
    GIVEN
        A client with credentials prompting disabled
        AND no credentials file
    WHEN
        The client attempts to authenticate
    THEN
        An exception is raised 
    """


def test_authenticte_existing_token():

    """
    GIVEN
        A client with an authenticate token
    WHEN
        The client attempts to authenticate
    THEN
        The authentication token is replaced with a new token
    """

