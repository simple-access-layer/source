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
from sal.core.object import BranchReport, Branch


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
def branch():

    return Branch('A Branch')

@pytest.fixture
def branch_response(branch, server_response):

    """
    JSON representing a Branch
    """

    server_response.json.return_value = serialise(branch)
    return server_response


@pytest.fixture
def auth_header(token):

    return {'Authorization': 'Bearer 78945JHKJFSJDFKH7897wej8UIOJKhuwiofdSDHk'}


@pytest.mark.parametrize('path, revision, verify_https_cert',
                         [('/node', 0, False),
                          ('/node:head', 0, True),
                          ('/node:10', 10, False)])
def test_list(host, patched_client, timestamp, branch_report_response,
              path, revision, verify_https_cert):

    """
    Parametrization includes the three different ways of passing a revision

    GIVEN
        A server with data nodes
    WHEN
        A client performs a list operation on a path
    THEN
        The client returns a BranchReport
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


@pytest.mark.parametrize('path, revision, verify_https_cert, summary',
                         [('/node', 0, False, True),
                          ('/node:head', 0, True, False),
                          ('/node:10', 10, False, False)])
def test_get(host, patched_client, timestamp, branch_response, path, revision,
             verify_https_cert, summary):

    """
    GIVEN
        A server with data nodes
    WHEN
        A client performs a get operation
    THEN
        The client returns either a Branch or a DataObject
    """

    patched_client.verify_https_cert = verify_https_cert
    obj = 'summary' if summary else 'full'

    with patch('sal.client.main.requests.get',
               return_value=branch_response) as br:
        b = patched_client.get(path, summary=summary)

        br.assert_called_with('{0}/data/node?object={1}&revision={2}'.format(
            host, obj, revision), verify=verify_https_cert)
        assert isinstance(b, Branch)


@pytest.mark.parametrize('path, verify_https_cert',
                         [('/node', False),
                          ('/node:head', True),
                          ('/node:0', False)])
def test_put(host, patched_client, no_content_response, branch, path,
             verify_https_cert):

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a put operation
    THEN
        A POST request for the specified path with a serialised Branch object
        is sent to the host
    """

    patched_client.verify_https_cert = verify_https_cert

    with patch('sal.client.main.requests.post',
               return_value=no_content_response) as r:
        patched_client.put(path, branch)

        r.assert_called_with('{0}/data/node'.format(host),
                             json=serialise(branch),
                             verify=verify_https_cert)


@pytest.mark.xfail(reason='SALCLient incorrectly sets source path',
                   raises=AssertionError)
@pytest.mark.parametrize('target, source, source_revision, verify_https_cert',
                         [('/node', '/another/node', 0, False),
                          ('/node:head', '/another/node:head', 0, True),
                          ('/node', '/another/node:15', 15, True)])
def test_copy(host, patched_client, no_content_response, target, source,
              source_revision, verify_https_cert):

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a copy operation from a source to a target
    THEN
        A POST request including a source and source revision is sent to the
        host
    """

    patched_client.verify_https_cert = verify_https_cert

    with patch('sal.client.main.requests.post',
               return_value=no_content_response) as r:
        patched_client.copy(target, source)

        r.assert_called_with(
            '{0}/data/node?source=/another/node&source_revision={1}'.format(
                host,
                source_revision),
            json=None,
            verify=verify_https_cert)


@pytest.mark.parametrize('path, verify_https_cert',
                         [('/node', False),
                          ('/node:head', True),
                          ('/node:0', False)])
def test_delete(host, patched_client, no_content_response, path,
                verify_https_cert):

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a delete operation
    THEN
        A DELETE request for the specified path is sent to the host 
    """

    patched_client.verify_https_cert = verify_https_cert

    with patch('sal.client.main.requests.delete',
               return_value=no_content_response) as r:
        patched_client.delete(path)

        r.assert_called_with('{0}/data/node'.format(host),
                             verify=verify_https_cert)


@pytest.mark.parametrize('op, requests_method, response',
                         [('list', 'get', 'branch_report_response'),
                          ('get', 'get', 'branch_response'),
                          ('put', 'post', 'no_content_response'),
                          ('copy', 'post', 'no_content_response'),
                          ('delete', 'delete', 'no_content_response')])
def test_op_with_auth(host, patched_client_with_auth, token, auth_header, op,
                      requests_method, response, request):

    """
    .. note::
        requests_method is the str of the requests method which will be called
        in performing the operation. It is not related to the pytest special
        fixture 'request', which is used to dynamically get the correct
        response fixture for the parametrization.

    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs an operation
    THEN
        The token is included in a request header
    """

    patched_client_with_auth.auth_token = token
    patched_client_with_auth.verify_https_cert = True
    args = ['/a/path']
    # Inelegant way to add put content and copy source path
    if op == 'put':
        args.append(request.getfixturevalue('branch'))
    elif op == 'copy':
        args.append('/another/path')

    with patch('sal.client.main.requests.{}'.format(requests_method),
               return_value=request.getfixturevalue(response)) as r:
        getattr(patched_client_with_auth, op)(*args)

        # Headers are part of **kwargs, so are always the last arg 
        headers = r.call_args[-1]['headers']
        assert auth_header == headers
