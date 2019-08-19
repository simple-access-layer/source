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


using namespace std;


int main(int argc, char **argv) {

    int64_t v = 786236756785667848;

    sal::object::Int8 i8(v);
    sal::object::Int16 i16(v);
    sal::object::Int32 i32(v);
    sal::object::Int64 i64(v);

    sal::object::UInt8 ui8(v);
    sal::object::UInt16 ui16(v);
    sal::object::UInt32 ui32(v);
    sal::object::UInt64 ui64(v);

    sal::object::Float32 f32(v);
    sal::object::Float64 f64(v);
    sal::object::Bool b(v);
    sal::object::String str("Hello!");

    cout << int64_t(i8.value) << " " << i8.type << endl;
    cout << i16.value << " " << i16.type << endl;
    cout << i32.value << " " << i32.type << endl;
    cout << i64.value << " " << i64.type << endl;

    cout << uint64_t(ui8.value) << " " << ui8.type << endl;
    cout << ui16.value << " " << ui16.type << endl;
    cout << ui32.value << " " << ui32.type << endl;
    cout << ui64.value << " " << ui64.type << endl;

    cout << f32.value << " " << f32.type << endl;
    cout << f64.value << " " << f64.type << endl;
    cout << b.value << " " << b.type << endl;
    cout << str.value << " " << str.type << endl;

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
}