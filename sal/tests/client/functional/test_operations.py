"""
Tests the SAL operations: list, get, put, copy, delete

.. note::
    Tests do not check all attributes of returned objects as deserialising
    objects is not directly a SALClient responsibility
"""

from datetime import datetime
from unittest.mock import patch

import pytest

from sal.core.serialise import serialise
from sal.core.object import BranchReport, LeafReport, Branch, DataObject 


@pytest.fixture
def timestamp():

    return datetime(2000, 1, 1, 12, 00, 00)


@pytest.fixture
def branch_report_response(server_response, timestamp):

    """
    JSON representing a BranchReport
    """

    br = BranchReport('A BranchReport', ['b1', 'b2', 'b3'], [], timestamp)
    server_response.json.return_value = serialise(br)
    return server_response


@pytest.fixture
def leaf_report_response():

    """
    JSON representing a LeafReport
    """

    pass


@pytest.fixture
def branch_response():

    """
    JSON representing a Branch
    """


@pytest.fixture
def data_object_response():

    """
    JSON representing a DataObject
    """

    pass


@pytest.mark.parametrize('path, revision, verify_https_cert',
                         [('/node', 0, False),
                          ('/node:head', 0, True),
                          ('/node:10', 10, False)])
def test_list_branch(host, patched_client, timestamp, branch_report_response,
                     path, revision, verify_https_cert):

    """
    Parametrization includes the three different ways of passing a revision

    GIVEN
        A server with data nodes
    WHEN
        A client performs a list operation on a path
    THEN
        The client returns either a BranchReport or a LeafReport
    """

    patched_client.verify_https_cert = verify_https_cert

    with patch('sal.client.main.requests.get',
               return_value=branch_report_response) as brr:
        br = patched_client.list(path)

        brr.assert_called_with('{0}/data/node?revision={1}'.format(host,
                                                                   revision),
                                      verify=verify_https_cert)
        assert isinstance(br, BranchReport)
        assert br.timestamp == timestamp


def test_list_with_auth():

    """
    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a list operation
    THEN
        The token is included in a request header
        AND the client returns either a BranchReport or a LeafReport
    """

def test_get():

    """
    GIVEN
        A server with data nodes
    WHEN
        A client performs a get operation
    THEN
        The client returns either a Branch or a DataObject
    """

    # Include '/data:head', '/data', and '/data:10' examples


def test_get_with_auth():

    """
    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a get operation
    THEN
        The token is included in a request header
        AND the client returns either a Branch or a DataObject
    """


def test_put_create():

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a put operation with a path that does not exist on
        the server
    THEN
        A new data node is created on the server
    """


def test_put_update():

    """
    GIVEN
        A server with data nodes which requires authentication
    WHEN
        The client performs a put operation with a path that does exist on the
        server 
    THEN
        The server data node corresponding to the path is replaced
    """


def test_put_with_auth():

    """
    GIVEN
        A server with data nodes
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a put operation with a path that does not exist on
        the server
    THEN
        The token is included in a request header
        AND a new data node is created on the server
    """


def test_copy():

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a copy operation from a source to a target
    THEN
        Data is copied from the source to the target on the server
    """

    # Include '/data:head', '/data', and '/data:10' examples for both target
    # and source


def test_copy_with_auth():

    """
    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a copy operation from a source to a target
    THEN
        Data is copied from the source to the target on the server
    """


def test_delete():

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a delete operation
    THEN
        Data is delete from the specified path 
    """

