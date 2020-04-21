/*
These tests use a the Catch2 header only c++ test framework: https://github.com/catchorg/Catch2
*/

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "sal.h"
#include <cfloat>

using namespace std;

/*
Data Object Attributes - Scalar, Signed Integers
*/

TEST_CASE("Data object Int8 scalar attribute.", "[sal::object::Int8]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Int8 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "int8");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Int8 v(-89);

        REQUIRE(v.value() == -89);
        REQUIRE(v.type_name() == "int8");
    }

    SECTION("Modify value.")
    {
        sal::object::Int8 v;

        v.value() = 67;
        REQUIRE(v.value() == 67);

        v.value() = -128;
        REQUIRE(v.value() == -128);

        v.value() = 127;
        REQUIRE(v.value() == 127);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Int8 v(-42);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<int8_t>() == -42);
        REQUIRE(obj->get("type").convert<string>() == "int8");
    }
}


TEST_CASE("Data object Int16 scalar attribute.", "[sal::object::Int16]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Int16 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "int16");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Int16 v(-6653);

        REQUIRE(v.value() == -6653);
        REQUIRE(v.type_name() == "int16");
    }

    SECTION("Modify value.")
    {
        sal::object::Int16 v;

        v.value() = 6237;
        REQUIRE(v.value() == 6237);

        v.value() = -32768;
        REQUIRE(v.value() == -32768);

        v.value() = 32765;
        REQUIRE(v.value() == 32765);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Int16 v(-783);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<int16_t>() == -783);
        REQUIRE(obj->get("type").convert<string>() == "int16");
    }
}


TEST_CASE("Data object Int32 scalar attribute.", "[sal::object::Int32]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Int32 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "int32");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Int32 v(-1393553);

        REQUIRE(v.value() == -1393553);
        REQUIRE(v.type_name() == "int32");
    }

    SECTION("Modify value.")
    {
        sal::object::Int32 v;

        v.value() = 623007;
        REQUIRE(v.value() == 623007);

        v.value() = -2147483648;
        REQUIRE(v.value() == -2147483648);

        v.value() = 2147483647;
        REQUIRE(v.value() == 2147483647);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Int32 v(-783433);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<int32_t>() == -783433);
        REQUIRE(obj->get("type").convert<string>() == "int32");
    }
}


TEST_CASE("Data object Int64 scalar attribute.", "[sal::object::Int64]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Int64 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "int64");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Int64 v(-1393553466553);

        REQUIRE(v.value() == -1393553466553);
        REQUIRE(v.type_name() == "int64");
    }

    SECTION("Modify value.")
    {
        sal::object::Int64 v;

        v.value() = 6233230466047;
        REQUIRE(v.value() == 6233230466047);

        v.value() = -9223372036854775808;
        REQUIRE(v.value() == -9223372036854775808);

        v.value() = 9223372036854775807;
        REQUIRE(v.value() == 9223372036854775807);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Int64 v(-77375993827738483);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<int64_t>() == -77375993827738483);
        REQUIRE(obj->get("type").convert<string>() == "int64");
    }
}


/*
Data Object Attributes - Scalar, Unsigned Integers
*/

TEST_CASE("Data object UInt8 scalar attribute.", "[sal::object::UInt8]")
{

    SECTION("Initialise without value.")
    {
        sal::object::UInt8 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "uint8");
    }

    SECTION("Initialise with value.")
    {
        sal::object::UInt8 v(89);

        REQUIRE(v.value() == 89);
        REQUIRE(v.type_name() == "uint8");
    }

    SECTION("Modify value.")
    {
        sal::object::UInt8 v;

        v.value() = 67;
        REQUIRE(v.value() == 67);

        v.value() = 0;
        REQUIRE(v.value() == 0);

        v.value() = 255;
        REQUIRE(v.value() == 255);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::UInt8 v(42);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<uint8_t>() == 42);
        REQUIRE(obj->get("type").convert<string>() == "uint8");
    }
}


