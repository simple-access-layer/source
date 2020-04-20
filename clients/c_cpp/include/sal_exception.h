#pragma once

#include <stdexcept>

namespace sal
{

    // using namespace std;

    /// The exception namespace is part of sal_core module
    namespace exception
    {
        /// Refactored: what() must be virtual function override public function
        /** This general error class can be constructed with message,
           Any exception does not fit SALException derived class should use this.
        */
        class SALException : public std::exception
        {
        protected:
            /** Error message.
             */
            std::string msg;

        public:
            SALException(const std::string& _msg)
                    : msg(_msg)
            {
            }
            SALException()
                    : msg("An error happen in SAL")
            {
            }
            virtual const char* what() const noexcept override // throw() == noexcept
            {
                return msg.c_str();
            };
        };

        class InvalidPath : public SALException
        {
        public:
            using SALException::SALException;
            virtual const char* what() const noexcept override
            {
                return "Path does not conform to path specification.";
            };
        };

        class NodeNotFound : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "The supplied path does not point to a valid node.";
            };
        };

        class UnsupportedOperation : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "Operation is not supported.";
            };
        };

        class InvalidRequest : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "The request sent to the server could not be handled.";
            };
        };

        class AuthenticationFailed : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "Valid authorisation credentials were not supplied";
            };
        };

        class PermissionDenied : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "The user does not have permission to perform this operation.";
            };
        };

        class InternalError : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "An error occurred affecting server operation. Please contact your administrator.";
            };
        };

        class InvalidResponse : public SALException
        {
        public:
            virtual const char* what() const noexcept override
            {
                return "The response sent by the server could not be interpreted.";
            };
        };
    }; // namespace exception
} // namespace sal