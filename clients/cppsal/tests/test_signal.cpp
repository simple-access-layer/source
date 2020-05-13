#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "sal_signal.h"
#include "test_sal.h"

/// signal leaf, to test get() data object
auto summary_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=summary";
auto full_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=full";

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
            jobj->stringify(cout, 2); // still bug, all data array has values of AAA
            cout << endl;
            // todo:
        }
    }
}