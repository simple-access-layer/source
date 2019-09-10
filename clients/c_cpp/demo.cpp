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

    UInt8::Ptr ui8 = new UInt8(v);
    UInt16::Ptr ui16 = new UInt16(v);
    UInt32::Ptr ui32 = new UInt32(v);
    UInt64::Ptr ui64 = new UInt64(v);

    Float32::Ptr f32 = new Float32(v);
    Float64::Ptr f64 = new Float64(v);
    Bool::Ptr b = new Bool(v);
    String::Ptr str = new String("Hello!");

    Int32Array::Ptr i32a = new Int32Array({5, 3, 2});
    cout << i32a->type << " " << i32a->element_type << " " << i32a->size() << endl;

    for (uint64_t v=0;v<i32a->size();v++) cout << (*i32a)[v] << " ";
    cout << endl;

    i32a->at(1, 1, 0) = 50;

    for (uint64_t v=0;v<i32a->size();v++) cout << (*i32a)[v] << " ";
    cout << endl;

    i32a->encode()->stringify(cout, 2);
    cout << endl;

    Branch::Ptr br = new Branch();

    br->set("myint", i8);
    br->set("myarr", i32a);

    cout << br->has("wibble") << endl;
    cout << br->has("myint") << endl;

    cout << (*br)["myint"]->type << endl;


    br->get_as<Int8>("myint")->value = 125;

    (*br)["myint"]->encode()->stringify(cout, 2);
    cout << endl;
//
    br->encode()->stringify(cout, 2);
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