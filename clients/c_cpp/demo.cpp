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

//    sal::node::Object("signal", "signal", 1);

    Int8::Ptr i1 = new Int8(65);
    Int8::Ptr i2;
    Poco::JSON::Object::Ptr obj;

    obj = i1->encode();

    i2 = Int8::decode(obj);

    i1->encode()->stringify(cout, 2);
    cout << endl;
    i2->encode()->stringify(cout, 2);
    cout << endl;


    String::Ptr s1 = new String("This is a description.");
    String::Ptr s2;

    obj = s1->encode();

    s2 = String::decode(obj);

    s1->encode()->stringify(cout, 2);
    cout << endl;
    s2->encode()->stringify(cout, 2);
    cout << endl;

    Float64Array::Ptr f64array1 = new Float64Array({5, 3, 2});
    Float64Array::Ptr f64array2;

    for (uint64_t i=0;i<f64array1->size();i++) {
        (*f64array1)[i] = 5.463 * i;
    }
    cout << endl;

    obj = f64array1->encode();

    f64array2 = Float64Array::decode(obj);

    f64array1->encode()->stringify(cout, 2);
    cout << endl;
    f64array2->encode()->stringify(cout, 2);
    cout << endl;

    for (uint64_t i=0;i<f64array2->size();i++) {
        cout << (*f64array1)[i] << " ";
        cout << (*f64array2)[i] << endl;
    }
    cout << endl;



    StringArray::Ptr sarray1 = new StringArray({3, 2});
    StringArray::Ptr sarray2;

    sarray1->at(0,0) = "alpha";
    sarray1->at(0,1) = "beta";
    sarray1->at(1,0) = "delta";
    sarray1->at(1,1) = "gamma";
    sarray1->at(2,0) = "eta";
    sarray1->at(2,1) = "omega";

    obj = sarray1->encode();

    sarray1->encode()->stringify(cout, 2);
    cout << endl;

    sarray2 = StringArray::decode(obj);

    sarray2->encode()->stringify(cout, 2);
    cout << endl;

    for (uint64_t i=0;i<sarray2->size();i++) {
        cout << (*sarray1)[i] << " ";
        cout << (*sarray2)[i] << endl;
    }
    cout << endl;

    Attribute::Ptr a = decode(i2->encode());
    cout << a->type << endl;

    Int8::Ptr i3 = decode_as<Int8>(i2->encode());
    if (i3) cout << i3->type << endl;

    Branch::Ptr br = new Branch();

    br->set("i8", new Int8(v));
    br->set("i16", new Int16(v));
    br->set("i32", new Int32(v));
    br->set("i64", new Int64(v));

    br->set("ui8", new UInt8(v));
    br->set("ui16", new UInt16(v));
    br->set("ui32", new UInt32(v));
    br->set("ui64", new UInt64(v));

    br->set("f32", new Float32(v));
    br->set("f64", new Float64(v));
    br->set("bl", new Bool());
    br->set("str", new String("Hello!"));

    Float64Array::Ptr f64a = new Float64Array({5, 3, 2});

    for (uint64_t i=0;i<f64a->size();i++) {
        (*f64a)[i] = 5.463 * i;
        cout << (*f64a)[i] << " ";
    }
    cout << endl;

    Branch::Ptr br2 = new Branch();
    br->set("ab", br2);
    br->get_as<Branch>("ab")->set("array", f64a);
    br->get_as<Branch>("ab")->set("array2", new Float32Array({10, 2}));

    br->encode()->stringify(cout, 2);
    cout << endl;

    auto brd = decode(br->encode());
    brd->encode()->stringify(cout, 2);
    cout << endl;



// -------------------JUNK

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