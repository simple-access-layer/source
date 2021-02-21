"""
Tests setting of the host on the client

.. note::
    These tests assume that the SALClient connects to the host when it is
    initialised. Although it would be preferable to test host connection using
    SALClient.host, there is no straightforward way to initialise attributes
    without connecting to the host.  
"""

from unittest.mock import patch

import requests
import pytest

from sal.client.main import SALClient, exception


def test_connect_to_valid_host(server_root_response):

    """
    GIVEN
        A URL to a valid SAL server
    WHEN
        A client connects to the host
    THEN
        The client validates the host (i.e. no exceptions are thrown)
        AND the client stores the host URL
        AND the client determines if the host has authentication enabled
    """

    host = 'https://sal.testing'
    requires_auth = server_root_response.json()['api']['requires_auth']

    with patch('sal.client.main.requests.get',
                return_value=server_root_response):
        sc = SALClient(host)
        assert sc.host == host
        assert sc.auth_required == requires_auth



@pytest.mark.xfail(reason='AttributeError when SALClient._host undefined',
                   raises=AttributeError)
def test_connect_to_host_no_response():

    """
    GIVEN
        A URL to which the client cannot connect
    WHEN
        A client attempts to connect to the host
    THEN
        The client raises a ConnectionError
    """

    with patch('sal.client.main.requests.get',
               side_effect=requests.ConnectionError()):
        with pytest.raises(ConnectionError):
            SALClient('https://sal.testing')



def test_connect_to_host_not_sal_server(server_root_response):

    """
    GIVEN
        A URL to which the client can connect, but the host is not a SAL server
    WHEN
        A client connects to the host
    THEN
        The client raises a ConnectionError
    """

    server_root_response.json.return_value['service']['name'] = 'Generic Server'

    with patch('sal.client.main.requests.get',
               return_value=server_root_response):
        with pytest.raises(ConnectionError):
            SALClient('https://sal.testing')


def test_connect_to_host_with_new_api(server_root_response):

    """
    GIVEN
        A URL for a SAL server with a newer API than the client
    WHEN
        The client connects to the host
    THEN
        The client raises a ConnectionError
    """

    server_root_response.json.return_value['api']['version'] = 3

    with patch('sal.client.main.requests.get',
               return_value=server_root_response):
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


# OSError is raised because client tries to authenticate without password,
# which requires stdin (which pytest complains about)
@pytest.mark.xfail(reason='Connecting to new host uses old host auth_required',
                   raises=OSError)
def test_connect_to_new_host(server_root_response):

    """
    GIVEN
        A client which has previously connected to a valid host
    WHEN
        The client changes to a new valid host
    THEN
        The client validates the new host (i.e. no exceptions are thrown)
        AND the client stores the new host URL (overwriting the old host URL)
        AND the client determines if the new host has authentication enabled
    """

    host = 'https://sal.testing'
    new_host = 'https://sal.new'

    with patch('sal.client.main.requests.get',
                return_value=server_root_response):
        sc = SALClient(host)
    
    new_host_requires_auth = not sc.auth_required
    new_response = server_root_response.json()
    new_response['api']['requires_auth'] = new_host_requires_auth
    
    with patch('sal.client.main.requests.get',
                return_value=new_response):
        assert sc.auth_required == True
        sc.host = new_host
        assert sc.host == new_host
        assert sc.auth_required == False