#pragma once

#include <float.h>
#include <iostream>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/SharedPtr.h"
#include "Poco/Version.h" // #define POCO_VERSION 0x01090000

namespace sal
{
    using namespace std;

    /// TODO: rename namespace object -> namespace data
    namespace object
    {
        /// register all the data types, covering all JSON types, to assist serialization
        // the attribute type enumeration used to identify the type of Attribute object being handled
        typedef enum
        {
            ATTR_NULL, ///!> empty (uninitialized, null state), JSON null type
            ATTR_INT8, ///!> JSON number type
            ATTR_INT16,
            ATTR_INT32,
            ATTR_INT64,
            ATTR_UINT8,
            ATTR_UINT16,
            ATTR_UINT32,
            ATTR_UINT64,
            ATTR_FLOAT32,
            ATTR_FLOAT64,
            ATTR_BOOL,   ///!> JSON bool type
            ATTR_STRING, ///!> JSON string type, UTF8 encoding assumed
            ATTR_ARRAY,
            // as there is element type member, these new array types may not necessary
            ATTR_INT8_ARRAY, ///!> JSON array type, with same element type
            ATTR_INT16_ARRAY,
            ATTR_INT32_ARRAY,
            ATTR_INT64_ARRAY,
            ATTR_UINT8_ARRAY,
            ATTR_UINT16_ARRAY,
            ATTR_UINT32_ARRAY,
            ATTR_UINT64_ARRAY,
            ATTR_FLOAT32_ARRAY,
            ATTR_FLOAT64_ARRAY,
            ATTR_STRING_ARRAY, /// JSON array type for string
            ATTR_DICTIONARY,   /// JSON object type, container of children json types
        } AttributeType;

        /** attribute identifier strings in serialised objects
          those type name should equal to `numpy.typename`
          see: https://numpy.org/devdocs/user/basics.types.html
          Type data are not organized into `std::map<AttributeType, std::string>`,
          because they can be shared by both C adn C++.
        */
        char TYPE_NAME_NULL[] = "null";
        char TYPE_NAME_INT8[] = "int8";
        char TYPE_NAME_INT16[] = "int16";
        char TYPE_NAME_INT32[] = "int32";
        char TYPE_NAME_INT64[] = "int64";
        char TYPE_NAME_UINT8[] = "uint8";
        char TYPE_NAME_UINT16[] = "uint16";
        char TYPE_NAME_UINT32[] = "uint32";
        char TYPE_NAME_UINT64[] = "uint64";
        char TYPE_NAME_FLOAT32[] = "float32";
        char TYPE_NAME_FLOAT64[] = "float64";
        char TYPE_NAME_BOOL[] = "bool";
        char TYPE_NAME_STRING[] = "string";
        char TYPE_NAME_ARRAY[] = "array"; /// has extra class member element_type_name
        char TYPE_NAME_DICTIONARY[] = "dictionary";

        // forward declaratoin
        class Dictionary;

        /// why call Attribute? (data entry), Node class has a dictionary of Attributes
        /// The base data class (without any data) should also have a Type and TypeName
        // Todo: rename to DataObject to align with Python implementation
        // Todo: implement metadata registration to simulate python decorator
        class Attribute
        {
        public:
            typedef Poco::SharedPtr<Attribute> Ptr;

            /*
            Constructors and destructor.
            */
            // TODO: needs copy and move constructors, will not them be generated?
            Attribute(const AttributeType _type, const string _type_name)
                    : m_type(_type)
                    , m_typeName(_type_name){};
            virtual ~Attribute(){};

            /// from Attribute instance to json, return `Poco::JSON::Object::Ptr`
            virtual Poco::JSON::Object::Ptr encode() = 0;
            // TODO: void* data_pointer() = 0;  // for C-API, C user will cast to typed pointer

            // consider: method name should be type_name()
            inline const std::string& type_name() const
            {
                return m_typeName;
            }

            inline const AttributeType& type_id() const
            {
                return m_type;
            }

            // consider: method name should be isNull()
            inline bool is_null() const
            {
                return m_type == ATTR_NULL;
            }
            // is bool regarded as scalar?
            inline bool is_scalar() const
            {
                return not(is_array() or is_string() or is_null() or is_object());
            }

            inline bool is_atomic() const
            {
                return not(is_array() or is_object());
            }

