"""
Tests operations involving the data tree

.. note::
    The health check suppression which is applied to some test functions is
    required so that hypothesis plays nicely with pytest: Hypothesis has the
    issue that each example on a test function isn't treated as a separate
    event for any fixtures (i.e., the fixtures aren't rebuilt for each
    example), so errors if the fixture has function scope

.. note::
    Unlike the other SAL operations, there are no tests for DELETE (i.e.
    DataTree.delete) as there is no real logic in the interface to test. Any
    tests relating to DELETE would have to be performed on any concrete
    implementations of the SAL server.
"""

from unittest.mock import Mock, patch

import pytest
from hypothesis import given, settings, strategies as st, HealthCheck
from werkzeug.exceptions import BadRequest

from sal.server.resource.data import DataTree, Branch, DataObject
from sal.core.exception import InvalidRequest


# Instantiate some hypothesis strategies that can be used across multiple tests
# This saves on creating an identical strategies 
generic_content = st.dictionaries(
    keys=st.text(),
    values=st.floats() | st.text() | st.booleans(),
    min_size=0, max_size=3)
valid_revisions = st.integers(min_value=0)
invalid_revisions = st.integers(max_value=-1)

@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(content=generic_content,
       revision=valid_revisions)
def test_data_tree_get_report(content, revision, server,
                              mock_persistence_provider):

    """
    Tests that a DataTree correctly calls a PersistenceProvider for a report 

    GIVEN
        A request with a URL path
        AND a revision query where the revision=non-negative int
        AND no object query
    WHEN
        The DataTree gets from the path
    THEN
        A report of the path is retrieved (from the PersistenceProvider)
    """

    path = 'this/is/the/path'
    data = {'revision':revision}
    
    expected = {**{'request':{'url':'http://localhost/' + path}}, **content}
    call_args = '/{}:{}'.format(path, revision)
    
    with patch('sal.server.resource.data.serialise', return_value=content):
        _check_request(server, 'get', path, expected, data=data)

    mock_persistence_provider.list.assert_called_with(call_args)
    # mock_persistence_provider must be reset, as it maintains state between
    # hypothesis examples. Reset will ensure the assert_called_with is valid. 
    mock_persistence_provider.reset_mock()


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(content=generic_content,
       revision=valid_revisions)
def test_data_tree_get_object(content, revision, server,
                              mock_persistence_provider):

    """
    Tests that a DataTree correctly calls a PersistenceProvider for a full
    object 

    GIVEN
        A request with a URL path
        AND a request with a query with revision=non-negative int
        AND a request with a query with object="full" 
    WHEN
        The DataTree gets from the path
    THEN
        A full object from the path is retrieved (from the PersistenceProvider)
    """

    path = 'this/is/the/path'
    data = {'revision':revision, 'object':'full'}

    expected = {**{'request':{'url':'http://localhost/' + path}}, **content}
    call_args = ('/{}:{}'.format(path,revision), False)

    with patch('sal.server.resource.data.serialise', return_value=content):
        _check_request(server, 'get', path, expected, data=data)

    mock_persistence_provider.get.assert_called_with(*call_args)
    # mock_persistence_provider must be reset, as it maintains state between
    # hypothesis examples. Reset will ensure the assert_called_with is valid. 
    mock_persistence_provider.reset_mock()


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(content=generic_content,
       revision=valid_revisions)
def test_data_tree_get_summary(content, revision, server,
                               mock_persistence_provider):

    """
    Tests that a DataTree correctly calls a PersistenceProvider for a summary
    object 

    GIVEN
        A request with a URL path
        AND a request with a query with revision=non-negative int
        AND a request with a query with object="summary" 
    WHEN
        The DataTree gets from the path
    THEN
        A summary object from the path is retrieved (from the
        PersistenceProvider)
    """

    path = 'this/is/the/path'
    data = {'revision':revision, 'object':'summary'}

    expected = {**{'request':{'url':'http://localhost/' + path}}, **content}
    call_args = ('/{}:{}'.format(path, revision), True)
    
    with patch('sal.server.resource.data.serialise', return_value=content):
        _check_request(server, 'get', path, expected, data=data)
    
    mock_persistence_provider.get.assert_called_with(*call_args)
    # mock_persistence_provider must be reset, as it maintains state between
    # hypothesis examples. Reset will ensure the assert_called_with is valid. 
    mock_persistence_provider.reset_mock()


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(content=generic_content,
       revision=invalid_revisions,
       obj=st.sampled_from(['full', 'summary', None]))
