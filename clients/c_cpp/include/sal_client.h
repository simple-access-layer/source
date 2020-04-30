#pragma once

#include "sal_data.h"
#include "sal_node.h"
#include "sal_signal.h"

#define USE_POCO_LOGGER 1
#if USE_POCO_LOGGER
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Logger.h"

#endif

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/UTF8String.h"

namespace sal
{
    using namespace std; // todo: remove this later
    using namespace exception;


    uint32_t SUPPORTED_API_VERSION = 2;

    /// Todo: authentication is not implemented

    /** client implementation class coresponds to Python `sal.client.SALClient` type
     * */
    class Client
    {
    private:
        bool m_verifying_https_cert;
        std::string m_auth_token;
        std::string m_log_filename;

    public:
        Client(const string host)
                : Client(host, true){};
        Client(const string host, const bool verify_https_cert)
        {

            // authentication attributes
            this->m_auth_required = false;
            this->m_auth_token = "";
            this->m_verifying_https_cert = verify_https_cert;

            // set and inspect host
            this->set_host(host);
            this->m_log_filename = "sal_client.log";
        };

        virtual ~Client(){};

#if USE_POCO_LOGGER
        Poco::Logger& logger() const
        {
            return Poco::Logger::get(m_log_filename);
        }
#endif
        const string get_host() const
        {
            return this->m_host_uri.toString();
        };
        void set_host(const string host)
        {

            bool auth_required;
            Poco::URI uri = Poco::URI(host);

            try
            {
                // connect to server and request server properties
                Poco::JSON::Object::Ptr json = this->make_get_request(uri);

                // check server response is valid and store relevant server settings
                string name = json->getObject("service")->getValue<string>("name");
                if (name != "Simple Access Layer (SAL) Server")
                {
                    std::string msg = "Could not identify a SAL server on the specified host:" + host;
                    throw exception::SALException(msg.c_str());
                }
                Poco::JSON::Object::Ptr api = json->getObject("api");
                uint32_t version = api->getValue<uint32_t>("version");
                if (version != SUPPORTED_API_VERSION)
                {
                    std::string msg = "The server is using a newer API than the client please update your client API";
                    throw exception::SALException(msg.c_str());
                }

                auth_required = api->getValue<bool>("requires_auth");
            }
            catch (Poco::JSON::JSONException e)
            {
                std::string msg = std::string("Failed to parse server JSON response:") + e.what();
                throw exception::SALException(msg.c_str());
            }
            catch (std::exception e)
            {
                std::string msg = std::string("Failed to set host, please check and try again \n") + e.what();
                throw exception::SALException(msg.c_str());
            }

            // all good, update host state
            this->m_auth_required = auth_required;
            this->m_host_uri = uri;
            this->m_auth_uri = Poco::URI(uri, "auth/");
            this->m_data_uri = Poco::URI(uri, "data/");
        };

        const bool is_auth_required() const
        {
            return this->m_auth_required;
        };

        bool verifying_https_cert() const
        {
            return m_verifying_https_cert;
        }


        /*
        bool& verifying_https_cert()
        {
            return m_verifying_https_cert;
        }
        std::string auth_token() const
        {
            return m_auth_token;
        }
        */
        void authenticate(const string user, const string password){
            /* server side has not yet completed
            try {
                // todo: connect to server and request server properties
            }
            */

        };

        /// explore the node path and deserialize into NodeObject
        node::NodeObject::Ptr list(const string sal_path) const
        {
            // TODO: unit test
            node::NodeObject::Ptr node_obj = nullptr;
            Poco::URI node_uri(this->m_data_uri, sal_path);
            auto jObj = make_get_request(node_uri);
            if (jObj)
                return node::decode(jObj);
            return node_obj;
        };

        /// deserialized into Signal or Dictionary data attribute/object
        /// `obj = sal.get(path, summary=False)`
        object::Attribute::Ptr get(const string path, bool summary = false) const
        {
            // TODO: convert sal path to uri, URI validation
            string sal_path = path;

            std::string query = "?object=full";
            if (summary)
                std::string query = "?object=summary";
            Poco::URI node_uri(this->m_data_uri, sal_path + query);
#if USE_POCO_LOGGER
            logger().debug(node_uri.toString());
#endif
            Poco::JSON::Object::Ptr obj = this->make_get_request(node_uri);

