
from unittest.mock import Mock, patch

import pytest
import requests

from sal.client.main import SALClient


@pytest.fixture
def host():

    return 'https://sal.testing'


@pytest.fixture
def token():

    return '78945JHKJFSJDFKH7897wej8UIOJKhuwiofdSDHk'


@pytest.fixture
def patched_client(host):

    """
    Patches SALClient host setting (which normally connects to a host server)
    so that SALClient can be initialized.
    """

    def host_setter(self, url):
        self._host = url

    patched_host = SALClient.host.setter(host_setter)

    with patch.object(SALClient, 'host', patched_host):
        return SALClient(host)


@pytest.fixture
def patched_client_with_auth(patched_client):

    """
    A SALClient connected to a server which requires authentication
    """

    patched_client.auth_required = True
    return patched_client


@pytest.fixture
def server_response():

    """
    200 response from server
    """

    server_response = Mock(spec=requests.Response)
    server_response.status_code = 200
    server_response.headers = {'Date': 'Tue, 16 Feb 2021 17:48:15 GMT',
                               'Content-Type': 'application/json',
                               'Content-Length': '347',
                               'Connection': 'keep-alive'}
    return server_response


@pytest.fixture
def server_root_response(server_response):

    """
    200 response from server when connecting to root
    """


    server_response.json.return_value = {
        'host': 'https://sal.testing/',
        'api': {'version': 2,
                'requires_auth': True,
                'resources': ['data', 'auth'],
                'classes': {'core': ['dictionary',
                                     'scalar',
                                     'string',
                                     'array']}},
        'service': {'name': 'Simple Access Layer (SAL) Server',
                    'version': '1.2.2'},
        'request': {'url': 'https://sal.testing/'}}

    return server_response


@pytest.fixture
def server_auth_response(server_response, token):

    """
    200 response from server when connecting to auth with valid credentials
    """

    server_response.json.return_value = {
        'authorisation': {'user': 'username',
                          'token': token}}

    return server_response



@pytest.fixture
def no_content_response(server_response):

    server_response.status_code = 204
    return server_response