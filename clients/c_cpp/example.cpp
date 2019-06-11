//#include "sal.h"
#include <stdint.h>
#include <float.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

namespace sal {

    typedef enum {BRANCH, LEAF} ReportType;

    class ObjectType {

        public:
            ObjectType();
            const string cls;
            const string group;
            const int version;
    };


    class Report {

        public:
            Report();
            ~Report();

            bool is_leaf();
            bool is_branch();

            const ReportType type;
            const string timestamp;  // todo: use something better std::chrono::time_point?
            const vector<string> branches;
            const vector<ObjectType> leaves;
    //        const

        // needs copy and move constructors

    };

    class Branch;

    class Variable {

        protected:
            Branch *parent;

        // needs copy and move constructors
    };
//
//    class Int8 : public Variable {
//
//        public:
//            int8_t value;
//            const string type = "int8";
//
//    };
//
//    class Int16 : public Variable {
//        public:
//            int16_t value;
//            const string type = "int16";
//    };
//
//    class Int32 : public Variable {
//        public:
//            int32_t value;
//            const string type = "int32";
//    };
//
//    class Int64 : public Variable {
//        public:
//            int64_t value;
//            const string type = "int64";
//    };
//
//    class UInt8 : public Variable {
//        public:
//            uint8_t value;
//            const string type = "uint8";
//    };
//
//    class UInt16 : public Variable {
//        public:
//            uint16_t value;
//            const string type = "uint16";
//    };
//
//    class UInt32 : public Variable {
//        public:
//            uint32_t value;
//            const string type = "uint32";
//    };
//
//    class UInt64 : public Variable {
//        public:
//            uint64_t value;
//            const string type = "uint64";
//    };
//
//    class Float32 : public Variable {
//        public:
//            float value;
//            const string type = "float32";
//    };
//
//    class Float64 : public Variable {
//        public:
//            double value;
//            const string type = "float64";
//    };
//
//    class Bool : public Variable {
//        public:
//            bool value;
//            const string type = "bool";
//    };
//
//    class String : public Variable {
//        public:
//            string value;
//            const string type = "string";
//    };

    char VAR_KEY_INT8[] = "int8";
    char VAR_KEY_INT16[] = "int16";
    char VAR_KEY_INT32[] = "int32";
    char VAR_KEY_INT64[] = "int64";

    char VAR_KEY_UINT8[] = "uint8";
    char VAR_KEY_UINT16[] = "uint16";
    char VAR_KEY_UINT32[] = "uint32";
    char VAR_KEY_UINT64[] = "uint64";

    char VAR_KEY_FLOAT32[] = "float32";
    char VAR_KEY_FLOAT64[] = "float64";

    char VAR_KEY_BOOL[] = "bool";

    char VAR_KEY_STRING[] = "string";

    template<class T, char const *NAME> class ScalarVariable : Variable {
        public:
            T value;
            const string name = NAME;

            ScalarVariable(T _value) : value(_value) {};
    };

    typedef ScalarVariable<int8_t, VAR_KEY_INT8> Int8;
    typedef ScalarVariable<int16_t, VAR_KEY_INT16> Int16;
    typedef ScalarVariable<int32_t, VAR_KEY_INT32> Int32;
    typedef ScalarVariable<int64_t, VAR_KEY_INT64> Int64;

    typedef ScalarVariable<uint8_t, VAR_KEY_UINT8> UInt8;
    typedef ScalarVariable<uint16_t, VAR_KEY_UINT16> UInt16;
    typedef ScalarVariable<uint32_t, VAR_KEY_UINT32> UInt32;
    typedef ScalarVariable<uint64_t, VAR_KEY_UINT64> UInt64;

    typedef ScalarVariable<float, VAR_KEY_FLOAT32> Float32;
    typedef ScalarVariable<double, VAR_KEY_FLOAT64> Float64;
    typedef ScalarVariable<bool, VAR_KEY_BOOL> Bool;
    typedef ScalarVariable<string, VAR_KEY_STRING> String;

    class Array : public Variable {
        public:
        const string type = "array";

    };

    class Branch : public Variable {

        private:
            map<string, Variable> _map;

    };

//    class Null : public Variable {
//    };

    class Object {

        public:
            const ObjectType type;
            Branch root;

        // needs copy and move constructors

    };

    class Client {

        public:
            Client(const string host);
            void authenticate(const string user, const string password);
            Report list(const string path) const;
            Object get(const string path, bool summary=false) const;
            void put(const string path, const Object obj) const;
            void copy(const string target, const string source) const;
            void del(const string path) const;
    };

};


int main(int argc, char **argv) {

    int64_t v = 786236756785667848;

    sal::Int8 i8(v);
    sal::Int16 i16(v);
    sal::Int32 i32(v);
    sal::Int64 i64(v);

    sal::UInt8 ui8(v);
    sal::UInt16 ui16(v);
    sal::UInt32 ui32(v);
    sal::UInt64 ui64(v);

    sal::Float32 f32(v);
    sal::Float64 f64(v);
    sal::Bool b(v);
    sal::String str("Hello!");

    cout << int64_t(i8.value) << " " << i8.name << endl;
    cout << i16.value << " " << i16.name << endl;
    cout << i32.value << " " << i32.name << endl;
    cout << i64.value << " " << i64.name << endl;

    cout << uint64_t(ui8.value) << " " << ui8.name << endl;
    cout << ui16.value << " " << ui16.name << endl;
    cout << ui32.value << " " << ui32.name << endl;
    cout << ui64.value << " " << ui64.name << endl;

    cout << f32.value << " " << f32.name << endl;
    cout << f64.value << " " << f64.name << endl;
    cout << b.value << " " << b.name << endl;
    cout << str.value << " " << str.name << endl;

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