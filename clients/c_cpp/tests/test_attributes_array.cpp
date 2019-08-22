#include "catch.h"
#include "sal.h"
#include <cfloat>

using namespace std;

/*
Data Object Attributes - Array, Signed Integers
*/

TEST_CASE("Data object Int8 array attribute.", "[sal::object::Int8]") {

    SECTION("Initialise without value.") {
        sal::object::Int8 v;

        REQUIRE(v.value == 0);
        REQUIRE(v.type == "int8");
    }

    SECTION("Initialise with array.") {
        sal::object::Int8 v(-89);

        REQUIRE(v.value == -89);
        REQUIRE(v.type == "int8");
    }

//    SECTION("Modify value.") {
//        sal::object::Int8 v;
//
//        v.value = 67;
//        REQUIRE(v.value == 67);
//
//        v.value = -128;
//        REQUIRE(v.value == -128);
//
//        v.value = 127;
//        REQUIRE(v.value == 127);
//    }
//
//    SECTION("Encode as JSON.") {
//
//        sal::object::Int8 v(-42);
//        Poco::JSON::Object::Ptr obj = v.encode();
//
//        REQUIRE(obj->get("value").convert<int8_t>() == -42);
//        REQUIRE(obj->get("type").convert<string>() == "int8");
//    }
}
