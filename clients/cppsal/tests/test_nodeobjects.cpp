/*
These tests use a the Catch2 header only c++ test framework:
https://github.com/catchorg/Catch2
*/

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "sal_node.h"
//#include "sal_path.h"

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
    cout << "res.getStatus() =" << response.getStatus() << std::endl;

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

    SECTION("local encode and decode leaf report")
    {
        Poco::JSON::Object::Ptr leafJson = leaf.encode();
        REQUIRE(bool(leafJson));
        // leafJson->stringify(cout, 2);
        // cout << endl;
        auto obj = leafJson->getObject("object");
        REQUIRE(obj->getValue<uint64_t>("version") == SAL_API_VERSION);
        Leaf::Ptr lp = Leaf::decode(leafJson);
    }

    SECTION("deserialization leaf report from server")
    {
        auto json = get_json(leaf_url);
        // get_json() will fail if not within intranet, but still return a json object
        if (json->has("type"))
        {
            // json->stringify(cout, 2);
            // cout << endl;
            REQUIRE(json->getValue<std::string>("type") == "leaf");
            NodeObject::Ptr node_ptr = Leaf::decode(json->getObject("object"));
            REQUIRE(node_ptr->nodeInfo().version == SAL_API_VERSION);
            Poco::JSON::Object::Ptr jobj = node_ptr->encode();
            // jobj->stringify(cout, 2);
            // cout << endl;
        }
    }
    // tear down
}


TEST_CASE("Tree-node data organization", "[sal::core::Branch]")
{
    // setup
    NodeInfo nInfo{"node", "core", SAL_API_VERSION, "test_branch"};
    Branch branch(nInfo, "test branch node");

    SECTION("Node info test")
    {
        REQUIRE(nInfo.version == SAL_API_VERSION);
    }

    SECTION("encode and decode branch report")
    {
        Poco::JSON::Object::Ptr json = branch.encode();
        REQUIRE(bool(json));
        // json->stringify(cout, 2);
        auto obj = json->getObject("object");
        REQUIRE(obj->getValue<uint64_t>("version") == SAL_API_VERSION);
        NodeObject::Ptr lp = Branch::decode(json); // test passed
    }

    SECTION("decode branch report from server")
    {
        auto json = get_json(branch_url);
        if (json->has("type")) // get_json() will fail if not within intranet
        {
            REQUIRE(json->getValue<std::string>("type") == "branch");
            NodeObject::Ptr node_ptr = Branch::decode(json->getObject("object"));
            Poco::JSON::Object::Ptr jobj = node_ptr->encode();
            // jobj->stringify(cout, 2);
            // cout << endl;
        }
    }
    // tear down
}

#include "sal_signal.h"
TEST_CASE("Signal data class", "[sal::data::Signal]")
{
    using namespace sal::dataclass;

    /*
        SECTION("local encode and decode leaf report")
        {
            Signal s;
            Poco::JSON::Object::Ptr leafJson = s.encode();
            REQUIRE(bool(leafJson));
            // leafJson->stringify(cout, 2);
            // cout << endl;
            auto obj = leafJson->getObject("object");
            REQUIRE(obj->getValue<uint64_t>("version") == SAL_API_VERSION);
            Leaf::Ptr lp = Leaf::decode(leafJson);
        }
  */
    SECTION("decode signal summary from server")
    {
        auto json = get_json(summary_url);
        if (json->has("type")) // get_json() will fail if not within intranet
        {
            // json->stringify(cout, 2);
            // cout << endl;
            REQUIRE(json->getValue<std::string>("type") == "leaf");
            Signal<float>::Ptr signal_ptr = Signal<float>::decode(json->getObject("object"));
            REQUIRE(bool(signal_ptr));
            REQUIRE(signal_ptr->is_summary());

            // encode_summary() for some classes are not implemented yet, low priority
            // Poco::JSON::Object::Ptr jobj = signal_ptr->encode();
            // jobj->stringify(cout, 2);
            // cout << endl;
        }
    }

    SECTION("decode signal full object from server")
    {
        auto json = get_json(full_url);
        if (json->has("object")) // get_json() return empty json if not within intranet
        {
            // json->stringify(cout, 2);
            // cout << endl;
            REQUIRE(json->getValue<std::string>("type") == "leaf");
            Signal<float>::Ptr signal_ptr = Signal<float>::decode(json->getObject("object"));
            REQUIRE(bool(signal_ptr));
            REQUIRE(!signal_ptr->is_summary());

            Poco::JSON::Object::Ptr jobj = signal_ptr->encode();
            REQUIRE(bool(jobj));
            jobj->stringify(cout, 2);
            cout << endl;
        }
    }
}