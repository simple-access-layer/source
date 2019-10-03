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

// todo: proper exception handling, current catch all in various places is just a quick kludge


namespace sal {

    using namespace std;

    char VALID_CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789-_.";


    namespace object {

        // the attribute type enumeration used to identify the type of Attribute object being handled
        typedef enum {
            ATTR_INT8, ATTR_INT16, ATTR_INT32, ATTR_INT64,
            ATTR_UINT8, ATTR_UINT16, ATTR_UINT32, ATTR_UINT64,
            ATTR_FLOAT32, ATTR_FLOAT64, ATTR_BOOL, ATTR_STRING,
            ATTR_INT8_ARRAY, ATTR_INT16_ARRAY, ATTR_INT32_ARRAY, ATTR_INT64_ARRAY,
            ATTR_UINT8_ARRAY, ATTR_UINT16_ARRAY, ATTR_UINT32_ARRAY, ATTR_UINT64_ARRAY,
            ATTR_FLOAT32_ARRAY, ATTR_FLOAT64_ARRAY, ATTR_STRING_ARRAY,
            ATTR_BRANCH,
        } AttributeType;

        // attribute identifier strings in serialised objects
        char ID_STR_INT8[] = "int8";
        char ID_STR_INT16[] = "int16";
        char ID_STR_INT32[] = "int32";
        char ID_STR_INT64[] = "int64";
        char ID_STR_UINT8[] = "uint8";
        char ID_STR_UINT16[] = "uint16";
        char ID_STR_UINT32[] = "uint32";
        char ID_STR_UINT64[] = "uint64";
        char ID_STR_FLOAT32[] = "float32";
        char ID_STR_FLOAT64[] = "float64";
        char ID_STR_BOOL[] = "bool";
        char ID_STR_STRING[] = "string";
        char ID_STR_ARRAY[] = "array";
        char ID_STR_BRANCH[] = "branch";


        // forward declare Branch
        class Branch;


        class Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Attribute> Ptr;
                const AttributeType type;

                /*
                Constructors and destructor.
                */
                // TODO: needs copy and move constructors
                Attribute(const AttributeType _type, const string _id_str) : type(_type), id_str(_id_str) {};
                virtual ~Attribute() {};

                virtual Poco::JSON::Object::Ptr encode() = 0;

            protected:
                const string id_str;

        };


        /*
        Data Object Scalar Attributes
        */
        template<class T, AttributeType TYPE, char const *ID_STR>
        class Scalar : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Scalar<T, TYPE, ID_STR>> Ptr;

                T value;

                /*
                Constructors and destructor.
                */
                Scalar() : Attribute(TYPE, ID_STR), value(0) {};
                Scalar(T _value) : Attribute(TYPE, ID_STR), value(_value) {};
                virtual ~Scalar() {};

