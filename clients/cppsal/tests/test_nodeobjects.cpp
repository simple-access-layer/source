/*
These tests use a the Catch2 header only c++ test framework:
https://github.com/catchorg/Catch2
*/

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "sal_node.h"
//#include "sal_path.h"
#include "test_sal.h"

using namespace std;
using namespace sal::node;


/// those links only work within jet intranet, without authentication
/// those links works also in webbrowser, recommend firefox
auto branch_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn";
auto leaf_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla";


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

/// https://sal.jet.uk/data/pulse/latest?object=full
