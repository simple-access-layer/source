"""
Tests of how the client handles invalid server responses and errors

Valid server responses are covered by other test modules
"""

from unittest.mock import patch

import pytest

from sal.client.main import SALClient, exception


@pytest.fixture
def xml_response(server_response):

    server_response.headers['Content-Type'] = 'application/xml'
    return server_response


def test_validate_xml_response(host, xml_response):

    """
    GIVEN
        A host which returns XML responses
    WHEN
        The client connects to the host
    THEN
        An InvalidResponse is raised
    """

    with patch('sal.client.main.requests.get', return_value=xml_response):
        with pytest.raises(exception.InvalidResponse):
            SALClient(host)


def test_validate_error_response():

    """
    GIVEN
        A response with a client or server error status
    WHEN
        The response is validated
    THEN
        The relevant exception is raised
    """


    