from werkzeug.exceptions import HTTPException

# base exception
class SALException(Exception):

    message = None

    def __init__(self, message=None, *args, **kwargs):

        # permit users to override default message
        message = message if message else self.message
        super().__init__(message, *args, **kwargs)

class SALHTTPException(HTTPException, SALException):

    """
    Base SAL specific HTTP exception
    
    Inherits from ``HTTPException`` first as we want it to use that `__init__`

    Inheriting from both means it will be caught if either ``HTTPException`` or
    ``SALException``

    Inheriting from ``HTTPException`` means it will have an associated status
    code (`code`) which can be handled by Flask
    """

    pass

# server side exceptions
class InvalidPath(SALHTTPException):
    code = 404
    description = 'Path does not conform to path specification.'


class NodeNotFound(SALException):
    code = 404
    description = 'The supplied path does not point to a valid node.'


class UnsupportedOperation(SALException):
    code = 500
    description = 'Operation is not supported.'


class InvalidRequest(SALException):
    code = 400
    description = 'The request sent to the server could not be handled.'


class AuthenticationFailed(SALException):
    code = 401
    description = 'Valid authorisation credentials were not supplied.'


class PermissionDenied(SALException):
    code = 403
    description = 'The user does not have permission to perform this operation.'


class InternalError(SALException):
    code = 500
    description = 'An error occurred affecting server operation. Please contact your administrator.'


# client side exceptions
class InvalidResponse(SALException):
    message = 'The response sent by the server could not be interpreted.'
