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


@pytest.mark.parametrize('response_exception, status_code',
                         [(exception.InvalidPath, 404),
                          (exception.NodeNotFound, 404),
                          (exception.UnsupportedOperation, 500),
                          (exception.InvalidRequest, 400),
                          (exception.AuthenticationFailed, 401),
                          (exception.PermissionDenied, 403),
                          (exception.InternalError, 500)])
def test_validate_error_response(host, server_response, response_exception,
                                 status_code):

    """
    GIVEN
        A response with a client or server error status
    WHEN
        The response is validated
    THEN
        The relevant exception is raised
    """

    server_response.status_code = status_code
    exception_str = response_exception.__name__
    server_response.json.return_value = {'exception': exception_str,
                                         'message': response_exception.message}

    with patch('sal.client.main.requests.get', return_value=server_response):
        with pytest.raises(response_exception):
            SALClient(host)
    