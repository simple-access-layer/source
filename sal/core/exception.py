# base exception
class SALException(Exception):

    message = None

    def __init__(self, message=None, *args, **kwargs):

        # permit users to override default message
        message = message if message else self.message
        super().__init__(message, *args, **kwargs)


# server side exceptions
class InvalidPath(SALException):
    message = 'Path does not conform to path specification.'


class NodeNotFound(SALException):
    message = 'The supplied path does not point to a valid node.'


class UnsupportedOperation(SALException):
    message = 'Operation is not supported.'


class InvalidRequest(SALException):
    message = 'The request sent to the server could not be handled.'


class AuthenticationFailed(SALException):
    message = 'Valid authorisation credentials were not supplied.'


class PermissionDenied(SALException):
    message = 'The user does not have permission to perform this operation.'


class InternalError(SALException):
    message = 'An error occurred affecting server operation. Please contact your administrator.'


# client side exceptions
class InvalidResponse(SALException):
    message = 'The response sent by the server could not be interpreted.'