            inline bool is_array() const
            {
                return m_type == ATTR_ARRAY;
            }

            inline bool is_string() const
            {
                return m_type == ATTR_STRING;
            }

            inline bool is_object() const
            {
                return m_type == ATTR_DICTIONARY;
            }

        protected:
            const AttributeType m_type;
            string m_typeName;
        };

        /// this class can be merged to the base class  It is a design decision
        /// do not merge, so the base class can not be instantiated
        /// to avoid silly mistake by API users. Note, C++ json

        /// content is optional, null is used inplace of missing content.
        /// corresponding to unintialized json (empty, null state).
        /// there is no need for a decode static function
        class Null : public Attribute
        {
        public:
            typedef Poco::SharedPtr<Null> Ptr;

            Null()
                    : Attribute(ATTR_NULL, "null"){};
            virtual ~Null(){};

            virtual Poco::JSON::Object::Ptr encode() override
            {
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                json->set("type", this->type_name());
                Poco::Dynamic::Var v;
                json->set("value", v);
                // json->stringify(cout, 4);
                return json;
            };
        };

        /*
        Data Object for Scalar atomic types (JSON number types)
        CONSIDER: rename Scalar into Atomic
        */
        template <class T, AttributeType TYPE, char const* TYPE_NAME> class Atomic : public Attribute
        {
            // TODO: add documentation
            T m_value;

        public:
            typedef Poco::SharedPtr<Atomic<T, TYPE, TYPE_NAME>> Ptr;
            /*
            Constructors and destructor.
            */
            Atomic()
                    : Attribute(TYPE, TYPE_NAME)
                    , m_value(T()){}; // using T() as the default value is more general
            Atomic(T _value)
                    : Attribute(TYPE, TYPE_NAME)
                    , m_value(_value){};
            virtual ~Atomic(){};


            inline T value() const
            {
                return m_value;
            }

            inline T& value()
            {
                return m_value;
            }
            /*
            Returns a Poco JSON object representation of the Scalar.
            */
            virtual Poco::JSON::Object::Ptr encode() override
            {
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                json->set("type", this->type_name());
                json->set("value", this->m_value);
                return json;
            };

            /*
            Decodes a Poco JSON object representation of the Scalar and returns a Scalar object.
            */
            static typename Atomic<T, TYPE, TYPE_NAME>::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                // treat any failure as a failure to decode
                try
                {
                    // check sal type is valid for this class
                    if (json->getValue<string>("type") != string(TYPE_NAME))
                        throw std::exception();
                    return new Atomic<T, TYPE, TYPE_NAME>(json->getValue<T>("value"));
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL scalar attribute.");
                }
            };
        };


        typedef Atomic<int8_t, ATTR_INT8, TYPE_NAME_INT8> Int8;
        typedef Atomic<int16_t, ATTR_INT16, TYPE_NAME_INT16> Int16;
        typedef Atomic<int32_t, ATTR_INT32, TYPE_NAME_INT32> Int32;
        typedef Atomic<int64_t, ATTR_INT64, TYPE_NAME_INT64> Int64;

        typedef Atomic<uint8_t, ATTR_UINT8, TYPE_NAME_UINT8> UInt8;
        typedef Atomic<uint16_t, ATTR_UINT16, TYPE_NAME_UINT16> UInt16;
        typedef Atomic<uint32_t, ATTR_UINT32, TYPE_NAME_UINT32> UInt32;
        typedef Atomic<uint64_t, ATTR_UINT64, TYPE_NAME_UINT64> UInt64;

        typedef Atomic<float, ATTR_FLOAT32, TYPE_NAME_FLOAT32> Float32;
        typedef Atomic<double, ATTR_FLOAT64, TYPE_NAME_FLOAT64> Float64;
        typedef Atomic<bool, ATTR_BOOL, TYPE_NAME_BOOL> Bool;
        typedef Atomic<std::string, ATTR_STRING, TYPE_NAME_STRING> String;

