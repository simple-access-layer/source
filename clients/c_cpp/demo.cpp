#include "sal.h"
#include <stdint.h>
#include <float.h>
//#include <stdarg.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include <initializer_list>
#include <array>
//
//void get(std::initializer_list<int> list)
//{
//    std::cout << "arg count: " << list.size() << endl;
//    for(auto elem: list )
//    {
//        std::cout << elem << std::endl ;
//    }
//}



using namespace std;
using namespace sal::object;

int main(int argc, char **argv) {

    int64_t v = 786236756785667848;

    Int8::Ptr i8 = new Int8(v);
    Int16::Ptr i16 = new Int16(v);
    Int32::Ptr i32 = new Int32(v);
    Int64::Ptr i64 = new Int64(v);

    sal::object::UInt8 ui8(v);
    sal::object::UInt16 ui16(v);
    sal::object::UInt32 ui32(v);
    sal::object::UInt64 ui64(v);

    sal::object::Float32 f32(v);
    sal::object::Float64 f64(v);
    sal::object::Bool b(v);
    sal::object::String str("Hello!");

    cout << int64_t(i8->value) << " " << i8->type << " " <<  endl;
    i8->encode()->stringify(cout, 2);
    cout << endl;

    cout << i16->value << " " << i16->type << endl;
    i16->encode()->stringify(cout, 2);
    cout << endl;

    cout << i32->value << " " << i32->type << endl;
    i32->encode()->stringify(cout, 2);
    cout << endl;

    cout << i64->value << " " << i64->type << endl;
    i64->encode()->stringify(cout, 2);
    cout << endl;


    cout << uint64_t(ui8.value) << " " << ui8.type << endl;
    ui8.encode()->stringify(cout, 2);
    cout << endl;

    cout << ui16.value << " " << ui16.type << endl;
    ui16.encode()->stringify(cout, 2);
    cout << endl;

    cout << ui32.value << " " << ui32.type << endl;
    ui32.encode()->stringify(cout, 2);
    cout << endl;

    cout << ui64.value << " " << ui64.type << endl;
    ui64.encode()->stringify(cout, 2);
    cout << endl;


    cout << f32.value << " " << f32.type << endl;
    f32.encode()->stringify(cout, 2);
    cout << endl;

    cout << f64.value << " " << f64.type << endl;
    f64.encode()->stringify(cout, 2);
    cout << endl;

    cout << b.value << " " << b.type << endl;
    b.encode()->stringify(cout, 2);
    cout << endl;

    cout << str.value << " " << str.type << endl;
    str.encode()->stringify(cout, 2);
    cout << endl;

    sal::object::Int32Array i32a({5, 3, 2});
    cout << i32a.type << " " << i32a.element_type << " " << i32a.size() << endl;

    for (uint64_t v=0;v<i32a.size();v++) cout << i32a[v] << " ";
    cout << endl;

    i32a.at(1, 1, 0) = 50;

    for (uint64_t v=0;v<i32a.size();v++) cout << i32a[v] << " ";
    cout << endl;

    i32a.encode()->stringify(cout, 2);
    cout << endl;


    sal::object::Branch br;

    br.set("myint", i8);
//    br.set("myarr", sal::object::Int32Array::Ptr(i32a));

//    cout << br.has("wibble") << endl;
    cout << br.has("myint") << endl;
//
//    cout << br["myint"].type << endl;
//    br.get_as<sal::object::Int32>("myint")->value = 500;
//
//    br["myint"].encode()->stringify(cout, 2);
//    cout << endl;
//
    br.encode()->stringify(cout, 2);
    cout << endl;

//    br.remove("myarr");
//
//    br.encode()->stringify(cout, 2);
//    cout << endl;
//


//    sal::StringArray sa;
//    sa.data.

//
//    sal::Client client("https://sal.jet.uk");
//    sal::Report rpt = client.list("/pulse/87737/ppf/signal/jetppf/magn/ipla"); // USE c++11 move symantics
//    sal::Object obj = client.get("/pulse/87737/ppf/signal/jetppf/magn/ipla", false);  // USE c++11 move symantics
//
//    if (rpt.is_leaf()) {
//        // blah
//    } else if {
//        // blah
//    }
//
//    cout << "Object type: " << obj.type() << endl;
//
//    int n;
//    cin >> n;
//
//    for (int j=0;j<n;j++) {
//        get({j,2,3,4,5});
//    }

}