TEST_CASE("Data object UInt16 scalar attribute.", "[sal::object::UInt16]")
{

    SECTION("Initialise without value.")
    {
        sal::object::UInt16 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "uint16");
    }

    SECTION("Initialise with value.")
    {
        sal::object::UInt16 v(6653);

        REQUIRE(v.value() == 6653);
        REQUIRE(v.type_name() == "uint16");
    }

    SECTION("Modify value.")
    {
        sal::object::UInt16 v;

        v.value() = 6237;
        REQUIRE(v.value() == 6237);

        v.value() = 0;
        REQUIRE(v.value() == 0);

        v.value() = 65535;
        REQUIRE(v.value() == 65535);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::UInt16 v(783);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<uint16_t>() == 783);
        REQUIRE(obj->get("type").convert<string>() == "uint16");
    }
}


TEST_CASE("Data object UInt32 scalar attribute.", "[sal::object::UInt32]")
{

    SECTION("Initialise without value.")
    {
        sal::object::UInt32 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "uint32");
    }

    SECTION("Initialise with value.")
    {
        sal::object::UInt32 v(1393553);

        REQUIRE(v.value() == 1393553);
        REQUIRE(v.type_name() == "uint32");
    }

    SECTION("Modify value.")
    {
        sal::object::UInt32 v;

        v.value() = 623007;
        REQUIRE(v.value() == 623007);

        v.value() = 0;
        REQUIRE(v.value() == 0);

        v.value() = 4294967295;
        REQUIRE(v.value() == 4294967295);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::UInt32 v(783433);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<uint32_t>() == 783433);
        REQUIRE(obj->get("type").convert<string>() == "uint32");
    }
}


TEST_CASE("Data object UInt64 scalar attribute.", "[sal::object::UInt64]")
{

    SECTION("Initialise without value.")
    {
        sal::object::UInt64 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "uint64");
    }

    SECTION("Initialise with value.")
    {
        sal::object::UInt64 v(1393553466553);

        REQUIRE(v.value() == 1393553466553);
        REQUIRE(v.type_name() == "uint64");
    }

    SECTION("Modify value.")
    {
        sal::object::UInt64 v;

        v.value() = 6233230466047;
        REQUIRE(v.value() == 6233230466047);

        v.value() = 0;
        REQUIRE(v.value() == 0);

        v.value() = 18446744073709551615;
        REQUIRE(v.value() == 18446744073709551615);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::UInt64 v(77375993827738483);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<uint64_t>() == 77375993827738483);
        REQUIRE(obj->get("type").convert<string>() == "uint64");
    }
}


/*
Data Object Attributes - Scalar, Floating Point
*/

TEST_CASE("Data object Float32 scalar attribute.", "[sal::object::Float32]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Float32 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "float32");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Float32 v(2.8774e8);

        REQUIRE(v.value() == 2.8774e8);
        REQUIRE(v.type_name() == "float32");
    }

    SECTION("Modify value.")
    {
        sal::object::Float32 v;

        v.value() = -364;
        REQUIRE(v.value() == -364);

        v.value() = FLT_MAX;
        REQUIRE(v.value() == FLT_MAX);

        v.value() = FLT_MIN;
        REQUIRE(v.value() == FLT_MIN);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Float32 v(8.273);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<float>() == 8.273f);
        REQUIRE(obj->get("type").convert<string>() == "float32");
    }
}


TEST_CASE("Data object Float64 scalar attribute.", "[sal::object::Float64]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Float64 v;

        REQUIRE(v.value() == 0);
        REQUIRE(v.type_name() == "float64");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Float64 v(2.875546654e78);

        REQUIRE(v.value() == 2.875546654e78);
        REQUIRE(v.type_name() == "float64");
    }

    SECTION("Modify value.")
    {
        sal::object::Float64 v;

        v.value() = -364;
        REQUIRE(v.value() == -364);

        v.value() = DBL_MAX;
        REQUIRE(v.value() == DBL_MAX);

        v.value() = DBL_MIN;
        REQUIRE(v.value() == DBL_MIN);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Float64 v(8.273);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<double>() == 8.273d);
        REQUIRE(obj->get("type").convert<string>() == "float64");
    }
}

/*
Data Object Attributes - Scalar, String and Bool
*/

TEST_CASE("Data object Bool scalar attribute.", "[sal::object::Bool]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Bool v;

        REQUIRE(v.value() == false);
        REQUIRE(v.type_name() == "bool");
    }

    SECTION("Initialise with value.")
    {
        sal::object::Bool v(true);

        REQUIRE(v.value() == true);
        REQUIRE(v.type_name() == "bool");
    }

    SECTION("Modify value.")
    {
        sal::object::Bool v;

        v.value() = true;
        REQUIRE(v.value() == true);

        v.value() = false;
        REQUIRE(v.value() == false);
    }

    SECTION("Encode as JSON.")
    {

        sal::object::Bool v(true);
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<bool>() == true);
        REQUIRE(obj->get("type").convert<string>() == "bool");

        v.value() = false;
        obj = v.encode();
        REQUIRE(obj->get("value").convert<bool>() == false);
        REQUIRE(obj->get("type").convert<string>() == "bool");
    }
}