        /*
        It is a multi-dimension array based on std::vector<T>
        No default constructor without parameter is allowed,
        so shape of the array, as std::vector<uint64_t>,  consistent with python numpy.array
        */
        template <class T, AttributeType TYPE, char const* TYPE_NAME> class Array : public Attribute
        {
        public:
            typedef Poco::SharedPtr<Array<T, TYPE, TYPE_NAME>> Ptr;

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
            Array(vector<uint64_t> _shape)
                    : Attribute(ATTR_ARRAY, TYPE_NAME_ARRAY)
                    , m_element_type(TYPE)
                    , m_element_type_name(TYPE_NAME)
            {

                this->m_dimension = _shape.size();
                this->m_shape = _shape;
                this->m_strides.resize(this->m_dimension);

                // calculate strides
                int16_t i = this->m_dimension - 1;
                this->m_strides[i] = 1;
                i--;
                while (i >= 0)
                {
                    this->m_strides[i] = this->m_strides[i + 1] * this->m_shape[i + 1];
                    i--;
                }

                // calculate array buffer length
                uint64_t element_size = 1;
                for (uint64_t d : this->m_shape)
                    element_size *= d;

                // create buffer, how about set a default value?
                this->data.resize(element_size);
            };

            // CONSIDER: disable those constructors
            //            Array(const Array&);
            //            Array& operator= (const Array&);
            //            Array(Array&&);
            //            Array& operator= (Array&&);

            /*
            Destructor
            */
            virtual ~Array(){};

            /*
            Returns the length of the array buffer, element_size, not byte size
            flattened 1D array from all dimensions
            */
            inline uint64_t size() const
            {
                return this->data.size();
            };
#if 0
            inline size_t byte_size() const
            {
                return this->data.size() * size_of(T);
            };
#endif
            inline vector<uint64_t> shape() const
            {
                return this->m_shape;
            };
            /// consider: plural name
            inline size_t dimension() const
            {
                return this->m_shape.size();
            };

            AttributeType element_type() const
            {
                return m_element_type;
            }

            string element_type_name() const
            {
                return m_element_type_name;
            }

            /*
            Fast element access via direct indexing of the array buffer (flattened ID array).

            The Array holds the data in a 1D strided array. Indexing into
            multidimensional arrays therefore requires the user to
            appropriately stride across the data. See the stride attribute.

            No bounds checking is performed.
            */
            inline T& operator[](const uint64_t index)
            {
                return this->data[index];
            };

            inline const T& operator[](const uint64_t index) const
            {
                return this->data[index];
            };

                // C++14 provide <T indices ...>
#if 1
            /// quick access an element for 2D matrix row and col,  without bound check
            /// array(row, col), all zero for the first
            inline T& operator()(const uint64_t row, const uint64_t column)
            {
                uint64_t index = row * this->m_strides[0] + column;
                return this->data[index];
            };

            inline const T& operator()(const uint64_t row, const uint64_t column) const
            {
                uint64_t index = row * this->m_strides[0] + column;
                return this->data[index];
            };
#endif

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
            const T& at(int i0, int64_t i1 = -1, int64_t i2 = -1, int64_t i3 = -1, int64_t i4 = -1, int64_t i5 = -1,
                        int64_t i6 = -1, int64_t i7 = -1, int64_t i8 = -1, int64_t i9 = -1) const throw()
            {

                if (this->m_dimension > 10)
                {
                    throw out_of_range("The at() method can only be used with arrays of 10 dimensions of less.");
                }

                // convert the list or arguments to an array for convenience
                array<int64_t, 10> dim_index = {i0, i1, i2, i3, i4, i5, i6, i7, i8, i9};

                uint64_t element_index = 0;
                for (uint8_t i = 0; i < this->m_dimension; i++)
                {

                    // check the indices are inside the array bounds
                    if ((dim_index[i] < 0) || (dim_index[i] > this->m_shape[i] - 1))
                    {
                        throw out_of_range("An array index is missing or is out of bounds.");
                    }

                    element_index += dim_index[i] * this->m_strides[i];
                }

                return this->data[element_index];
            }

            /*
            Returns a Poco JSON object representation of the Array.
            */
            virtual Poco::JSON::Object::Ptr encode() override
            {
                // not complete  array_definition is empty
                Poco::JSON::Object::Ptr array_definition = new Poco::JSON::Object();
                Poco::JSON::Object::Ptr json_obj = new Poco::JSON::Object();

                array_definition->set("type", this->m_element_type_name);
                array_definition->set("shape", this->encode_shape());
                array_definition->set("encoding", "base64");
                array_definition->set("data", this->encode_data());

                json_obj->set("type", this->type_name());
                json_obj->set("value", array_definition);

                return json_obj;
            };

            /*
            Decodes a Poco JSON object representation of the Array and returns an Array object.
            */
            static typename Array<T, TYPE, TYPE_NAME>::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                Poco::JSON::Object::Ptr array_definition;
                vector<uint64_t> shape;
                string encoded_data;
                typename Array<T, TYPE, TYPE_NAME>::Ptr array;

                // treat any failure as a failure to decode
                try
                {

                    // check sal type is valid for this class
                    if (json->getValue<string>("type") != string(TYPE_NAME_ARRAY))
                        throw std::exception();

                    // extract array definition
                    array_definition = json->getObject("value");

                    // check array element type and array encoding are valid for this class
                    if (array_definition->getValue<string>("type") != string(TYPE_NAME))
                        throw std::exception();
                    if (array_definition->getValue<string>("encoding") != string("base64"))
                        throw std::exception();
                    if (!array_definition->isArray("shape"))
                        throw std::exception();

                    // decode shape
                    shape = Array<T, TYPE, TYPE_NAME>::decode_shape(array_definition->getArray("shape"));

                    // create and populate array
                    array = new Array<T, TYPE, TYPE_NAME>(shape);
                    Array<T, TYPE, TYPE_NAME>::decode_data(array->data, array_definition->getValue<string>("data"));
                    return array;
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                }
            };

