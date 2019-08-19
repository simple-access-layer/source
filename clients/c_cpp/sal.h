#ifndef __SAL_H__
#define __SAL_H__

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

namespace sal {

    extern char VALID_CHARS[];

    namespace object {

        // variable identifier strings in serialised objects
        extern char VAR_KEY_INT8[];
        extern char VAR_KEY_INT16[];
        extern char VAR_KEY_INT32[];
        extern char VAR_KEY_INT64[];
        extern char VAR_KEY_UINT8[];
        extern char VAR_KEY_UINT16[];
        extern char VAR_KEY_UINT32[];
        extern char VAR_KEY_UINT64[];
        extern char VAR_KEY_FLOAT32[];
        extern char VAR_KEY_FLOAT64[];
        extern char VAR_KEY_BOOL[];
        extern char VAR_KEY_STRING[];
        extern char VAR_KEY_ARRAY[];
        extern char VAR_KEY_BRANCH[];

        class Branch;

        class Variable {

            public:

                // needs copy and move constructors
                virtual Poco::JSON::Object::Ptr encode() = 0;

            protected:
                Branch *parent;
        };

        class Branch : public Variable {

            public:
                Poco::JSON::Object::Ptr encode() {};

            private:
                string type = VAR_KEY_BRANCH;
                map<string, Variable> _map;
        };

        // define object types
        template<class T, char const *TYPE> class ScalarVariable : Variable {

            public:
                T value;
                const string type = TYPE;

                ScalarVariable(T _value) : value(_value) {};
                Poco::JSON::Object::Ptr encode() {

                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();

                    # todo: populate me


                    return obj;
                };
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

//                ArrayVariable();
    //            ~ArrayVariable();
    //            ArrayVariable(const ArrayVariable&);
    //            ArrayVariable& operator= (const ArrayVariable&);
    //            ArrayVariable(ArrayVariable&&);
    //            ArrayVariable& operator= (ArrayVariable&&);

                T operator[](uint64_t index) { return this->data[index]; };

                vector<uint64_t> shape;
                vector<T> data;

                Poco::JSON::Object::Ptr encode() {


                };
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

    }

    namespace node {

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


        class Branch {
        };

        class Object {

            public:
                const ObjectType type;
                object::Branch root;

            // needs copy and move constructors

        };

    };

    class Client {

        public:
            Client(const string host);
            void authenticate(const string user, const string password);
            node::Report list(const string path) const;
            node::Object get(const string path, bool summary=false) const;
            void put(const string path, const node::Object obj) const;
            void copy(const string target, const string source) const;
            void del(const string path) const;

        protected:
            string serialise(node::Object obj);
            node::Object deserialise(string json);

    };

};

#endif