def test_data_tree_get_invalid_revision(content, revision, obj, server,
                                        mock_persistence_provider):

    """
    Tests that a DataTree get operation fails if the path revision is invalid  

    GIVEN
        A request with a URL path
        AND a revision query where the revision=negative int
        AND a request with an object query of full, summary or no object query
    WHEN
        The DataTree gets from the path
    THEN
        A BadRequest exception is raised
    """

    path = 'this/is/the/path'
    data = {'revision':revision}
    if obj:
        data['object'] = obj

    _check_raising_request(server, 'get', path, BadRequest, data=data)


def test_data_tree_get_invalid_object_query(server):

    """
    Tests that an exception is raised when the query argument for object is not
    'full' or 'summary'

    GIVEN
        A request with a URL path
        AND a request with a query with revision=int
        AND a request with a query with an invalid object (not full or summary) 
    WHEN
        The DataTree gets from the path
    THEN
        A BadRequest exception is raised  
    """

    path = 'this/is/the/path'
    data = {'revision':0, 'object':'invalid'}
    
    _check_raising_request(server, 'get', path, BadRequest, data=data)


@pytest.mark.parametrize('json_content, cls',
                         [({'content':'object',
                            'type':'branch',
                            'object':{'description':'A branch'}},
                            Branch),
                          ({'content':'object',
                            'type':'leaf',
                            'object':{'description':{'type':'string',
                                                     'value':'A data object'},
                                      '_type':{'type':'string',
                                                'value':'object'},
                                      '_class':{'type':'string',
                                                'value':'scalar'},
                                      '_group':{'type':'string',
                                                'value':'core'},
                                      '_version':{'type':'uint64',
                                                  'value':1},
                                      'value':{'type':'uint64',
                                               'value':5}}},
                            DataObject)])
def test_data_tree_post_object(server, mock_persistence_provider,
                               json_content, cls):

    """
    Tests that a DataTree correctly puts a branch or data object to a path
    using a PersistenceProvider 

    GIVEN
        A request with a URL path
        AND a request with JSON content describing a branch or data object
    WHEN
        The DataTree posts to the path
    THEN
        The Datatree puts the object onto the PersistenceProvider at the path
        and returns 204
    """

    path = 'this/is/the/path'
    
    expected = ('', 204)

    _check_request(server, 'post', path, expected, json=json_content)

    # Ideally we would use assert_called_with here, however as Branch and
    # DataObject do not implement __eq__, we cannot create an object from
    # content['object'] and test if mock_persistence_provider.put has been
    # called with it 
    # If Branch.__eq__ and DataObject.__eq__ are added (and they are
    # member-wise comparisons), this should be changed
    call_args = mock_persistence_provider.put.call_args
    assert len(call_args) == 2
    assert call_args[0][0] == '/{}'.format(path)
    assert isinstance(call_args[0][1], cls)


# Post does not currently raise an exception if a revision is supplied
@pytest.mark.xfail(reason='Known exception omission in DataTree.post')
@pytest.mark.parametrize('json_content, cls',
                         [({'content':'object',
                            'type':'branch',
                            'object':{'description':'A branch'}},
                            Branch),
                          ({'content':'object',
                            'type':'leaf',
                            'object':{'description':{'type':'string',
                                                     'value':'A data object'},
                                      '_type':{'type':'string',
                                                'value':'object'},
                                      '_class':{'type':'string',
                                                'value':'scalar'},
                                      '_group':{'type':'string',
                                                'value':'core'},
                                      '_version':{'type':'uint64',
                                                  'value':1},
                                      'value':{'type':'uint64',
                                               'value':5}}},
                            DataObject)])
@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(revision=valid_revisions)
def test_data_tree_post_object_with_revision(revision, server,
                                             mock_persistence_provider,
                                             json_content, cls):

    """
    Tests that a DataTree raises an exception if a user tries to post to a path
    with a revision number

    GIVEN
        A request with a URL path
        AND a request with JSON content describing a branch or data object
        AND a request with a revision=non-negative int
    WHEN
        The DataTree posts to the path
    THEN
        A BadRequest exception is raised
    """

    path = 'this/is/the/path'
    json_content['revision'] = revision

    _check_raising_request(server, 'post', path, BadRequest,
                           json=json_content)


