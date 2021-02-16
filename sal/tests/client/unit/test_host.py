"""
Tests setting of the host on the client

.. note::
    These tests assume that the SALClient connects to the host when it is
    initialised. Although it would be preferable to test host connection using
    SALClient.host, there is no straightforward way to initialise attributes
    without connecting to the host.  
"""

from unittest.mock import Mock, patch

import requests
import pytest

from sal.client.main import SALClient, exception


@pytest.fixture
def server_response_ok():

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
        'resources': ['data'],
        'classes': {'core': ['dictionary',
                             'scalar',
                             'string',
                             'array']}},
        'service': {'name': 'Simple Access Layer (SAL) Server',
                    'version': '1.2.2'},
        'request': {'url': 'https://sal.testing/'}}

    return server_response


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
        The client raises a ConnectionError
    """


def test_connect_to_host_not_sal_server():

    """
    GIVEN
        A URL to which the client can connect, but the host is not a SAL server
    WHEN
        A client connects to the host
    THEN
        The client raises a ConnectionError
    """


def test_connect_to_host_with_new_api(server_response_ok):

    """
    GIVEN
        A URL for a SAL server with a newer API than the client
    WHEN
        The client connects to the host
    THEN
        The client raises a ConnectionError
    """

    server_response_ok.json.return_value['api']['version'] = 3

    with patch('sal.client.main.requests.get',
               return_value=server_response_ok):
        with pytest.raises(ConnectionError):
            SALClient('https://sal.testing')


def test_connect_to_host_invalid_url():

    """
    GIVEN
        An invalid URL
    WHEN
        A client attempts to connect
    THEN
        The client raises a ValueError
    """

    with pytest.raises(ValueError):
        SALClient('989s0d8fisdfk')