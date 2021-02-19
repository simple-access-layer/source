
from unittest.mock import Mock

import pytest
import requests


@pytest.fixture
def server_root_response():

    """
    200 response from server when connecting to root
    """

    server_response = Mock(spec=requests.Response)
    server_response.status_code = 200
    server_response.headers = {'Date': 'Tue, 16 Feb 2021 17:48:15 GMT',
                               'Content-Type': 'application/json',
                               'Content-Length': '347',
                               'Connection': 'keep-alive'}
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
def server_auth_response():

    """
    200 response from server when connecting to auth with valid credentials
    """

    server_response = Mock(spec=requests.Response)
    server_response.status_code = 200
    server_response.headers = {'Date': 'Tue, 16 Feb 2021 17:48:15 GMT',
                               'Content-Type': 'application/json',
                               'Content-Length': '347',
                               'Connection': 'keep-alive'}
    server_response.json.return_value = {
        'authorisation': {'user': 'username',
                          'token': '78945JHKJFSJDFKH7897wej8UIOJKhuwiofdSDHk'}}

    return server_response