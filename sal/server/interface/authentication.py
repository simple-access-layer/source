# Authentication handler checks the client is who they claim to be
# returns username for use by authorisation system in deciding access to resources
# e.g. LDAP or Kerboros authentication


class AuthenticationProvider:

    NAME = 'Authentication Provider'
    VERSION = '0.0.0'

    def authenticate(self, username, password):
        """
        Authenticates the user using the supplied password.

        :param username: Username string.
        :param password: Password string.
        :return: True / False
        """
        raise NotImplementedError('The authenticate method must be implemented via sub-classing.')

    def required(self):
        """
        Specifies if authentication is required.

        Where authentication is dependent on the request origin or other
        request property, this method may be implemented to specify if
        authentication is required.

        The default implementation returns True for all requests - all incoming
        requests require authentication. Override this method to implement an
        alternative behaviour.

        :return:
        """
        return True