# Post currently has a bug which means that the exception is returned rather
# than raised, so this is currently an expected fail
@pytest.mark.xfail(reason='Known bug in DataTree.post')
@pytest.mark.parametrize('json_content',
                         [{'content':'report',
                           'type':'branch',
                           'object':{'description':'A branch report',
                                     'children':{'leaves':[],
                                     'branches':[]},
                           'timestamp':'2021-01-01T11:20:00.000000',
                           'revision':{'latest':1,
                                       'current':2,
                                       'modified':[5,]}}},
                          {'content':'report',
                           'type':'leaf',
                           'object':{'description':'A leaf report',
                                     'object':{'class':'someclass',
                                     'group':'somegroup',
                                     'version':1.1},
                           'timestamp':'2021-01-01T11:20:00.000000',
                           'revision':{'latest':1,
                                       'current':2,
                                       'modified':[5,]}}}])
def test_data_tree_post_report(server, json_content):

    """
    Tests that a DataTree does not put a branch report or a leaf report to a
    path 

    GIVEN
        A request with a URL path
        AND a request with JSON content describing a branch report or leaf
        report
    WHEN
        The DataTree posts to the path
    THEN
        An InvalidRequest is raised
    """

    path = 'this/is/the/path'

    _check_raising_request(server, 'post', path, InvalidRequest,
                           json=json_content)


# Post currently has a bug which passes a different path format for the source
# and target paths (target is prefixed with '/', source isn't), so this is
# currently an expected fail
@pytest.mark.xfail(reason='Known bug when copying using DataTree.post')
@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(source_revision=valid_revisions)
def test_data_tree_copy_object(source_revision, server,
                               mock_persistence_provider):

    """
    Tests that a DataTree correctly copies an object from one path to another
    path using a PersistenceProvider 

    GIVEN
        A request with a URL path
        AND a request with a query with a source=path
        AND a request with a query with source_revision=non-negative int
    WHEN
        The DataTree posts to the path
    THEN
        The Datatree copies the content from the PersistenceProvider source
        path to the PersistenceProvider path
    """

    path = 'this/is/the/path'
    source_path = 'this/is/another/path'
    data = {'source':source_path, 'source_revision':source_revision}

    expected = ('', 204)
    call_args = '/{}'.format(path), '/{}:{}'.format(source_path,
                                                    source_revision)

    _check_request(server, 'post', path, expected, data=data)

    mock_persistence_provider.copy.assert_called_with(call_args)


@settings(suppress_health_check=[HealthCheck.function_scoped_fixture])
@given(source_revision=invalid_revisions)
def test_data_tree_copy_invalid_source_revision(source_revision, server,
                                                mock_persistence_provider):

    """
    Tests that a DataTree copy is invalid if the source has a negative source
    revision 

    GIVEN
        A request with a URL path
        AND a request with a query with a source=path
        AND a request with a query with source_revision=negative int
    WHEN
        The DataTree posts to the path
    THEN
        A BadRequest exception is raised
    """

    path = 'this/is/the/path'
    source_path = 'this/is/another/path'
    data = {'source':source_path, 'source_revision':source_revision}
    
    _check_raising_request(server, 'post', path, BadRequest, data=data)


def _check_request(server, http_method, path, expected, **kwargs):

    """
    Makes a request and checks that the return is as expected

    :param server: The sal.server.SALServer used for the test
    :param http_method: The (str) http method being used (e.g. 'get', 'post') 
    :param path: The (str) path of the request
    :param expected: The expected output from the request
    
    **kwargs Can be one or more dict to be passed to the request context e.g.
    `data` or `json`    
    """

    dt = DataTree()    
    with server.test_request_context(path=path, **kwargs):
            out = getattr(dt, http_method)(path) 
    assert out == expected


def _check_raising_request(server, http_method, path, e_cls, **kwargs):

    """
    Function where the combination of parameters is expected to raise a
    BadRequest, which is tested pytest.raises

    :param server: The sal.server.SALServer used for the test
    :param http_method: The (str) http method being used (e.g. 'get', 'post') 
    :param path: The (str) path of the request
    :param e_cls: The class of the Exception which is expected to be raised
    
    **kwargs Can be one or more dict to be passed to the request context e.g.
    `data` or `json`
    """
    
    dt = DataTree()
    with server.test_request_context(path=path,
                                     **kwargs):
        with pytest.raises(e_cls):
            getattr(dt, http_method)(path)