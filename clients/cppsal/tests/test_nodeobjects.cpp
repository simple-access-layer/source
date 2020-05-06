/*
These tests use a the Catch2 header only c++ test framework:
https://github.com/catchorg/Catch2
*/

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "sal_node.h"

using namespace std;
using namespace sal::node;

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/UTF8String.h"

Poco::JSON::Object::Ptr get_json(const std::string url)
{
    using namespace Poco;
    using namespace Poco::Net;
    using namespace Poco::JSON;

    HTTPResponse response;
    Poco::JSON::Parser parser;
    string json;
    Poco::URI uri{url};

    // https session, ignoring invalid certificates
    auto session = new HTTPSClientSession(uri.getHost(), uri.getPort(),
                                          new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE));

    HTTPRequest request(HTTPRequest::HTTP_GET, url, HTTPMessage::HTTP_1_1);
    session->sendRequest(request);
    StreamCopier::copyToString(session->receiveResponse(response), json);
    cout << "res.getStatus() =" << response.getStatus();

    // todo: handle exceptions and check response status
    auto j = parser.parse(json).extract<Poco::JSON::Object::Ptr>(); // decode json
    // j->stringify(cout, 2);                                          // debugging output
    return j;
}

/// those links only work within jet intranet, without authentication
/// those links works also in webbrowser
auto branch_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn";
auto leaf_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla";
/// signal leaf, to test get() data object
auto summary_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=summary";
auto full_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=full";


TEST_CASE("Tree-node leaf object", "[sal::core::Leaf]")
{
    // setup
    NodeInfo nInfo{"node", "core", SAL_API_VERSION};
    Leaf leaf(nInfo, "test node", true);

    SECTION("test Leaf object")
    {
        REQUIRE(nInfo.version == SAL_API_VERSION);
    }

    SECTION("serialization leaf report")
    {
        Poco::JSON::Object::Ptr leafJson = leaf.encode();
        REQUIRE(bool(leafJson));
        auto obj = leafJson->getObject("object");
        REQUIRE(obj->getValue<uint64_t>("version") == SAL_API_VERSION);
        Leaf::Ptr lp = Leaf::decode(leafJson);
    }

    SECTION("deserialization leaf report from server")
    {
        auto json = get_json(leaf_url);
        REQUIRE(bool(json));
        if (json)
        {
            NodeObject::Ptr node_ptr = Leaf::decode(json);
            REQUIRE(node_ptr->nodeInfo().version == SAL_API_VERSION);
            Poco::JSON::Object::Ptr jobj = node_ptr->encode();
        }
    }

    // tear down
}

TEST_CASE("Tree-node data organization", "[sal::core::Branch]")
{
    // setup
    NodeInfo nInfo{"node", "core", SAL_API_VERSION};
    Branch branch(nInfo, "test branch node");

    SECTION("Node info test")
    {
        REQUIRE(nInfo.version == SAL_API_VERSION);
    }

    SECTION("serialization branch report")
    {
        Poco::JSON::Object::Ptr json = branch.encode();
        auto obj = json->getObject("object");
        REQUIRE(obj->getValue<uint64_t>("version") == SAL_API_VERSION);
        // Leaf::Ptr lp = Leaf::decode_summary(leafJson);
        Leaf::Ptr lp = Leaf::decode(json);
    }

    SECTION("deserialization branch report from server")
    {
        auto json = get_json(branch_url);
        REQUIRE(bool(json));
        if (json)
        {
            NodeObject::Ptr node_ptr = Branch::decode(json);
            REQUIRE(node_ptr->nodeInfo().version == SAL_API_VERSION);

            Poco::JSON::Object::Ptr jobj = node_ptr->encode();
        }
    }
    // tear down
}