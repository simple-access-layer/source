"""
Tests setting of the host on the client    
"""


def test_connect_to_valid_host():

    """
    GIVEN
        A URL to a valid SAL server
    WHEN
        A client connects to the host
    THEN
        The URL and server are validated
    """


def test_connect_to_host_no_response():

    """
    GIVEN
        A URL to which the client cannot connect
    WHEN
        A client attempts to connect to the host
    THEN
        The client raises a requests.ConnectionError
    """


def test_connect_to_host_not_sal_server():

    """
    GIVEN
        A URL to which the client can connect, but the host is not a SAL server
    WHEN
        A client connects to the host
    THEN
        The client raises a requests.ConnectionError
    """


def test_connect_to_host_with_new_api():

    """
    GIVEN
        A URL for a SAL server with a newer API than the client
    WHEN
        The client connects to the host
    THEN
        The client raises a requests.ConnectionError
    """


def test_connect_to_host_invalid_url():

    """
    GIVEN
        An invalid URL
    WHEN
        A client attempts to connect
    THEN
        The client raises a ValueError
    """