            obj->stringify(cout, 2); // debugging output

            return object::decode(obj);
        };

        // if data type has been knowned from list(), directly cast to type

        /// push modified data to server,
        /// also needs to check permission
        void put(const string path, const node::NodeObject obj) const
        {
            // todo: first of all, make/register an explorable node  on the back end
            // then push full data to the backend and store
            throw SALException("this method is currently not supported by server backend.");
        };

        /// this works like copy file, providing the source path and target path
        /// todo: very weird API, switch T and S the parameter position
        void copy(const string target, const string source) const
        {
            // todo: send request, and check http STATUS
            throw SALException("this method is currently not supported by server backend.");
        }

        /// todo: user must have the permission to delete any node object
        void del(const string path) const
        {
            // todo: send request, and check http STATUS
            throw SALException("not implemented yet");
        };

    protected:
        bool m_auth_required;
        Poco::URI m_host_uri;
        Poco::URI m_auth_uri;
        Poco::URI m_data_uri;

        const Poco::SharedPtr<Poco::Net::HTTPClientSession> open_session(const Poco::URI uri) const
        {

            using namespace Poco::Net;

            // http or https?
            std::string scheme = uri.getScheme();
            std::string userInfo = uri.getUserInfo();
            if (scheme == "http")
            {
                if (userInfo.size())
                {
                    throw SALException("Username and password without encryption (HTTPS) is forbidden");
                }
                std::cerr << "Use http non-encryption connection for internal test only" << std::endl;
                return new HTTPClientSession(uri.getHost(), uri.getPort());
            }
            else if (scheme == "https")
            {
                if (this->m_verifying_https_cert)
                {
                    // https session, checking certificate is valid
                    return new HTTPSClientSession(uri.getHost(), uri.getPort());
                }
                else
                {
                    // https session, ignoring invalid certificates
                    return new HTTPSClientSession(uri.getHost(), uri.getPort(),
                                                  new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE));
                }
            }
            else
                throw Poco::UnknownURISchemeException(uri.toString().c_str());
        };

        /// return nullptr if failed, embrace all code in try block
        Poco::JSON::Object::Ptr make_get_request(const Poco::URI uri) const
        {

            using namespace Poco;
            using namespace Poco::Net;
            using namespace Poco::JSON;

            HTTPResponse response;
            Poco::JSON::Parser parser;
            string json;

            // open http or https session
            SharedPtr<HTTPClientSession> session = this->open_session(uri);

            // redirect empty path to root
            string path = uri.getPathEtc();
            if (path.empty())
                path = "/";

            // make request
            HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
            session->sendRequest(request);
            // todo: handle errors: NodeNotFound
            auto& istream = session->receiveResponse(response);
            if (!check_response_status(response))
                return nullptr;
            StreamCopier::copyToString(istream, json);

            // decode json
            // todo: handle exceptions: parsing error, return nullptr if failed?
            return parser.parse(json).extract<Poco::JSON::Object::Ptr>();
        };

        /// handle different situation
        bool check_response_status(const Poco::Net::HTTPResponse& res) const
        {
            using namespace Poco::Net;
            const auto status = res.getStatus();
            if (status == HTTPResponse::HTTPStatus::HTTP_OK)
                return true;
#if USE_POCO_LOGGER
            else if (status == HTTPResponse::HTTPStatus::HTTP_FORBIDDEN)
            {
                logger().error("permission error " + res.getReason());
            }
            else if (status == HTTPResponse::HTTPStatus::HTTP_OK)
            {
                logger().error("server is not found, please check url" + res.getReason());
            }
            else
            {
                logger().error("HTTP STATUS: " + std::to_string(status) + "\n reason: " + res.getReason());
            }
#endif
        }

        Poco::JSON::Object::Ptr make_post_request(const Poco::URI uri) const
        {
            throw UnsupportedOperation("RESTFUL post operation has not been implemented");
            return nullptr;
        }

        Poco::JSON::Object::Ptr make_del_request(const Poco::URI uri) const
        {
            throw UnsupportedOperation("RESTFUL post operation has not been implemented");
            return nullptr;
        }

        //            string serialise(node::NodeObject obj);
        //            node::NodeObject deserialise(string json);
    };
} // namespace sal