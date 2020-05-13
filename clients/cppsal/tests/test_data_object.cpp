#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "sal_data_object.h"
#include "test_sal.h"
#include "test_signal.cpp"

using namespace sal::dataclass;

auto attr_url = "https://sal.jet.uk/data/pulse/latest?object=full";
auto data_object_full_url = "https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=full";

TEST_CASE("DataObject test", "[sal::dataobject::DataObject]")
{
    // todo: construct a DataObject from a scalar attribute

    SECTION("Test Attribute to DataObject")
    {
        auto json = get_json(attr_url);
        if (json->has("object")) // get_json() return empty json if not within intranet
        {
            auto d = DataObject::decode(json); // failed
            REQUIRE(bool(d));
            auto a = d->cast<sal::object::UInt64>();
            REQUIRE(a);
        }
    }

    SECTION("Test Signal to DataObject")
    {
        auto json = get_json(data_object_full_url);
        if (json->has("object")) // get_json() return empty json if not within intranet
        {
            auto d = DataObject::decode(json);
            REQUIRE(bool(d)); // passed, needs further unit test
            // auto a = d->cast<sal::dataclass::Signal>();
            // REQUIRE(a);
        }
    }
}