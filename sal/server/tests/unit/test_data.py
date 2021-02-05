# Test get data
# Test post data
# Test delete data

# Test server root info path
# Test auth path
# Test data path
# Test auth validation
# Test admin validation

from unittest.mock import Mock, patch

from hypothesis import example, given, strategies as st
import pytest

from sal.server.main import (SALServer, PersistenceProvider, DataTree, DataObject)

@pytest.fixture(scope='module')
def mock_persistence_provider():
    pp = Mock(spec_set=PersistenceProvider)
    # pp.list.return_value = 5
    # pp.list = lambda path, group=None: {'path':path, "group":group}
    return pp 

# Module scope is required so that pytest plays nicely with hypothesis
# hypothesis has the issue that each example on a test function isn't treated
# as a separate event for any fixtures (i.e., the fixtures aren't rebuilt for
# each example), so errors if the fixture has function scope
@pytest.fixture(scope='module')
def mock_server(mock_persistence_provider):
    """
    A SALServer with mocked attributes 
    """

    sal_server = SALServer(mock_persistence_provider)
    sal_server.config['TESTING'] = True
    with sal_server.app_context():
        yield sal_server


# def test_temp(mock_server):
#     perp = mock_server.config['SAL']['PERSISTENCE']
#     with patch.object(perp, 'list', return_value=5):
#         assert perp.list() == 5
#         perp.list.assert_called_with()
#     import pdb; pdb.set_trace()
#     assert isinstance(mock_server, SALServer)

@given(content=st.dictionaries(keys=st.text(),
                               values=st.floats() | st.text() | st.booleans(),
                               min_size=0, max_size=3),
       revision=st.integers(min_value=0))
def test_data_tree_get_report(content, revision, mock_server,
                             mock_persistence_provider):

    """
    Tests that a DataTree correctly calls a PersistenceProvider for a report 

    GIVEN
        A request with a URL path
        AND a revision query where the revision is an int
        AND No object query
    WHEN
        The DataTree gets from the path
    THEN
        A report of the path is retrieved (from the PersistenceProvider)
    """

    path = 'this/is/the/path'
    expected = {**{'request':{'url':'http://localhost/' + path}}, **content}
    dt = DataTree()
    
    with mock_server.test_request_context(path=path,
                                          data={'revision':revision}):
        with patch('sal.server.resource.data.serialise',
                   return_value=content):
            list_out = dt.get(path)
    mock_persistence_provider.list.assert_called_with('/{}:{}'.format(path,
                                                                      revision)
                                                     )
    # mock_persistence_provider must be reset, as it maintains state between
    # hypothesis examples. Reset will ensure the assert_called_with is valid. 
    mock_persistence_provider.reset_mock()
    assert list_out == expected


def test_data_tree_get_object():

    """
    Tests that a DataTree correctly calls a PersistenceProvider for an object 

    GIVEN
        A request with a URL path
        AND a request with a query with revision=int
        AND a request with a query with object="full" or object="summary" 
    WHEN
        The DataTree gets from the path
    THEN
        A object (full or summary) from the path is retrieved (from the
        PersistenceProvider)
    """

    pass


def test_data_tree_post_object():

    """
    Tests that a DataTree correctly puts an object to a path using a
    PersistenceProvider 

    GIVEN
        A request with a URL path
        AND a request with JSON content
    WHEN
        The DataTree posts to the path
    THEN
        The Datatree puts the JSON content onto the PersistenceProvider at the
        path
    """

    pass


def test_data_tree_copy_object():

    """
    Tests that a DataTree correctly copies an object from one path to another
    path using a PersistenceProvider 

    GIVEN
        A request with a URL path
        AND a request with a query with a source=path
        AND a request with a query with source_revision=int
    WHEN
        The DataTree posts to the path
    THEN
        The Datatree copies the content from the PersistenceProvider source
        path to the PersistenceProvider path
    """

    pass    
