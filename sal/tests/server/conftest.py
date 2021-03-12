"""
Defines the fixtures user in the server unit tests

.. note::
    The vast majority of test functions will require one of the server fixtures
    (e.g. server, server_with_auth_prov) to be a parameter, even if the fixture
    is not used within the function. This is because Flask requires an app to
    be instantiated and the `app_context` to be entered in order for its
    functionality (e.g. requests, Resource) to be used. 
"""

from unittest.mock import Mock, patch

import pytest

from sal.core.object.report import BranchReport, LeafReport
from sal.server.main import (AuthenticationProvider, PersistenceProvider,
                             SALServer)  

@pytest.fixture
def mock_persistence_provider():
    pp = Mock(spec_set=PersistenceProvider)
    return pp 

@pytest.fixture
def mock_authentication_provider():
    ap = Mock(spec_set=AuthenticationProvider)
    ap.authenticate = (
        lambda u, p : True if u == 'username' and p == 'password' else False)
    # Note that ap.required currently mimics AuthenticationProvider.required
    # This should not be relied upon to always be the case!
    ap.required.return_value = True
    return ap 

@pytest.fixture
def server(mock_persistence_provider):

    """
    A SALServer with mocked attributes
    """

    ss = SALServer(mock_persistence_provider)
    ss.config['TESTING'] = True
    with ss.app_context():
        yield ss

@pytest.fixture
def server_with_auth_prov(mock_persistence_provider,
                          mock_authentication_provider):

    """
    A SALServer with a persistence_provider and an authentication_provider
    """

    ss = SALServer(mock_persistence_provider,
                   authentication_provider=mock_authentication_provider,
                   auth_token_secret="SECRET!"*10,
                   auth_token_lifetime=10.,
                   admin_enabled=True,
                   admin_username='admin',
                   admin_password='admin_password')
    ss.config['TESTING'] = True
    with ss.app_context():
        yield ss
