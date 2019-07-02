//#include "sal.h"
#include <stdint.h>
#include <float.h>
//#include <stdarg.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>


using namespace std;

namespace sal {

    typedef enum {BRANCH, LEAF} ReportType;

    // variable identifier strings in serialised objects
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
    char VAR_KEY_ARRAY[] = "array";
    char VAR_KEY_BRANCH[] = "branch";

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

    // define object types
    template<class T, char const *TYPE> class ScalarVariable : Variable {
        public:
            T value;
            const string type = TYPE;

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

    template<class T, char const *ELEMENT_TYPE> class ArrayVariable : Variable {

        public:
            const string type = VAR_KEY_ARRAY;
            const string element_type = ELEMENT_TYPE;

            ArrayVariable();
//            ~ArrayVariable();
//            ArrayVariable(const ArrayVariable&);
//            ArrayVariable& operator= (const ArrayVariable&);
//            ArrayVariable(ArrayVariable&&);
//            ArrayVariable& operator= (ArrayVariable&&);

            T operator[](uint64_t index) { return this->data[index]; };

            vector<uint64_t> shape;
            vector<T> data;
    };

    typedef ArrayVariable<int8_t, VAR_KEY_INT8> Int8Array;
    typedef ArrayVariable<int16_t, VAR_KEY_INT16> Int16Array;
    typedef ArrayVariable<int32_t, VAR_KEY_INT32> Int32Array;
    typedef ArrayVariable<int64_t, VAR_KEY_INT64> Int64Array;

    typedef ArrayVariable<uint8_t, VAR_KEY_UINT8> UInt8Array;
    typedef ArrayVariable<uint16_t, VAR_KEY_UINT16> UInt16Array;
    typedef ArrayVariable<uint32_t, VAR_KEY_UINT32> UInt32Array;
    typedef ArrayVariable<uint64_t, VAR_KEY_UINT64> UInt64Array;

    typedef ArrayVariable<float, VAR_KEY_FLOAT32> Float32Array;
    typedef ArrayVariable<double, VAR_KEY_FLOAT64> Float64Array;
    typedef ArrayVariable<bool, VAR_KEY_BOOL> BoolArray;
    typedef ArrayVariable<string, VAR_KEY_STRING> StringArray;


    class Branch : public Variable {

        private:
            string type = VAR_KEY_BRANCH;
            map<string, Variable> _map;

    };

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