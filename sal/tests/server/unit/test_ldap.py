"""
LDAP Authenticator is too tightly coupled to ldap3 to test

.. note::
    This tests are far from ideal: they are highly implementation dependent, as
    a result of authenticate being strongly coupled with ldap3

"""

from unittest.mock import Mock, patch

import pytest

from sal.server.main import SALServer, PersistenceProvider
from sal.server.providers import ldap

@pytest.fixture
def mock_persistence_provider():
    pp = Mock(spec_set=PersistenceProvider)
    # pp.list.return_value = 5
    # pp.list = lambda path, group=None: {'path':path, "group":group}
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

def mock_init(self, server, uname, pword, **kwargs):
    self.sv = server
    self.un = uname
    self.pw = pword
    self.vn = kwargs['version']
    self.au = kwargs['authentication']

mock_bind = lambda self : True if (self.sv == 'host.server' and
    self.un == 'uid=user,ou=users,dc=ldap.test' and
    self.pw == 'pass' and
    self.vn == 3 and
    self.au == 'SIMPLE') else False 

def apply_ldap_patches(func):
    """
    Equivalent to appling __init__ patch, then bind patch, then Server patch
    """

    return patch.object(ldap.ldap3, 'Server', str)(
        patch.object(ldap.ldap3.Connection, 'bind', mock_bind)(
            patch.object(ldap.ldap3.Connection, '__init__', mock_init)(func)
        )
    )


@apply_ldap_patches
def test_ldap_authenticate_valid_credentials(mock_server):

    """
    GIVEN
        An LDAP server
    WHEN
        Valid credentials are provided to the LDAP server
    THEN
        The credentials are authenticated
    """

    ldap_auth = ldap.LDAPAuthenticator('host.server',
                                       'ou=users,dc=ldap.test',
                                       'uid')
    assert ldap_auth.authenticate('user', 'pass')


@apply_ldap_patches
def test_ldap_authenticate_invalid_credentials(mock_server):

    """
    GIVEN
        An LDAP server
    WHEN
        Invalid credentials are provided to the LDAP server
    THEN
        The credentials are not authenticated
    """

    ldap_auth = ldap.LDAPAuthenticator('host.server',
                                       'ou=users,dc=ldap.test',
                                       'uid')
    assert not ldap_auth.authenticate('invalid_user', 'pass')
