"""
Tests server resource responses (i.e., Api correctly routes requests)
"""

import json
from unittest.mock import Mock, patch

import pytest

from sal.core.object import Branch, BranchReport, DataObject, LeafReport 


def test_get_server_info(server_with_auth_prov):

    """
    GIVEN
        A SAL server
    WHEN
        A GET request is made by a client for the root ('/') 
    THEN
        The server responds with a status code of 200
    
    .. note::
        This only tests that the endpoint has been set, and not the content of
        the response
    """

    with server_with_auth_prov.test_client() as client:
            assert client.get('/').status_code == 200


@pytest.mark.parametrize('path',
                         ['/auth',
                          '/auth/'])
def test_get_token(server_with_auth_prov, path):

    """
    GIVEN
        A server where authentication is enabled
    WHEN
        A GET request is made by a client for the authentication endpoint
    THEN
        The server responds with a status code of 200 and a token

    .. note::
        This does not test the validity of the token
    """

    headers = {'Authorization': 'Basic dXNlcm5hbWU6cGFzc3dvcmQ='}

    with server_with_auth_prov.test_client() as client:
        out = client.get(path, headers=headers)
        assert out.status_code == 200
        assert 'token' in json.loads(out.data)['Authorization']


@pytest.mark.parametrize('data_path, is_branch',
                         [('/', False),
                          ('/a/path', True),
                          ('/a/different/path', False),
                          ('/one/more/path', True)])
def test_get_report(server, mock_persistence_provider, data_path, is_branch):

    """
    GIVEN
        A server which can get a BranchReport or a LeafReport
    WHEN
        A GET request is made by a client for a specific data path 
    THEN
        The server responds with a status code of 200 and the report
        corresponding to the path
    """

    path = '/data' + data_path

    status_code, data = get_report_response(server, mock_persistence_provider,
                                            path, is_branch)

    assert status_code == 200
    assert data == {'content':'report',
                    'type': 'branch' if is_branch else 'leaf',
                    'object': {'path': data_path + ':0'},
                    'request': {'url': 'http://localhost' + path}}


@pytest.mark.parametrize('data_path, is_branch, revision',
                         [('/', False, 0),
                          ('/a/path', True, 5),
                          ('/a/different/path', False, 10)])
def test_get_report_with_revision(server, mock_persistence_provider, data_path,
                                  is_branch, revision):

    """
    GIVEN
        A server with a node with either a BranchReport or a LeafReport
    WHEN
        A GET request is made by a client for a specific data path with a
        revision included in the query 
    THEN
        The server responds with a status code of 200 and the report
        corresponding to the path
    """

    path = '/data{0}?revision={1}'.format(data_path, revision)

    status_code, data = get_report_response(server, mock_persistence_provider,
                                            path, is_branch)

    assert status_code == 200
    assert data == {'content':'report',
                    'type': 'branch' if is_branch else 'leaf',
                    'object': {'path':'{0}:{1}'.format(data_path, revision)},
                    'request': {'url': 'http://localhost' + path}}


@pytest.mark.parametrize('data_path, is_branch, is_summary',
                         [('/', True, True),
                          ('/a/path', False, True),
                          ('/a/different/path', True, False),
                          ('/one/more/path', False, False)])
def test_get_object(server, mock_persistence_provider, data_path, is_branch,
                    is_summary):

    """
    GIVEN
        A server with a node with either a Branch or DataObject
    WHEN
        A GET request is made by a client for a specific data path 
    THEN
        The server responds with a status code of 200 and the object
        corresponding to the path
    """

    obj = 'summary' if is_summary else 'full'
    path = '/data{0}?object={1}'.format(data_path, obj)
    status_code, data = get_object_response(server, mock_persistence_provider,
                                            path, is_branch)
    if is_branch:
        expected_object = {'path': data_path + ':0',
                           'summary': is_summary}
    else:
        expected_object = {'path': {'type': 'string',
                                    'value': data_path + ':0'},
                            'summary': {'type': 'bool',
                                        'value': is_summary}}

    assert status_code == 200
    assert data == {'content':'object',
                    'type': 'branch' if is_branch else 'leaf',
                    'object': expected_object,
                    'request': {'url': 'http://localhost{}'.format(path)}}


def test_get_report_with_token(server_with_auth_prov,
                               mock_persistence_provider):

    """
    GIVEN
        A server with authentication enabled with a node with a BranchReport
    WHEN
        A token is requested
        AND a GET request is made with the token for the data path
    THEN
        The server responds with a status code of 200 and the BranchReport
        corresponding to the path
    """

    path = '/data'
    mock_persistence_provider.list = lambda path : make_node_data(path, True)

    with server_with_auth_prov.test_client() as client:
        token_response = client.get('/auth',
            headers={'Authorization': 'Basic dXNlcm5hbWU6cGFzc3dvcmQ='})
        token = json.loads(token_response.data)['authorisation']['token']
        report_response = client.get(path,
            headers={'Authorization': 'Bearer {}'.format(token)})
        assert report_response.status_code == 200


def get_report_response(server, mock_persistence_provider, path, is_branch):

    """
    Gets either a BranchReport or LeafReport from the server 
    """

    # Set the persistence provider to return either a BranchReport or
    # LeafReport from any data path
    mock_persistence_provider.list = lambda path : make_node_data(path,
                                                                  is_branch)

    return get_response(server, path)


def get_object_response(server, mock_persistence_provider, path, is_branch):

    """
    Gets either a Branch or DataObject from the server
    """

    # Set the persistence provider to return either a Branch or DataObject from
    # any data path
    mock_persistence_provider.get = lambda path, summary : make_node_data(path,
        is_branch,
        summary=summary)
    return get_response(server, path)


def get_response(server, path):

    with server.test_client() as client:
        out = client.get(path)
        return out.status_code, json.loads(out.data)


def make_node_data(path, is_branch, **kwargs):

    """
    Creates a dict corresponding to either a BranchReport, LeafReport, Branch
    or DataObject
    """

    cls = select_node_class(is_branch, kwargs.get('summary'))
    noda_data = Mock(spec_set=cls)
    noda_data.to_dict.return_value = {'path': path, **kwargs}
    return noda_data


def select_node_class(is_branch, summary):

    if summary is not None:
        return Branch if is_branch else DataObject
    return BranchReport if is_branch else LeafReport
