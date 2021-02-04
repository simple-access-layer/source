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


@given(data=st.dictionaries(keys=st.text(),
                            values=st.floats() | st.text() | st.booleans(),
                            min_size=0, max_size=3),
       revision=st.integers(min_value=0))
# @example(data={},
#          revision=None)
def test_data_tree(data, revision, mock_server, mock_persistence_provider):

    path = 'this/is/the/path'
    dt = DataTree()
    with mock_server.test_request_context('/how/about/this'):
        with patch('sal.server.resource.data.serialise',
                   return_value=data):
            a = dt.get(path)
    print(path)
    print(mock_persistence_provider.__dict__)
    mock_persistence_provider.list.assert_called_with('/{}:{}'.format(path,
                                                                      revision)
                                                     )
    # assert a == {**{'request':{'url':'https://localhost/' + path}}, **data}