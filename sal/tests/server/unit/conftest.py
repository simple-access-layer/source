from unittest.mock import Mock, patch

import pytest

from sal.server.main import SALServer, PersistenceProvider

@pytest.fixture
def mock_persistence_provider():
    pp = Mock(spec_set=PersistenceProvider)
    return pp 

@pytest.fixture
def mock_server(mock_persistence_provider):
    """
    A SALServer with mocked attributes 
    """

    sal_server = SALServer(mock_persistence_provider)
    sal_server.config['TESTING'] = True
    with sal_server.app_context():
        yield sal_server