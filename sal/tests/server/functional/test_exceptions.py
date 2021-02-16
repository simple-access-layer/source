"""
Tests server exception responses
"""

import json
from unittest.mock import patch

import pytest

import sal.core.exception as exc


@pytest.mark.parametrize('exception, code',
                         [(exc.InvalidRequest, 400),
                          (exc.AuthenticationFailed, 401),
                          (exc.PermissionDenied, 403),
                          (exc.InvalidPath, 404),
                          (exc.NodeNotFound, 404),
                          (exc.UnsupportedOperation, 500),
                          (exc.InternalError, 500)])
def test_exception_type_response(server, exception, code):

    """
    GIVEN
        A server which maps internal exceptions to HTTP responses    
        AND the server raises a specified exception on GET root
    WHEN
        A client uses GET on the server root
    THEN
        The response contains the corresponding status code and message
    """

    def raise_exception(self):
        raise exception

    with server.test_client() as client:
        with patch('sal.server.main.ServerInfo.get', new=raise_exception):
            # Confirm that the expected exception raised
            with pytest.raises(exception):
                client.get()

            # Stop exception being propagated from server to client
            # This enables testing that the correct code and message (that
            # corresponds to the exception raised) are sent to the client
            server.testing = False
            response = client.get()
            assert code == response.status_code
            # Convert response data from str to test message 
            assert exception.message == json.loads(response.data)['message']
