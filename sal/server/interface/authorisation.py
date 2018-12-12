# Role (group) based Access Control List (ACL) permission system
# returns the groups an authenticated user is a member of
# e.g. LDAP directory lookup of group membership

# guest and default group id for access control list based permissions system
ACL_GUEST = 'guest'


class AuthorisationProvider:

    NAME = 'Authorisation Provider'
    VERSION = '0.0.0'

    def validate(self, username, password):
        # TODO: be defined
        pass