TEST_CASE("Data object String scalar attribute.", "[sal::object::String]")
{

    SECTION("Initialise without value.")
    {
        sal::object::String v;

        REQUIRE(v.value() == "");
        REQUIRE(v.type_name() == "string");
    }

    SECTION("Initialise with value.")
    {
        sal::object::String v("hello world!");

        REQUIRE(v.value() == "hello world!");
        REQUIRE(v.type_name() == "string");
    }

    SECTION("Modify value.")
    {
        sal::object::String v;

        v.value() = "Apple";
        REQUIRE(v.value() == "Apple");

        v.value() = "Cart";
        REQUIRE(v.value() == "Cart");
    }

    SECTION("Encode as JSON.")
    {

        sal::object::String v("Encode this string.");
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->get("value").convert<string>() == "Encode this string.");
        REQUIRE(obj->get("type").convert<string>() == "string");
    }
}

/* sal::object::Attribute is abstract, can not been instantiated
 instead a new derived object Null is created as a special value
*/
TEST_CASE("Data object null or empty attribute.", "[sal::object::Attribute]")
{

    SECTION("Initialise without value.")
    {
        sal::object::Null v;

        // REQUIRE(v.value() == nullptr);
        REQUIRE(v.type_name() == "null");
    }

    SECTION("Encode Null object as JSON.")
    {
        sal::object::Null v;
        Poco::JSON::Object::Ptr obj = v.encode();

        REQUIRE(obj->isNull("value"));
        REQUIRE(obj->get("type").convert<string>() == "null");
    }
}


TEST_CASE("Data object summary", "[sal::object::SummaryInterface]")
{
    SECTION("scalar summary")
    {
        sal::object::Int32 v{100};
        Poco::JSON::Object::Ptr obj = v.encode_summary();
        REQUIRE(obj->get("value").convert<int32_t>() == 100);
    }

    SECTION("array summary")
    {
        sal::object::Int8Array v({2, 3});
        Poco::JSON::Object::Ptr obj = v.encode_summary();

        // obj->stringify(cout);
        Poco::Dynamic::Array shape = *(obj->getArray("shape"));
        REQUIRE(shape[0] == 2);
        REQUIRE(shape[1] == 3);
    }
}

TEST_CASE("Data object Int8 array attribute.", "[sal::object::Int8Array]")
{

    // SECTION("Initialise without value.")
    // {
    //     sal::object::Int8Array v;  // no default ctor()

    //     REQUIRE(v.size() == 0);
    //     REQUIRE(v.typeName() == "array");
    //     //REQUIRE(v.elementTypeName() == "uint8");
    // }

    SECTION("Initialise with array.")
    {
        sal::object::Int8Array v({2, 3});

        REQUIRE(v.size() == 2 * 3);
        REQUIRE(v.dimension() == 2);
        REQUIRE(v.shape()[0] == 2);
        REQUIRE(v.type_name() == "array");
        REQUIRE(v.element_type_name() == "int8");
    }

    SECTION("Test indexing and modify element value")
    {
        sal::object::Int8Array v({2, 3});

        int8_t number = 100u;
        v[0] = number;
        REQUIRE(v[0] == number);
        REQUIRE(v(0, 0) == number);

        v[1] = -128;
        REQUIRE(v[1] == -128);

        v(1, 0) = 127;
        REQUIRE(v[3] == 127);
    }

    SECTION("Test buffer pointer for C-API")
    {
        sal::object::Int8Array v({2, 3});

        int8_t number = 100u;
        v[0] = number;

        // cp is readonly buffer
        const int8_t* cp = static_cast<const int8_t*>(v.data_pointer());
        REQUIRE(cp[0] == number);

        int8_t number2 = 20u;
        int8_t* p = static_cast<int8_t*>(v.data_pointer());
        p[0] = number2;
        REQUIRE(v(0, 0) == number2);
    }
}
