from sal.server.interface import AuthenticationProvider, AuthorisationProvider

"""
LDAP Authentication and Authorisation Providers

Requires ldap3 package.
"""

try:
    import ldap3
except ImportError:
    raise ImportError('Requires ldap3 package version >= 2.5.')


class LDAPAuthenticator(AuthenticationProvider):

    NAME = 'LDAP Authenticator'
    VERSION = '1.0.0'

    def __init__(self, server, base_dn, user_attribute):

        # validate contents to prevent arbitrary execution of LDAP call
        self.server = server
        self.base_dn = base_dn
        self.user_attribute = user_attribute

    def authenticate(self, username, password):

        # sanitise username to prevent injection and construct dn filter
        username = ldap3.utils.dn.escape_rdn(username)
        user_dn = '{}={},{}'.format(self.user_attribute, username, self.base_dn)

        # try to connect to the ldap server with the users username/password
        server = ldap3.Server(self.server)
        connection = ldap3.Connection(server, user_dn, password, version=3, authentication=ldap3.SIMPLE)
        return connection.bind()


class LDAPAuthoriser(AuthorisationProvider):

    NAME = 'LDAP Authoriser'
    VERSION = '1.0.0'

    # todo: implement me
    pass