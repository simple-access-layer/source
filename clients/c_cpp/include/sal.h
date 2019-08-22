#ifndef __SAL_H__
#define __SAL_H__

#include <stdint.h>
#include <float.h>
//#include <stdarg.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stdexcept>
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

        class Attribute {

            public:

                // needs copy and move constructors
                virtual Poco::JSON::Object::Ptr encode() = 0;

            protected:
                Branch *parent;
        };


        /*
        Data Object Branch Attribute
        */
        class Branch : public Attribute {

            public:
                Poco::JSON::Object::Ptr encode() {};

            private:
                string type = VAR_KEY_BRANCH;
                map<string, Attribute> _map;
        };


        /*
        Data Object Scalar Attributes
        */
        template<class T, char const *TYPE>
        class ScalarVariable : Attribute {

            public:
                T value;
                const string type = TYPE;

                ScalarVariable() : value(0) {};
                ScalarVariable(T _value) : value(_value) {};

                /*
                Returns a Poco JSON object representation of the ScalarVariable.
                */
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
                    obj->set("type", this->type);
                    obj->set("value", this->value);
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

        /*
        Data Object String Attributes
        */
        class String : Attribute {

            public:
                string value;
                const string type = VAR_KEY_STRING;

                String() : value("") {};
                String(string _value) : value(_value) {};

                // Returns a Poco JSON object representation of the String.
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
                    obj->set("type", this->type);
                    obj->set("value", this->value);
                    return obj;
                };
        };

        /*
        Data Object Array Attributes
        */
        template<class T, char const *ELEMENT_TYPE>
        class ArrayVariable : Attribute {

            public:
                const string type = VAR_KEY_ARRAY;
                const string element_type = ELEMENT_TYPE;

                ArrayVariable(vector<uint64_t> shape) {

                    this->dimensions = shape.size();
                    this->shape = shape;
                    this->stride.resize(this->dimensions);

                    // calculate strides
                    int16_t i = this->dimensions - 1;
                    this->stride[i--] = 1;
                    while (i >= 0) {
                        this->stride[i] = this->stride[i+1] * this->shape[i+1];
                        i--;
                    }

                    // calculate array buffer length
                    uint64_t size = 1;
                    for (uint64_t d: this->shape) size *= d;

                    // create buffer
                    this->data.resize(size);
                };


    //            ArrayVariable(const ArrayVariable&);
    //            ArrayVariable& operator= (const ArrayVariable&);
    //            ArrayVariable(ArrayVariable&&);
    //            ArrayVariable& operator= (ArrayVariable&&);

                uint64_t size() const { return this->data.size(); };

                /*
                Fast element access via direct indexing of the array buffer.

                The Array holds the data in a 1D strided array. Indexing into
                multidimensional arrays therefore requires the user to
                appropriately stride across the data. See the stride attribute.

                No bounds checking is performed.
                */
                T& operator[](uint64_t index) { return this->data[index]; };

                /*
                Access an element of the array.

                This method performs bounds checking and accepts a variable
                number of array indices corresponding to the dimensionality of
                the array.

                Data access is slower than direct buffer indexing, however it
                handles striding for the user.

                Due to the method of implementing this functionality in C++, it
                only supports arrays with a maximum of 10 dimensions.
                */
                T& at(int i0, int64_t i1=-1, int64_t i2=-1, int64_t i3=-1, int64_t i4=-1,
                      int64_t i5=-1, int64_t i6=-1, int64_t i7=-1, int64_t i8=-1, int64_t i9=-1) {

                    if (this->dimensions > 10) {
                        throw out_of_range("The at() method can only be used with arrays of 10 dimensions of less.");
                    }

                    // convert the list or arguments to an array for convenience
                    array<int64_t, 10> dim_index = {i0, i1, i2, i3, i4, i5, i6, i7, i8, i9};

                    uint64_t element_index = 0;
                    for (uint8_t i=0; i<this->dimensions; i++) {

                        // check the indices are inside the array bounds
                        if ((dim_index[i] < 0) || (dim_index[i] > this->shape[i] - 1)) {
                            throw out_of_range("An array index is missing or is out of bounds.");
                        }

                        element_index += dim_index[i] * this->stride[i];
                    }

                    return this->data[element_index];
                }

            protected:
                uint8_t dimensions;
                vector<uint64_t> shape;
                vector<uint64_t> stride;
                vector<T> data;

                /*
                Returns a Poco JSON object representation of the ArrayVariable.
                */
                Poco::JSON::Object::Ptr encode() {

                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
                    Poco::JSON::Array::Ptr shape = new Poco::JSON::Array();

                    // todo: encode data in base 64
                    // todo: populate shape

                    obj->set("type", this->type);
                    obj->set("encoding", "base64");
                    // obj->set("data", data);
                    return obj;
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
