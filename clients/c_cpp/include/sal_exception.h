#pragma once

#include <stdexcept>

namespace sal
{

    using namespace std;

    /// The exception namespace is part of sal_core module
    namespace exception
    {

        class SALException : public exception
        {
            const char* what() const throw()
            {
                return "An error has occurred with the SAL client.";
            };
        };

        class InvalidPath : public SALException
        {
            const char* what() const throw()
            {
                return "Path does not conform to path specification.";
            };
        };

        class NodeNotFound : public SALException
        {
            const char* what() const throw()
            {
                return "The supplied path does not point to a valid node.";
            };
        };

        class UnsupportedOperation : public SALException
        {
            const char* what() const throw()
            {
                return "Operation is not supported.";
            };
        };

        class InvalidRequest : public SALException
        {
            const char* what() const throw()
            {
                return "The request sent to the server could not be handled.";
            };
        };

        class AuthenticationFailed : public SALException
        {
            const char* what() const throw()
            {
                return "Valid authorisation credentials were not supplied";
            };
        };

        class PermissionDenied : public SALException
        {
            const char* what() const throw()
            {
                return "The user does not have permission to perform this operation.";
            };
        };

        class InternalError : public SALException
        {
            const char* what() const throw()
            {
                return "An error occurred affecting server operation. Please contact your administrator.";
            };
        };

        class InvalidResponse : public SALException
        {
            const char* what() const throw()
            {
                return "The response sent by the server could not be interpreted.";
            };
        };
    }; // namespace exception
} // namespace sal