        protected:
            const string m_element_type_name;
            const AttributeType m_element_type;
            uint8_t m_dimension; // CONSIDER: size_t otherwise lots of compiler warning
            vector<uint64_t> m_shape;
            vector<uint64_t> m_strides;
            vector<T> data;

            /*
            Converts the shape array to a POCO JSON array object.
            */
            Poco::JSON::Array::Ptr encode_shape() const
            {
                Poco::JSON::Array::Ptr shape = new Poco::JSON::Array();
                for (uint8_t i = 0; i < this->m_shape.size(); i++)
                    shape->add(this->m_shape[i]);
                return shape;
            };

            /*
            Decodes the shape array from a POCO JSON array object.
            */
            static vector<uint64_t> decode_shape(Poco::JSON::Array::Ptr encoded)
            {
                vector<uint64_t> shape(encoded->size());
                for (uint8_t i = 0; i < encoded->size(); i++)
                    shape[i] = encoded->getElement<uint64_t>(i);
                return shape;
            };

            /*
            Encodes the data buffer as a base64 string.
            */
            const string encode_data() //  reinterpret_cast stop mark this f as const
            {
                stringstream s;
#if POCO_VERSION >= 0x01080000
                // Poco::BASE64_URL_ENCODING is a enum, with value 1
                Poco::Base64Encoder encoder(s, Poco::BASE64_URL_ENCODING);
#else
                // POCO 1.6 on Centos7 does not have such
                Poco::Base64Encoder encoder(s);
#endif
                // TODO: why not reinterpret_cast<unsigned char*>
                encoder.write(reinterpret_cast<char*>(this->data.data()), this->data.size() * sizeof(T));
                encoder.close();
                return s.str();
            };

            /*
            Decodes the data buffer from a base64 string.
            */
            static void decode_data(vector<T>& data, const string b64)
            {
                stringstream s(b64);
#if POCO_VERSION >= 0x01080000
                // Poco::BASE64_URL_ENCODING is a enum, with value 1
                Poco::Base64Decoder decoder(s, Poco::BASE64_URL_ENCODING);
#else
                // POCO 1.6 on Centos7 does not have Poco::BASE64_URL_ENCODING enum
                Poco::Base64Decoder decoder(s);
#endif
                decoder.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(T));
            };
        };

        /// naming as Javascript TypedArray
        typedef Array<int8_t, ATTR_INT8, TYPE_NAME_INT8> Int8Array;
        typedef Array<int16_t, ATTR_INT16, TYPE_NAME_INT16> Int16Array;
        typedef Array<int32_t, ATTR_INT32, TYPE_NAME_INT32> Int32Array;
        typedef Array<int64_t, ATTR_INT64, TYPE_NAME_INT64> Int64Array;

        typedef Array<uint8_t, ATTR_UINT8, TYPE_NAME_UINT8> UInt8Array;
        typedef Array<uint16_t, ATTR_UINT16, TYPE_NAME_UINT16> UInt16Array;
        typedef Array<uint32_t, ATTR_UINT32, TYPE_NAME_UINT32> UInt32Array;
        typedef Array<uint64_t, ATTR_UINT64, TYPE_NAME_UINT64> UInt64Array;

        typedef Array<float, ATTR_FLOAT32, TYPE_NAME_FLOAT32> Float32Array;
        typedef Array<double, ATTR_FLOAT64, TYPE_NAME_FLOAT64> Float64Array;

        typedef Array<bool, ATTR_BOOL, TYPE_NAME_BOOL> BoolArray;
