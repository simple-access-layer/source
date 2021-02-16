"""
Tests the authentication endpoint correctly challenges clients and provides
tokens
"""

from unittest.mock import patch

import pytest
from werkzeug.exceptions import Forbidden

from sal.server.resource.authenticator import Authenticator, generate_token
from sal.tests.server.constants import B64_ADMIN_CRED, B64_USER_CRED


def test_get_token_without_auth_required(server):

    """
    GIVEN
        A server which does not require authentication
    WHEN
        Authentication is requested
    THEN
        A PermissionDenied (403) exception is raised
    """

    auth = Authenticator()

    with server.test_request_context(path="/auth"):
        with pytest.raises(Forbidden):
            auth.get()


@pytest.mark.parametrize('headers',
                         [{},
                          {'Authorization': 'Basic ZG9nOmNhdA=='}])
def test_get_token_without_valid_credentials(server_with_auth_prov,
                                             mock_authentication_provider,
                                             headers):

    """
    GIVEN
        A server which requires authentication
        AND a request with credentials are either omitted or invalid 
    WHEN
        Authentication is requested
    THEN
        A 401 response with a 'WWW-Authenticate' header (as required by RFC
        7235) is returned
    """

    auth = Authenticator()

    with server_with_auth_prov.test_request_context(path="/auth",
                                                    headers=headers):
        response = auth.get()
        # flask_restful.Response requires status code at index 1 and headers at
        # index 2
        assert response[1] == 401
        assert 'WWW-Authenticate' in response[2].keys()


@pytest.mark.parametrize('headers, uname',
    [({'Authorization': 'Basic {}'.format(B64_USER_CRED)}, 'username'),
     ({'Authorization': 'Basic {}'.format(B64_ADMIN_CRED)}, 'admin')])
def test_get_token_with_valid_credentials(server_with_auth_prov,
                                          mock_authentication_provider,
                                          headers, uname):

    """
    Authorization credentials are base64 encoded

    First parametrization tests user credentials ('')
    Second parametrization tests admin credentials ('')

    GIVEN
        A server which requires authentication
        AND a request which supplies valid credentials
    WHEN
        Authentication is requested
    THEN
        A username dependent token is returned
    """

    auth = Authenticator()

    with patch('sal.server.resource.authenticator.generate_token') as gen_t:
        with server_with_auth_prov.test_request_context(path="/auth",
                                                        headers=headers):
            response = auth.get()
        assert set(['user', 'token']) == set(response['authorisation'].keys())
        gen_t.assert_called_with(uname)