                /*
                Returns a Poco JSON object representation of the Scalar.
                */
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                    json->set("type", this->type);
                    json->set("value", this->value);
                    return json;
                };

                /*
                Decodes a Poco JSON object representation of the Scalar and returns a Scalar object.
                */
                static typename Scalar<T, TYPE, ID_STR>::Ptr decode(Poco::JSON::Object::Ptr json) {

                    // treat any failure as a failure to decode
                    try {
                        // check sal type is valid for this class
                        if (json->getValue<string>("type") != string(ID_STR)) throw exception();
                        return new Scalar<T, TYPE, ID_STR>( json->getValue<T>("value") );
                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL scalar attribute.");
                    }
                };
        };


        typedef Scalar<int8_t, ATTR_INT8, ID_STR_INT8> Int8;
        typedef Scalar<int16_t, ATTR_INT16, ID_STR_INT16> Int16;
        typedef Scalar<int32_t, ATTR_INT32, ID_STR_INT32> Int32;
        typedef Scalar<int64_t, ATTR_INT64, ID_STR_INT64> Int64;

        typedef Scalar<uint8_t, ATTR_UINT8, ID_STR_UINT8> UInt8;
        typedef Scalar<uint16_t, ATTR_UINT16, ID_STR_UINT16> UInt16;
        typedef Scalar<uint32_t, ATTR_UINT32, ID_STR_UINT32> UInt32;
        typedef Scalar<uint64_t, ATTR_UINT64, ID_STR_UINT64> UInt64;

        typedef Scalar<float, ATTR_FLOAT32, ID_STR_FLOAT32> Float32;
        typedef Scalar<double, ATTR_FLOAT64, ID_STR_FLOAT64> Float64;
        typedef Scalar<bool, ATTR_BOOL, ID_STR_BOOL> Bool;


        /*
        Data Object String Attributes
        */
        class String : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<String> Ptr;

                string value;

                /*
                Constructors and destructor.
                */
                String() : Attribute(ATTR_STRING, ID_STR_STRING), value("") {};
                String(string _value) : Attribute(ATTR_STRING, ID_STR_STRING), value(_value) {};
                virtual ~String() {};

                /*
                Returns a Poco JSON object representation of the String.
                */
                Poco::JSON::Object::Ptr encode() {
                    Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                    json->set("type", this->type);
                    json->set("value", this->value);
                    return json;
                };

                /*
                Decodes a Poco JSON object representation of the String and returns a String object.
                */
                static String::Ptr decode(Poco::JSON::Object::Ptr json) {

                    // treat any failure as a failure to decode
                    try {
                        // check sal type is valid for this class
                        if (json->getValue<string>("type") != ID_STR_STRING) throw exception();
                        return new String( json->getValue<string>("value") );
                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL string attribute.");
                    }
                };
        };


        /*
        Data Object Array Attributes
        */
        template<class T, AttributeType TYPE, char const *ID_STR>
        class Array : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<Array<T, TYPE, ID_STR>> Ptr;

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
                Array(vector<uint64_t> shape) : Attribute(TYPE, ID_STR_ARRAY), element_id_str(ID_STR) {

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
                Destructor
                */
                virtual ~Array() {};

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

                    array_definition->set("type", this->element_id_str);
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
                static typename Array<T, TYPE, ID_STR>::Ptr decode(Poco::JSON::Object::Ptr json) {

                    Poco::JSON::Object::Ptr array_definition;
                    vector<uint64_t> shape;
                    string encoded_data;
                    typename Array<T, TYPE, ID_STR>::Ptr array;

                    // treat any failure as a failure to decode
                    try {

                        // check sal type is valid for this class
                        if (json->getValue<string>("type") != string(ID_STR_ARRAY)) throw exception();

                        // extract array definition
                        array_definition = json->getObject("value");

                        // check array element type and array encoding are valid for this class
                        if (array_definition->getValue<string>("type") != string(ID_STR)) throw exception();
                        if (array_definition->getValue<string>("encoding") != string("base64")) throw exception();
                        if (!array_definition->isArray("shape")) throw exception();

                        // decode shape
                        shape = Array<T, TYPE, ID_STR>::decode_shape(array_definition->getArray("shape"));

                        // create and populate array
                        array = new Array<T, TYPE, ID_STR>(shape);
                        Array<T, TYPE, ID_STR>::decode_data(array->data, array_definition->getValue<string>("data"));
                        return array;

                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                    }
                };

            protected:
                const string element_id_str;
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


        typedef Array<int8_t, ATTR_INT8_ARRAY, ID_STR_INT8> Int8Array;
        typedef Array<int16_t, ATTR_INT16_ARRAY, ID_STR_INT16> Int16Array;
        typedef Array<int32_t, ATTR_INT32_ARRAY, ID_STR_INT32> Int32Array;
        typedef Array<int64_t, ATTR_INT64_ARRAY, ID_STR_INT64> Int64Array;

        typedef Array<uint8_t, ATTR_UINT8_ARRAY, ID_STR_UINT8> UInt8Array;
        typedef Array<uint16_t, ATTR_UINT16_ARRAY, ID_STR_UINT16> UInt16Array;
        typedef Array<uint32_t, ATTR_UINT32_ARRAY, ID_STR_UINT32> UInt32Array;
        typedef Array<uint64_t, ATTR_UINT64_ARRAY, ID_STR_UINT64> UInt64Array;

        typedef Array<float, ATTR_FLOAT32_ARRAY, ID_STR_FLOAT32> Float32Array;
        typedef Array<double, ATTR_FLOAT64_ARRAY, ID_STR_FLOAT64> Float64Array;


        /*
        Data Object String Array Attribute
        */
        class StringArray : public Attribute {

            // TODO: add documentation

            public:

                typedef Poco::SharedPtr<StringArray> Ptr;

                /*
                StringArray constructor.

                Initialises an array with the specified dimensions (shape). The
                array shape is a vector defining the length of each dimensions
                of the array. The number of elements in the shape vector
                defines the number of dimensions.
                */
                StringArray(vector<uint64_t> shape) : Attribute(ATTR_STRING_ARRAY, ID_STR_ARRAY), element_id_str(ID_STR_STRING) {

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
                Destructor
                */
                virtual ~StringArray() {};

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

                    array_definition->set("type", this->element_id_str);
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
                static StringArray::Ptr decode(Poco::JSON::Object::Ptr json) {

                    Poco::JSON::Object::Ptr array_definition;
                    vector<uint64_t> shape;
                    string encoded_data;
                    typename StringArray::Ptr array;

                    // treat any failure as a failure to decode
                    try {

                        // check sal type is valid for this class
                        if (json->getValue<string>("type") != string(ID_STR_ARRAY)) throw exception();

                        // extract array definition
                        array_definition = json->getObject("value");

                        // check array element type and array encoding are valid for this class
                        if (array_definition->getValue<string>("type") != string(ID_STR_STRING)) throw exception();
                        if (array_definition->getValue<string>("encoding") != string("list")) throw exception();
                        if (!array_definition->isArray("shape")) throw exception();
                        if (!array_definition->isArray("data")) throw exception();

                        // decode shape
                        shape = StringArray::decode_shape(array_definition->getArray("shape"));

                        // create and populate array
                        array = new StringArray(shape);
                        StringArray::decode_data(array, array_definition->getArray("data"));
                        return array;

                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                    }
                };

            protected:
                const string element_id_str;
                uint8_t dimensions;
                vector<uint64_t> shape;
                vector<uint64_t> stride;
                vector<string> data;

            /*
            Converts the shape array to a POCO JSON array object.
            */
            Poco::JSON::Array::Ptr encode_shape() {
                Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
                for (uint8_t i=0;i<this->shape.size();i++) json->add(this->shape[i]);
                return json;
            };

            /*
            Decodes the shape array from a POCO JSON array object.
            */
            static vector<uint64_t> decode_shape(Poco::JSON::Array::Ptr json) {
                vector<uint64_t> shape(json->size());
                for (uint8_t i=0;i<json->size();i++) shape[i] = json->getElement<uint64_t>(i);
                return shape;
            };

            /*
            Encodes the string vector as a nested Poco Array of strings.
            */
            Poco::JSON::Array::Ptr encode_data(uint8_t dimension=0, uint64_t offset=0) {

                Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

                if (dimension == (this->dimensions - 1)) {

                    // populate innermost array with the strings
                    for (uint64_t i=0;i<this->shape[dimension];i++) {
                        json->add(this->data[offset + i]);
                    }

                } else {

                    // create nested array objects
                    for (uint64_t i=0;i<this->shape[dimension];i++) {
                        json->add(encode_data(dimension + 1, offset + i*this->stride[dimension]));
                    }

                }
                return json;
            };

            /*
            Decodes a nested Poco Array into a string vector.
            */
            static void decode_data(StringArray::Ptr array, const Poco::JSON::Array::Ptr json, uint8_t dimension=0, uint64_t offset=0) {

                if (dimension == (array->dimensions - 1)) {

                    // innermost array contains strings
                    for (uint64_t i=0;i<array->shape[dimension];i++) {
                        array->data[offset + i] = json->getElement<string>(i);
                    }

                } else {

                    // decode nested array objects
                    for (uint64_t i=0;i<array->shape[dimension];i++) {
                        decode_data(array, json->getArray(i), dimension + 1, offset + i*array->stride[dimension]);
                    }

                }
            };
        };


        // forward declare decode()
        Attribute::Ptr decode(Poco::JSON::Object::Ptr json);


        /*
        Data Object Branch Attribute
        */
        class Branch : public Attribute {

            public:

                typedef Poco::SharedPtr<Branch> Ptr;

                /*
                Constructors and destructor.
                */
                Branch() : Attribute(ATTR_BRANCH, ID_STR_BRANCH) {};
                virtual ~Branch() {};

                // TODO: better exception handling
                // TODO: add documentation
                Attribute::Ptr& operator[](const string &key) { return this->attributes.at(key); };
                Attribute::Ptr &get(const string &key) { return (*this)[key]; };
                template<class T> typename T::Ptr get_as(const string &key) { return typename T::Ptr(this->get(key).cast<T>()); };
                void set(const string &key, const Attribute::Ptr &attribute) { this->attributes[key] = attribute; };
                const bool has(const string &key) const { return this->attributes.count(key); };
                void remove (const string &key) { this->attributes.erase(key); };

                /*
                Returns a Poco JSON object representation of the Branch.
                */
                Poco::JSON::Object::Ptr encode() {

                    Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                    Poco::JSON::Object::Ptr content = new Poco::JSON::Object();

                    // encode each attribute
                    for (map<string, Attribute::Ptr>::iterator i=this->attributes.begin(); i!=this->attributes.end(); ++i)
                        content->set(i->first, i->second->encode());

                    json->set("type", this->type);
                    json->set("value", content);
                    return json;
                };

                /*
                Decodes a Poco JSON object representation of the Branch attribute.
                */
                static Branch::Ptr decode(Poco::JSON::Object::Ptr json) {

                    Poco::JSON::Object::Ptr contents;
                    vector<string> keys;
                    vector<string>::iterator key;
                    Branch::Ptr branch;

                    // treat any failure as a failure to decode
                    try {

                        // check sal type is valid for this class
                        if (json->getValue<string>("type") != ID_STR_BRANCH) throw exception();

                        // extract array definition
                        contents = json->getObject("value");

                        // create branch object and populate
                        branch = new Branch();
                        contents->getNames(keys);
                        for (key=keys.begin(); key!=keys.end(); ++key) {

                            // skip null elements
                            if (contents->isNull(*key)) continue;

                            // all valid attributes definitions are JSON objects
                            if (!contents->isObject(*key)) throw exception();

                            // dispatch object to the appropriate decoder
                            branch->set(*key, sal::object::decode(contents->getObject(*key)));
                        }
                        return branch;

                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                    }
                };

            protected:
                map<string, Attribute::Ptr> attributes;
        };


        /*
        Attempts to decode a JSON object into a SAL object attribute.
        */
        Attribute::Ptr decode(Poco::JSON::Object::Ptr json) {

            string id;

            try {
                id = json->getValue<string>("type");
            } catch(...) {
                // todo: define a sal exception and replace
                throw runtime_error("JSON object does not define a valid SAL attribute.");
            }

            cout << "decoding: " << id << endl;

            // branches
            if (id == ID_STR_BRANCH) return Branch::decode(json);

            // atomic
            if (id == ID_STR_INT8) return Int8::decode(json);
            if (id == ID_STR_INT16) return Int16::decode(json);
            if (id == ID_STR_INT32) return Int32::decode(json);
            if (id == ID_STR_INT64) return Int64::decode(json);
            if (id == ID_STR_UINT8) return UInt8::decode(json);
            if (id == ID_STR_UINT16) return UInt16::decode(json);
            if (id == ID_STR_UINT32) return UInt32::decode(json);
            if (id == ID_STR_UINT64) return UInt64::decode(json);
            if (id == ID_STR_FLOAT32) return Float32::decode(json);
            if (id == ID_STR_FLOAT64) return Float64::decode(json);
            if (id == ID_STR_BOOL) return Bool::decode(json);
            if (id == ID_STR_STRING) return String::decode(json);

            // arrays
            if (id == ID_STR_ARRAY) {

                Poco::JSON::Object::Ptr array_definition;
                string element_id;

                try {
                    array_definition = json->getObject("value");
                    element_id = array_definition->getValue<string>("type");
                } catch(...) {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL attribute.");
                }

                if (element_id == ID_STR_INT8) return Int8Array::decode(json);
                if (element_id == ID_STR_INT16) return Int16Array::decode(json);
                if (element_id == ID_STR_INT32) return Int32Array::decode(json);
                if (element_id == ID_STR_INT64) return Int64Array::decode(json);
                if (element_id == ID_STR_UINT8) return UInt8Array::decode(json);
                if (element_id == ID_STR_UINT16) return UInt16Array::decode(json);
                if (element_id == ID_STR_UINT32) return UInt32Array::decode(json);
                if (element_id == ID_STR_UINT64) return UInt64Array::decode(json);
                if (element_id == ID_STR_FLOAT32) return Float32Array::decode(json);
                if (element_id == ID_STR_FLOAT64) return Float64Array::decode(json);
                if (element_id == ID_STR_STRING) return StringArray::decode(json);
            }

            // todo: define a sal exception and replace
            throw runtime_error("JSON object does not define a valid SAL attribute.");
        }


        /*
        Attempts to decode a JSON object into the specified SAL object attribute.

        Returns null pointer if cast is invalid.
        */
        template<class T> typename T::Ptr decode_as(Poco::JSON::Object::Ptr json) {
            return typename T::Ptr(decode(json).cast<T>());
        };
    }

    namespace node {

        typedef enum {NODE_BRANCH, NODE_LEAF} NodeType;

        char JSON_CONTENT_REPORT[] = "report";
        char JSON_CONTENT_OBJECT[] = "object";

        char JSON_TYPE_LEAF[] = "leaf";
        char JSON_TYPE_BRANCH[] = "branch";

        char OBJ_TYPE_FULL[] = "object";
        char OBJ_TYPE_SUMMARY[] = "summary";


        class LeafType {

            public:
                LeafType();
                const string cls;
                const string group;
                const int version;
        };


        class Report {

            public:

                typedef Poco::SharedPtr<Report> Ptr;

                Report();
                ~Report();

                bool is_leaf();
                bool is_branch();

                const NodeType type;
                const string timestamp;  // todo: use something better std::chrono::time_point?
                const vector<string> branches;
                const vector<LeafType> leaves;
        //        const

            // needs copy and move constructors

        };


        class Object {

            public:

                typedef Poco::SharedPtr<Object> Ptr;

                static const NodeType type;


        };


        class Branch : public Object {

            public:
                static const NodeType type = NODE_BRANCH;


        };


        class Leaf : public Object {

            public:

                static const NodeType type = NODE_LEAF;
                const string cls;
                const string group;
                const uint64_t version;
                const bool summary;

                typedef Poco::SharedPtr<Leaf> Ptr;

                // todo: needs copy and move constructors
                Leaf(string _cls, string _group, uint64_t _version, bool _summary) : cls(_cls), group(_group), version(_version), summary(_summary) {};
                virtual ~Leaf() {};

                object::Attribute::Ptr& operator[](const string &key) { return this->attributes.at(key); };
                object::Attribute::Ptr &get(const string &key) { return (*this)[key]; };
                template<class T> typename T::Ptr get_as(const string &key) { return typename T::Ptr(this->get(key).cast<T>()); };
                void set(const string &key, const object::Attribute::Ptr &attribute) { this->attributes[key] = attribute; };
                const bool has(const string &key) const { return this->attributes.count(key); };
                void remove (const string &key) { this->attributes.erase(key); };


                /*
                Decodes a Poco JSON object representation of the data object and returns a SAL data object.
                */
                static Leaf::Ptr decode(Poco::JSON::Object::Ptr json) {

                    vector<string> keys;
                    vector<string>::iterator key;
                    Leaf::Ptr obj;

                    // treat any failure as a failure to decode
                    try {

                        // extract class, group, version and object type
                        object::String::Ptr cls = object::decode_as<object::String>(json->getObject("_class"));
                        object::String::Ptr group = object::decode_as<object::String>(json->getObject("_group"));
                        object::UInt64::Ptr version = object::decode_as<object::UInt64>(json->getObject("_version"));
                        object::String::Ptr type = object::decode_as<object::String>(json->getObject("_type"));

                        // create object and populate
                        obj = new Leaf(cls->value, group->value, version->value, type->value == OBJ_TYPE_SUMMARY);
                        json->getNames(keys);
                        for (key=keys.begin(); key!=keys.end(); ++key) {

                            // skip null elements
                            if (json->isNull(*key)) continue;

                            // all valid attributes definitions are JSON objects
                            if (!json->isObject(*key)) throw exception();

                            // dispatch object to the appropriate decoder
                            obj->set(*key, sal::object::decode(json->getObject(*key)));
                        }
                        return obj;

                    } catch(...) {
                        // todo: define a sal exception and replace
                        throw runtime_error("JSON object does not define a valid SAL data object.");
                    }
                };

            protected:
                map<string, object::Attribute::Ptr> attributes;
        };


        /*
        Attempts to decode a JSON object into a SAL object.
        */
        Object::Ptr decode_object(Poco::JSON::Object::Ptr json) {

            string content;
            string type;
            Poco::JSON::Object::Ptr object;

            try {
                 content = json->getValue<string>("content");
                 if (content != JSON_CONTENT_OBJECT) throw exception();

                 type = json->getValue<string>("type");

                 if (json->isObject("object")) throw exception();
                 object = json->getObject("object");
            } catch(...) {
                // todo: define a sal exception and replace
                throw runtime_error("JSON object does not define a valid SAL object.");
            }

            cout << "decoding object: " <<  content << ", " << type << endl;

            // TODO: implement me
//            if (type == JSON_TYPE_BRANCH) return Branch::decode(object);
            if (type == JSON_TYPE_LEAF) return Leaf::decode(object);


            // todo: define a sal exception and replace
            throw runtime_error("JSON object does not define a valid SAL object.");
        }


        /*
        Attempts to decode a JSON object into the specified SAL object.

        Returns null pointer if cast is invalid.
        */
        template<class T> typename T::Ptr decode_object_as(Poco::JSON::Object::Ptr json) {
            return typename T::Ptr(decode_object(json).cast<T>());
        };


        /*
        Attempts to decode a JSON object into a SAL object.
        */
        Report::Ptr decode_report(Poco::JSON::Object::Ptr json) {
            // TODO: implement me
        };
    };

    class Client {

        public:
            Client(const string host);
            virtual ~Client() {};
            void authenticate(const string user, const string password);
            node::Report list(const string path) const;
            node::Object object(const string path, bool summary=false) const;
            void put(const string path, const node::Object obj) const;
            void copy(const string target, const string source) const;
            void del(const string path) const;

        protected:
            string serialise(node::Object obj);
            node::Object deserialise(string json);

    };

};

#endif