#if 1
        typedef Array<std::string, ATTR_STRING, TYPE_NAME_STRING> StringArray;

#else
        /*
        Data Object String Array Attribute
        // TODO: use Array<std::string> instead
        derived from  Array<std::string, ATTR_STRING, TYPE_NAME_STRING>
        */
        class StringArray : public Attribute
        {

        public:
            typedef Poco::SharedPtr<StringArray> Ptr;

            /*
            StringArray constructor.

            Initialises an array with the specified dimensions (shape). The
            array shape is a vector defining the length of each dimensions
            of the array. The number of elements in the shape vector
            defines the number of dimensions.
            */
            StringArray(vector<uint64_t> shape)
                    : Attribute(ATTR_ARRAY, TYPE_NAME_ARRAY)
                    , element_type_name(TYPE_NAME_STRING)
            {

                this->dimensions = shape.size();
                this->shape = shape;
                this->stride.resize(this->dimensions);

                // calculate strides
                int16_t i = this->dimensions - 1;
                this->stride[i--] = 1;
                while (i >= 0)
                {
                    this->stride[i] = this->stride[i + 1] * this->shape[i + 1];
                    i--;
                }

                // calculate array buffer length
                uint64_t size = 1;
                for (uint64_t d : this->shape)
                    size *= d;

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
            virtual ~StringArray(){};

            /*
            Returns the length of the array buffer.
            */
            uint64_t size() const
            {
                return this->data.size();
            };

            /*
            Fast element access via direct indexing of the array buffer.

            The Array holds the data in a 1D strided array. Indexing into
            multidimensional arrays therefore requires the user to
            appropriately stride across the data. See the stride attribute.

            No bounds checking is performed.
            */
            string& operator[](uint64_t index)
            {
                return this->data[index];
            };

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
            string& at(int i0, int64_t i1 = -1, int64_t i2 = -1, int64_t i3 = -1, int64_t i4 = -1, int64_t i5 = -1,
                       int64_t i6 = -1, int64_t i7 = -1, int64_t i8 = -1, int64_t i9 = -1)
            {

                if (this->dimensions > 10)
                {
                    throw out_of_range("The at() method can only be used with arrays of 10 dimensions of less.");
                }

                // convert the list or arguments to an array for convenience
                array<int64_t, 10> dim_index = {i0, i1, i2, i3, i4, i5, i6, i7, i8, i9};

                uint64_t element_index = 0;
                for (uint8_t i = 0; i < this->dimensions; i++)
                {

                    // check the indices are inside the array bounds
                    if ((dim_index[i] < 0) || (dim_index[i] > this->shape[i] - 1))
                    {
                        throw out_of_range("An array index is missing or is out of bounds.");
                    }

                    element_index += dim_index[i] * this->stride[i];
                }

                return this->data[element_index];
            }

            /*
            Returns a Poco JSON object representation of the Array.
            */
            Poco::JSON::Object::Ptr encode()
            {

                Poco::JSON::Object::Ptr array_definition = new Poco::JSON::Object();
                Poco::JSON::Object::Ptr attribute = new Poco::JSON::Object();

                array_definition->set("type", this->element_type_name);
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
            static StringArray::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                Poco::JSON::Object::Ptr array_definition;
                vector<uint64_t> shape;
                string encoded_data;
                typename StringArray::Ptr array;

                // treat any failure as a failure to decode
                try
                {

                    // check sal type is valid for this class
                    if (json->getValue<string>("type") != string(TYPE_NAME_ARRAY))
                        throw std::exception();

                    // extract array definition
                    array_definition = json->getObject("value");

                    // check array element type and array encoding are valid for this class
                    if (array_definition->getValue<string>("type") != string(TYPE_NAME_STRING))
                        throw std::exception();
                    if (array_definition->getValue<string>("encoding") != string("list"))
                        throw std::exception();
                    if (!array_definition->isArray("shape"))
                        throw std::exception();
                    if (!array_definition->isArray("data"))
                        throw std::exception();

                    // decode shape
                    shape = StringArray::decode_shape(array_definition->getArray("shape"));

                    // create and populate array
                    array = new StringArray(shape);
                    StringArray::decode_data(array, array_definition->getArray("data"));
                    return array;
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL Array attribute.");
                }
            };

        protected:
            const string element_type_name;
            uint8_t dimensions;
            vector<uint64_t> shape;
            vector<uint64_t> stride;
            vector<string> data;

            /*
            Converts the shape array to a POCO JSON array object.
            */
            Poco::JSON::Array::Ptr encode_shape()
            {
                Poco::JSON::Array::Ptr json = new Poco::JSON::Array();
                for (uint8_t i = 0; i < this->shape.size(); i++)
                    json->add(this->shape[i]);
                return json;
            };

            /*
            Decodes the shape array from a POCO JSON array object.
            */
            static vector<uint64_t> decode_shape(Poco::JSON::Array::Ptr json)
            {
                vector<uint64_t> shape(json->size());
                for (uint8_t i = 0; i < json->size(); i++)
                    shape[i] = json->getElement<uint64_t>(i);
                return shape;
            };

            /*
            Encodes the string vector as a nested Poco Array of strings.
            */
            Poco::JSON::Array::Ptr encode_data(uint8_t dimension = 0, uint64_t offset = 0)
            {

                Poco::JSON::Array::Ptr json = new Poco::JSON::Array();

                if (dimension == (this->dimensions - 1))
                {

                    // populate innermost array with the strings
                    for (uint64_t i = 0; i < this->shape[dimension]; i++)
                    {
                        json->add(this->data[offset + i]);
                    }
                }
                else
                {

                    // create nested array objects
                    for (uint64_t i = 0; i < this->shape[dimension]; i++)
                    {
                        json->add(encode_data(dimension + 1, offset + i * this->stride[dimension]));
                    }
                }
                return json;
            };

            /*
            Decodes a nested Poco Array into a string vector.
            */
            static void decode_data(StringArray::Ptr array, const Poco::JSON::Array::Ptr json, uint8_t dimension = 0,
                                    uint64_t offset = 0)
            {

                if (dimension == (array->dimensions - 1))
                {

                    // innermost array contains strings
                    for (uint64_t i = 0; i < array->shape[dimension]; i++)
                    {
                        array->data[offset + i] = json->getElement<string>(i);
                    }
                }
                else
                {

                    // decode nested array objects
                    for (uint64_t i = 0; i < array->shape[dimension]; i++)
                    {
                        decode_data(array, json->getArray(i), dimension + 1, offset + i * array->stride[dimension]);
                    }
                }
            };
        };
#endif

        // forward declare decode()
        Attribute::Ptr decode(Poco::JSON::Object::Ptr json);


        /*
        a container of string key and Attribute type data
        Refactored: rename from Branch to Dictionary, corresponding to python sal.data.dictionary
        */
        class Dictionary : public Attribute
        {

        public:
            typedef Poco::SharedPtr<Dictionary> Ptr;

            /*
            Constructors and destructor.
            */
            Dictionary()
                    : Attribute(ATTR_DICTIONARY, TYPE_NAME_DICTIONARY){};
            virtual ~Dictionary(){};

            // TODO: better exception handling
            // TODO: add documentation
            Attribute::Ptr& operator[](const string& key)
            {
                return this->attributes.at(key);
            };
            Attribute::Ptr& get(const string& key)
            {
                return (*this)[key];
            };
            template <class T> typename T::Ptr get_as(const string& key)
            {
                return typename T::Ptr(this->get(key).cast<T>());
            };
            void set(const string& key, const Attribute::Ptr& attribute)
            {
                this->attributes[key] = attribute;
            };
            const bool has(const string& key) const
            {
                return this->attributes.count(key);
            };
            void remove(const string& key)
            {
                this->attributes.erase(key);
            };

            /*
            Returns a Poco JSON object representation of the Branch.
            */
            virtual Poco::JSON::Object::Ptr encode() override
            {

                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                Poco::JSON::Object::Ptr value = new Poco::JSON::Object();

                // encode each attribute
                for (map<string, Attribute::Ptr>::iterator i = this->attributes.begin(); i != this->attributes.end();
                     ++i)
                    value->set(i->first, i->second->encode());

                json->set("type", this->type_name());
                json->set("value", value);
                return json;
            };

            /*
            Decodes a Poco JSON object representation of the Dictionary attribute.
            */
            static Dictionary::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                Poco::JSON::Object::Ptr contents;
                vector<string> keys;
                vector<string>::iterator key;
                Dictionary::Ptr container;

                // treat any failure as a failure to decode
                try
                {

                    // check sal type is valid for this class
                    if (json->getValue<string>("type") != TYPE_NAME_DICTIONARY)
                        throw std::exception();

                    // extract array definition
                    contents = json->getObject("value");

                    // create container object and populate
                    container = new Dictionary();
                    contents->getNames(keys);
                    for (key = keys.begin(); key != keys.end(); ++key)
                    {

                        // CONSIDER: Null data object is ready, still skip null elements?
                        if (contents->isNull(*key))
                            continue;

                        // all valid attributes definitions are JSON objects
                        if (!contents->isObject(*key))
                            throw std::exception();

                        // dispatch object to the appropriate decoder
                        container->set(*key, sal::object::decode(contents->getObject(*key)));
                    }
                    return container;
                }
                catch (...)
                {
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
        Attribute::Ptr decode(Poco::JSON::Object::Ptr json)
        {
            // CONSIDER:  type enum ID is more efficient in comparison then type name
            string id;

            try
            {
                id = json->getValue<string>("type");
            }
            catch (...)
            {
                // todo: define a sal exception and replace
                throw runtime_error("JSON object does not define a valid SAL attribute.");
            }

            // containers
            if (id == TYPE_NAME_DICTIONARY)
                return Dictionary::decode(json);

            // atomic
            if (id == TYPE_NAME_INT8)
                return Int8::decode(json);
            if (id == TYPE_NAME_INT16)
                return Int16::decode(json);
            if (id == TYPE_NAME_INT32)
                return Int32::decode(json);
            if (id == TYPE_NAME_INT64)
                return Int64::decode(json);
            if (id == TYPE_NAME_UINT8)
                return UInt8::decode(json);
            if (id == TYPE_NAME_UINT16)
                return UInt16::decode(json);
            if (id == TYPE_NAME_UINT32)
                return UInt32::decode(json);
            if (id == TYPE_NAME_UINT64)
                return UInt64::decode(json);
            if (id == TYPE_NAME_FLOAT32)
                return Float32::decode(json);
            if (id == TYPE_NAME_FLOAT64)
                return Float64::decode(json);
            if (id == TYPE_NAME_BOOL)
                return Bool::decode(json);
            if (id == TYPE_NAME_STRING)
                return String::decode(json);

            // arrays
            if (id == TYPE_NAME_ARRAY)
            {

                Poco::JSON::Object::Ptr array_definition;
                string element_id;

                try
                {
                    array_definition = json->getObject("value");
                    element_id = array_definition->getValue<string>("type");
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL attribute.");
                }

                if (element_id == TYPE_NAME_INT8)
                    return Int8Array::decode(json);
                if (element_id == TYPE_NAME_INT16)
                    return Int16Array::decode(json);
                if (element_id == TYPE_NAME_INT32)
                    return Int32Array::decode(json);
                if (element_id == TYPE_NAME_INT64)
                    return Int64Array::decode(json);
                if (element_id == TYPE_NAME_UINT8)
                    return UInt8Array::decode(json);
                if (element_id == TYPE_NAME_UINT16)
                    return UInt16Array::decode(json);
                if (element_id == TYPE_NAME_UINT32)
                    return UInt32Array::decode(json);
                if (element_id == TYPE_NAME_UINT64)
                    return UInt64Array::decode(json);
                if (element_id == TYPE_NAME_FLOAT32)
                    return Float32Array::decode(json);
                if (element_id == TYPE_NAME_FLOAT64)
                    return Float64Array::decode(json);
                if (element_id == TYPE_NAME_STRING)
                    return StringArray::decode(json);
            }

            // todo: define a sal exception and replace
            throw runtime_error("JSON object does not define a valid SAL attribute.");
        }


        /*
        Attempts to decode a JSON object into the specified SAL object attribute.

        Returns null pointer if cast is invalid.
        */
        template <class T> typename T::Ptr decode_as(Poco::JSON::Object::Ptr json)
        {
            return typename T::Ptr(decode(json).cast<T>());
        };
    } // namespace object
} // namespace sal