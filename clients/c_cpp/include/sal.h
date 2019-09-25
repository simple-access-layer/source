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
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"


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

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Attribute> Ptr;
                const string type;

                Attribute(const string _type) : type(_type) {};

                // TODO: needs copy and move constructors
                virtual Poco::JSON::Object::Ptr encode() = 0;
        };

        /*
        Data Object Scalar Attributes
        */
        template<class T, char const *TYPE>
        class Scalar : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Scalar<T, TYPE>> Ptr;

                T value;

                /*
                Constructors.
                */
                Scalar() : Attribute(TYPE), value(0) {};
                Scalar(T _value) : Attribute(TYPE), value(_value) {};

                /*
                Returns a Poco JSON object representation of the Scalar.
                */
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
                    obj->set("type", this->type);
                    obj->set("value", this->value);
                    return obj;
                };

                /*
                Decodes a Poco JSON object representation of the Scalar and returns a Scalar object.
                */
                static typename Scalar<T, TYPE>::Ptr decode(Poco::JSON::Object::Ptr obj) {

                    // treat any failure as a failure to decode
                    try {
                        // check sal type is valid for this class
                        if (obj->getValue<string>("type") != string(TYPE)) throw exception();
                        return new Scalar<T, TYPE>( obj->getValue<T>("value") );
                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL scalar attribute.");
                    }
                };
        };

        typedef Scalar<int8_t, VAR_KEY_INT8> Int8;
        typedef Scalar<int16_t, VAR_KEY_INT16> Int16;
        typedef Scalar<int32_t, VAR_KEY_INT32> Int32;
        typedef Scalar<int64_t, VAR_KEY_INT64> Int64;

        typedef Scalar<uint8_t, VAR_KEY_UINT8> UInt8;
        typedef Scalar<uint16_t, VAR_KEY_UINT16> UInt16;
        typedef Scalar<uint32_t, VAR_KEY_UINT32> UInt32;
        typedef Scalar<uint64_t, VAR_KEY_UINT64> UInt64;

        typedef Scalar<float, VAR_KEY_FLOAT32> Float32;
        typedef Scalar<double, VAR_KEY_FLOAT64> Float64;
        typedef Scalar<bool, VAR_KEY_BOOL> Bool;

        /*
        Data Object String Attributes
        */
        class String : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<String> Ptr;

                string value;

                /*
                Constructors.
                */
                String() : Attribute(VAR_KEY_STRING), value("") {};
                String(string _value) : Attribute(VAR_KEY_STRING), value(_value) {};

                /*
                Returns a Poco JSON object representation of the String.
                */
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
                    obj->set("type", this->type);
                    obj->set("value", this->value);
                    return obj;
                };

                /*
                Decodes a Poco JSON object representation of the String and returns a String object.
                */
                static String::Ptr decode(Poco::JSON::Object::Ptr obj) {

                    // treat any failure as a failure to decode
                    try {
                        // check sal type is valid for this class
                        if (obj->getValue<string>("type") != VAR_KEY_STRING) throw exception();
                        return new String( obj->getValue<string>("value") );
                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL string attribute.");
                    }
                };
        };

        /*
        Data Object Array Attributes
        */
        template<class T, char const *ELEMENT_TYPE>
        class Array : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Array<T, ELEMENT_TYPE>> Ptr;

                const string element_type;

                /*
                Array constructor.

                Initialises an array with the specified dimensions (shape). The
                array shape is a vector defining the length of each dimensions
                of the array. The number of elements in the shape vector
                defines the number of dimensions.

                This class is not intended to be used directly by the users, a
                set of typedefs are provided that define the supported SAL
                array types. For example:

                    // create a 1D uint8 array with 1000 elements.
                    UInt8Array a1({1000});

                    // create a 2D int32 array with 50x20 elements.
                    Int32Array a2({50, 20});

                    // create a 3D float array with 512x512x3 elements.
                    Float32Array a3({512, 512, 3});

                */
                Array(vector<uint64_t> shape) : Attribute(VAR_KEY_ARRAY), element_type(ELEMENT_TYPE) {

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

    //            Array(const Array&);
    //            Array& operator= (const Array&);
    //            Array(Array&&);
    //            Array& operator= (Array&&);

                /*
                Returns the length of the array buffer.
                */
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

                /*
                Returns a Poco JSON object representation of the Array.
                */
                Poco::JSON::Object::Ptr encode() {

                    Poco::JSON::Object::Ptr array_definition = new Poco::JSON::Object();
                    Poco::JSON::Object::Ptr attribute = new Poco::JSON::Object();

                    array_definition->set("type", this->element_type);
                    array_definition->set("shape", this->encode_shape());
                    array_definition->set("encoding", "base64");
                    array_definition->set("data", this->encode_data());

                    attribute->set("type", this->type);
                    attribute->set("value", array_definition);

                    return attribute;
                };

                /*
                Decodes a Poco JSON object representation of the Array and returns an Array object.
                */
                static typename Array<T, ELEMENT_TYPE>::Ptr decode(Poco::JSON::Object::Ptr obj) {

                    Poco::JSON::Object::Ptr array_definition;
                    vector<uint64_t> shape;
                    string encoded_data;
                    typename Array<T, ELEMENT_TYPE>::Ptr array;

                    // treat any failure as a failure to decode
                    try {

                        // check sal type is valid for this class
                        if (obj->getValue<string>("type") != string(VAR_KEY_ARRAY)) throw exception();

                        // extract array definition
                        array_definition = obj->getObject("value");

                        // check array element type and array encoding are valid for this class
                        if (array_definition->getValue<string>("type") != string(ELEMENT_TYPE)) throw exception();
                        if (array_definition->getValue<string>("encoding") != string("base64")) throw exception();
                        if (!array_definition->isArray("shape")) throw exception();

                        // decode shape
                        shape = Array<T, ELEMENT_TYPE>::decode_shape(array_definition->getArray("shape"));

                        // create and populate array
                        array = new Array<T, ELEMENT_TYPE>(shape);
                        Array<T, ELEMENT_TYPE>::decode_data(array->data, array_definition->getValue<string>("data"));
                        return array;

                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                    }
                };

            protected:
                uint8_t dimensions;
                vector<uint64_t> shape;
                vector<uint64_t> stride;
                vector<T> data;

            /*
            Converts the shape array to a POCO JSON array object.
            */
            Poco::JSON::Array::Ptr encode_shape() {
                Poco::JSON::Array::Ptr shape = new Poco::JSON::Array();
                for (uint8_t i=0;i<this->shape.size();i++) shape->add(this->shape[i]);
                return shape;
            };

            /*
            Decodes the shape array from a POCO JSON array object.
            */
            static vector<uint64_t> decode_shape(Poco::JSON::Array::Ptr encoded) {
                vector<uint64_t> shape(encoded->size());
                for (uint8_t i=0;i<encoded->size();i++) shape[i] = encoded->getElement<uint64_t>(i);
                return shape;
            };

            /*
            Encodes the data buffer as a base64 string.
            */
            const string encode_data() {
                stringstream s;
                Poco::Base64Encoder encoder(s, Poco::BASE64_URL_ENCODING);
                encoder.write(reinterpret_cast<char*>(this->data.data()), this->data.size() * sizeof(T));
                encoder.close();
                return s.str();
            };

            /*
            Decodes the data buffer from a base64 string.
            */
            static void decode_data(vector<T> &data, const string b64) {
                stringstream s(b64);
                Poco::Base64Decoder decoder(s, Poco::BASE64_URL_ENCODING);
                decoder.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(T));
            };
        };

        typedef Array<int8_t, VAR_KEY_INT8> Int8Array;
        typedef Array<int16_t, VAR_KEY_INT16> Int16Array;
        typedef Array<int32_t, VAR_KEY_INT32> Int32Array;
        typedef Array<int64_t, VAR_KEY_INT64> Int64Array;

        typedef Array<uint8_t, VAR_KEY_UINT8> UInt8Array;
        typedef Array<uint16_t, VAR_KEY_UINT16> UInt16Array;
        typedef Array<uint32_t, VAR_KEY_UINT32> UInt32Array;
        typedef Array<uint64_t, VAR_KEY_UINT64> UInt64Array;

        typedef Array<float, VAR_KEY_FLOAT32> Float32Array;
        typedef Array<double, VAR_KEY_FLOAT64> Float64Array;
        typedef Array<bool, VAR_KEY_BOOL> BoolArray;

        /*
        Data Object String Array Attribute
        */
        class StringArray : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<StringArray> Ptr;

                const string element_type;

                /*
                StringArray constructor.

                Initialises an array with the specified dimensions (shape). The
                array shape is a vector defining the length of each dimensions
                of the array. The number of elements in the shape vector
                defines the number of dimensions.
                */
                StringArray(vector<uint64_t> shape) : Attribute(VAR_KEY_ARRAY), element_type(VAR_KEY_STRING) {

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

    //            StringArray(const StringArray&);
    //            StringArray& operator= (const StringArray&);
    //            StringArray(StringArray&&);
    //            StringArray& operator= (StringArray&&);

                /*
                Returns the length of the array buffer.
                */
                uint64_t size() const { return this->data.size(); };

                /*
                Fast element access via direct indexing of the array buffer.

                The Array holds the data in a 1D strided array. Indexing into
                multidimensional arrays therefore requires the user to
                appropriately stride across the data. See the stride attribute.

                No bounds checking is performed.
                */
                string& operator[](uint64_t index) { return this->data[index]; };

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
                string& at(int i0, int64_t i1=-1, int64_t i2=-1, int64_t i3=-1, int64_t i4=-1,
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

                /*
                Returns a Poco JSON object representation of the Array.
                */
                Poco::JSON::Object::Ptr encode() {

                    Poco::JSON::Object::Ptr array_definition = new Poco::JSON::Object();
                    Poco::JSON::Object::Ptr attribute = new Poco::JSON::Object();

                    array_definition->set("type", this->element_type);
                    array_definition->set("shape", this->encode_shape());
                    array_definition->set("encoding", "list");
                    array_definition->set("data", this->encode_data());

                    attribute->set("type", this->type);
                    attribute->set("value", array_definition);

                    return attribute;
                };

                /*
                Decodes a Poco JSON object representation of the Array and returns a StringArray object.
                */
                static StringArray::Ptr decode(Poco::JSON::Object::Ptr obj) {

                    Poco::JSON::Object::Ptr array_definition;
                    vector<uint64_t> shape;
                    string encoded_data;
                    typename StringArray::Ptr array;

                    // treat any failure as a failure to decode
                    try {

                        // check sal type is valid for this class
                        if (obj->getValue<string>("type") != string(VAR_KEY_ARRAY)) throw exception();

                        // extract array definition
                        array_definition = obj->getObject("value");

                        // check array element type and array encoding are valid for this class
                        if (array_definition->getValue<string>("type") != string(VAR_KEY_STRING)) throw exception();
                        if (array_definition->getValue<string>("encoding") != string("list")) throw exception();
                        if (!array_definition->isArray("shape")) throw exception();
                        if (!array_definition->isArray("data")) throw exception();

                        // decode shape
                        shape = StringArray::decode_shape(array_definition->getArray("shape"));

                        // create and populate array
                        array = new StringArray(shape);
                        StringArray::decode_data(array->data, array_definition->getArray("data"));
                        return array;

                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                    }
                };

            protected:
                uint8_t dimensions;
                vector<uint64_t> shape;
                vector<uint64_t> stride;
                vector<string> data;

            /*
            Converts the shape array to a POCO JSON array object.
            */
            Poco::JSON::Array::Ptr encode_shape() {
                Poco::JSON::Array::Ptr shape = new Poco::JSON::Array();
                for (uint8_t i=0;i<this->shape.size();i++) shape->add(this->shape[i]);
                return shape;
            };

            /*
            Decodes the shape array from a POCO JSON array object.
            */
            static vector<uint64_t> decode_shape(Poco::JSON::Array::Ptr encoded) {
                vector<uint64_t> shape(encoded->size());
                for (uint8_t i=0;i<encoded->size();i++) shape[i] = encoded->getElement<uint64_t>(i);
                return shape;
            };

            /*
            Encodes the string vector as a Poco Array of strings.
            */
            // TODO: double check if python returns a 1D list or an ND list, if ND this needs to be rewritten to write nested lists
            Poco::JSON::Array::Ptr encode_data() {
                Poco::JSON::Array::Ptr array = new Poco::JSON::Array();
                for (uint64_t i=0;i<this->data.size();i++) array->add(this->data[i]);
                return array;
            };

            /*
            Decodes the Poco Array into a string vector.
            */
            // TODO: double check if python returns a 1D list or an ND list, if ND this needs to be rewritten to write nested lists
            static void decode_data(vector<string> &data, const Poco::JSON::Array::Ptr array) {
                for (uint8_t i=0;i<data.size();i++) data[i] = array->getElement<string>(i);
            };
        };


        /*
        Data Object Branch Attribute
        */
        class Branch : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Branch> Ptr;

                /*
                Constructors.
                */
                Branch() : Attribute(VAR_KEY_BRANCH) {};

                // TODO: better exception handling
                // TODO: add documentation
                Attribute::Ptr& operator[](const string &key) { return this->attributes.at(key); };
                Attribute::Ptr &get(const string &key) { return (*this)[key]; };
                template<class T> typename T::Ptr get_as(const string &key) { return typename T::Ptr(this->get(key).cast<T>()); };
                void set(const string &key, const Attribute::Ptr &attribute) { this->attributes[key] = attribute; };
                const bool has(const string &key) const { return this->attributes.count(key); };
                void remove (const string &key) { this->attributes.erase(key); };

                /*
                Returns a Poco JSON object representation of the String.
                */
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
                    Poco::JSON::Object::Ptr content = new Poco::JSON::Object();

                    // encode each attribute
                    for (map<string, Attribute::Ptr>::iterator i=this->attributes.begin(); i!=this->attributes.end(); ++i)
                        content->set(i->first, i->second->encode());

                    obj->set("type", this->type);
                    obj->set("value", content);
                    return obj;
                };

                // todo: add decoding

            protected:
                map<string, Attribute::Ptr> attributes;
        };


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
