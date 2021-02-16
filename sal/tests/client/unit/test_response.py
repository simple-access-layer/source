"""
Tests of how the client handles server responses
"""

def test_validate_valid_response():

    """
    GIVEN
        A valid [200] server response 
    WHEN
        The response is validated
    THEN
        No exceptions are raised
    """


def test_validate_xml_response():

    """
    GIVEN
        An XML response
    WHEN
        The response is validated
    THEN
        An InvalidResponse is raised
    """


def test_validate_error_response():

    """
    GIVEN
        A response with a client or server error status
    WHEN
        The response is validated
    THEN
        The relevant exception is raised
    """
    