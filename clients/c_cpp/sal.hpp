#ifndef __SAL_H__
#define __SAL_H__

#include <stdint.h>
#include <float.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace sal {
/*
    typedef enum {BRANCH, LEAF} ReportType;

    char VALID_NODE_CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789-_."

    // variable identifier strings in serialised objects
    char ID_INT8[] = "int8";
    char ID_INT16[] = "int16";
    char ID_INT32[] = "int32";
    char ID_INT64[] = "int64";
    char ID_UINT8[] = "uint8";
    char ID_UINT16[] = "uint16";
    char ID_UINT32[] = "uint32";
    char ID_UINT64[] = "uint64";
    char ID_FLOAT32[] = "float32";
    char ID_FLOAT64[] = "float64";
    char ID_BOOL[] = "bool";
    char ID_STRING[] = "string";
    char ID_ARRAY[] = "array";
    char ID_BRANCH[] = "branch";

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

    typedef ScalarVariable<int8_t, ID_INT8> Int8;
    typedef ScalarVariable<int16_t, ID_INT16> Int16;
    typedef ScalarVariable<int32_t, ID_INT32> Int32;
    typedef ScalarVariable<int64_t, ID_INT64> Int64;

    typedef ScalarVariable<uint8_t, ID_UINT8> UInt8;
    typedef ScalarVariable<uint16_t, ID_UINT16> UInt16;
    typedef ScalarVariable<uint32_t, ID_UINT32> UInt32;
    typedef ScalarVariable<uint64_t, ID_UINT64> UInt64;

    typedef ScalarVariable<float, ID_FLOAT32> Float32;
    typedef ScalarVariable<double, ID_FLOAT64> Float64;
    typedef ScalarVariable<bool, ID_BOOL> Bool;
    typedef ScalarVariable<string, ID_STRING> String;

    template<class T, char const *ELEMENT_TYPE> class ArrayVariable : Variable {

        public:
            const string type = ID_ARRAY;
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

    typedef ArrayVariable<int8_t, ID_INT8> Int8Array;
    typedef ArrayVariable<int16_t, ID_INT16> Int16Array;
    typedef ArrayVariable<int32_t, ID_INT32> Int32Array;
    typedef ArrayVariable<int64_t, ID_INT64> Int64Array;

    typedef ArrayVariable<uint8_t, ID_UINT8> UInt8Array;
    typedef ArrayVariable<uint16_t, ID_UINT16> UInt16Array;
    typedef ArrayVariable<uint32_t, ID_UINT32> UInt32Array;
    typedef ArrayVariable<uint64_t, ID_UINT64> UInt64Array;

    typedef ArrayVariable<float, ID_FLOAT32> Float32Array;
    typedef ArrayVariable<double, ID_FLOAT64> Float64Array;
    typedef ArrayVariable<bool, ID_BOOL> BoolArray;
    typedef ArrayVariable<string, ID_STRING> StringArray;


    class Branch : public Variable {

        private:
            string type = ID_BRANCH;
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
*/
};

#endif
