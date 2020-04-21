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

#include "sal_exception.h"

namespace sal
{
    // using namespace std;
    using namespace exception;

    /// TODO: rename namespace object -> namespace data
    namespace object
    {
        /// register all the data types, covering all JSON types, to assist serialization
        /// Type data can be registered to a static data store `std::map<AttributeType, std::string>`
        /// at compiling time.

        /// C language comptable
        // the attribute type enumeration used to identify the type of Attribute object being handled
        typedef enum
        {
            ATTR_NULL,   ///!> empty (uninitialized, null state), JSON null type
            ATTR_SCALAR, ///!> JSON scalar number type + boolean = SAL scalar class
            // consider: split dtype out as DTYPE
            ATTR_INT8, ///!> JSON scalar number type
            ATTR_INT16,
            ATTR_INT32,
            ATTR_INT64,
            ATTR_UINT8,
            ATTR_UINT16,
            ATTR_UINT32,
            ATTR_UINT64,
            ATTR_FLOAT32,
            ATTR_FLOAT64,
            ATTR_BOOL, ///!> JSON bool type

            ATTR_STRING, ///!> JSON string type, UTF8 encoding assumed
            ATTR_ARRAY,  ///!> JSON array type, with same element type
#if 0
            // as there is element type member, these new array types may not necessary
            ATTR_INT8_ARRAY, 
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
#endif
            ATTR_DICTIONARY, ///!> JSON object type, container of children json types
            ATTR_SIGNAL,     ///!> customized JSON object type for SAL physical pulse signal
        } AttributeType;

        /** attribute identifier strings in serialised objects
          those type name should equal to `numpy.typename`
          see: https://numpy.org/devdocs/user/basics.types.html
          because they can be shared by both C adn C++.
          why not const char*, maybe caused by Poco::JSON
        */
        char TYPE_NAME_NULL[] = "null";
        char TYPE_NAME_SCALAR[] = "scalar";
        // numpy.dtype 's typename
        // consider split dtype out as DTYPE_NAME
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
        char TYPE_NAME_SIGNAL[] = "signal";

        // forward declaratoin
        class Dictionary;

        /// abstract class corresponding to python DataSummary class.
        /// Attribute class (base data class) implements this interface,
        /// to avoid another summary class tree in parallel with data class tree as in Python
        /// static SummaryInterface::Ptr decode_summary(Poco::JSON::Object::Ptr json)
        class SummaryInterface
        {
        public:
            typedef Poco::SharedPtr<SummaryInterface> Ptr;
            virtual Poco::JSON::Object::Ptr encode_header() const = 0;
            virtual Poco::JSON::Object::Ptr encode_summary() const = 0;

            /// stringified json object returned from `encode_summary()`
            /// C API can get `const char*` string buffer
            /// consider: return std::shared_ptr<std::string> for better performance
            virtual std::string summary() const = 0;

        protected:
        };

        /// why call Attribute? (data entry), Node class has a dictionary of Attributes
        /// The base data class (without any data) should also have a Type and TypeName
        // Todo: rename to DataObject to align with Python implementation
        // Todo: implement metadata registration to simulate python decorator
        class Attribute : public SummaryInterface
        {
        public:
            typedef Poco::SharedPtr<Attribute> Ptr;

            // TODO: needs copy and move constructors, will not them be generated?

            /*
            Constructors and destructor.
            */
            Attribute(const AttributeType _type, const std::string _type_name)
                    : m_type(_type)
                    , m_type_name(_type_name){};
            virtual ~Attribute(){};

            /// from Attribute instance to json, return `Poco::JSON::Object::Ptr`
            virtual Poco::JSON::Object::Ptr encode() const = 0;

            /// consider: this member method and variable name should be class_name(),
            /// map to python class member variable `CLASS`
            inline const std::string& type_name() const noexcept
            {
                return m_type_name;
            }

            /// no corresponding on python side, maybe useful to
            inline const AttributeType& type_id() const noexcept
            {
                return m_type;
            }

            // CONSIDER buffer pointer interface for C-API, currently implemented for Array only
            // void* data_pointer() = 0;


            /// @{ SummaryInterface
            /// for complex data type, there is no data available
            /// is_summary() true if the instance is created by decode_summary()
            bool is_summary() const noexcept
            {
                return m_is_summary;
            };

            inline const std::string& description() const noexcept
            {
                if (m_description.size())
                    return m_description;
                else
                {
                    return m_type_name;
                }
            }
            /// description property setter
            inline std::string& description() noexcept
            {
                return m_description;
            }

            /// core.dataclass._new_dict() with common header info
            virtual Poco::JSON::Object::Ptr encode_header() const override final
            {
                /// constant members from python DataObject ReportSummary
                const static std::string GROUP = "core";
                // static member init of std::string is not supported
                const static uint64_t VERSION = 1;

                // d['_type'] = TYPE_SUMMARY
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

                // to be python compatible, remove this block after further refactoring,
                if (is_number())
                {
                    json->set("_class", "scalar");
                }
                else
                    json->set("_class", this->type_name());

                json->set("_group", GROUP);
                json->set("_version", VERSION);
                json->set("description", this->m_description);
                return json;
            }

            /*
            Returns a Poco JSON object summary of the data object.
            corresponding to python DataSummary class `to_dict()`
            only container derived class like Array and Dictionary need to override
            */
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                auto json = encode_header();
                // only if value is small, put into summary
                if (is_atomic())
                {
                    const auto j = encode();
                    json->set("value", j->get("value"));
                }
                return json;
            }

            virtual std::string summary() const override
            {
                std::stringstream ss;
                auto jp = encode_summary(); // TODO: check if derived virtual function invoked?
                jp->stringify(ss);
                return ss.str();
            }
            /// @}


            /// @{
            inline bool is_null() const noexcept
            {
                return m_type == ATTR_NULL;
            }
            // bool is not regarded as a scalar
            inline bool is_number() const noexcept
            {
                return not(is_array() or is_string() or is_boolean() or is_null() or is_object());
            }
            // is bool regarded as scalar?
            inline bool is_boolean() const noexcept
            {
                return m_type == ATTR_BOOL;
            }
            inline bool is_atomic() const noexcept
            {
                return not(is_array() or is_object() or is_null());
            }

            inline bool is_array() const noexcept
            {
                return m_type == ATTR_ARRAY;
            }

            inline bool is_string() const noexcept
            {
                return m_type == ATTR_STRING;
            }

            inline bool is_object() const noexcept
            {
                return m_type == ATTR_DICTIONARY;
            }
            /// @}

        protected:
            const AttributeType m_type;
            std::string m_type_name; // CLASS is the typname, it is different for types

            /// member for summary interface
            bool m_is_summary;
            std::string m_description;
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

            virtual Poco::JSON::Object::Ptr encode() const override
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
        REFACTORED: rename Scalar<> into Atomic<>
        TODO: std::atomic<> to make them atomic as the name suggested
        CONSIDER: type_name (CLASS name) == SCALAR, to be consistent with python
        */
        template <class T, AttributeType DTYPE, char const* DTYPE_NAME> class Atomic : public Attribute
        {
            T m_value;

        public:
            typedef Poco::SharedPtr<Atomic<T, DTYPE, DTYPE_NAME>> Ptr;
            /*
            Constructors and destructor.
            */
            Atomic()
                    : Attribute(DTYPE, DTYPE_NAME)
                    , m_value(T()){}; // using T() as the default value is more general
            Atomic(T _value)
                    : Attribute(DTYPE, DTYPE_NAME)
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
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                json->set("type", this->type_name());
                json->set("value", this->m_value);
                return json;
            };

            /*
            Decodes a Poco JSON object representation of the Scalar and returns a Scalar object.
            */
            static typename Atomic<T, DTYPE, DTYPE_NAME>::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                // treat any failure as a failure to decode
                try
                {
                    // check sal type is valid for this class
                    if (json->getValue<std::string>("type") != std::string(DTYPE_NAME))
                        throw std::exception();
                    return new Atomic<T, DTYPE, DTYPE_NAME>(json->getValue<T>("value"));
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL scalar attribute.");
                }
            };
        };

        /// TODO: using scalar as CLASS type, map to python sal implementation
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

        // TODO: specialization String to have different CLASS name
        // std::string is also not support atomic operation
        typedef Atomic<std::string, ATTR_STRING, TYPE_NAME_STRING> String;

        /// a typedef to ease future refactoring on data structure
        using ShapeType = std::vector<uint64_t>;

        /*
        It is a multi-dimension array based on std::vector<T>
        No default constructor without parameter is allowed,
        so shape of the array, as std::vector<uint64_t>,  consistent with python numpy.array
        */
        template <class T, AttributeType ELEMENT_TYPE, char const* ELEMENT_TYPE_NAME> class Array : public Attribute
        {
        public:
            typedef Poco::SharedPtr<Array<T, ELEMENT_TYPE, ELEMENT_TYPE_NAME>> Ptr;

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
            Array(ShapeType _shape)
                    : Attribute(ATTR_ARRAY, TYPE_NAME_ARRAY)
                    , m_element_type(ELEMENT_TYPE)
                    , m_element_type_name(ELEMENT_TYPE_NAME)
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

            // CONSIDER: disable those constructors, force shared_ptr<>
            //            Array(const Array&);
            //            Array& operator= (const Array&);
            //            Array(Array&&);
            //            Array& operator= (Array&&);

            /*
            virtual destructor
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

            inline ShapeType shape() const
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

            std::string element_type_name() const
            {
                return m_element_type_name;
            }

            /// {@
            /** infra-structure for C-API */
            inline size_t byte_size() const
            {
                return this->data.size() * sizeof(T);
            };

            /// read-only pointer to provide read view into the data buffer
            const void* data_pointer() const
            {
                return this->data.data();
            }

            /// modifiable raw pointer to data buffer, use it with care
            void* data_pointer()
            {
                return this->data.data();
            }
            /// @}


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
                    throw std::out_of_range("The at() method can only be used with arrays of 10 dimensions of less.");
                }

                // convert the list or arguments to an array for convenience
                std::array<int64_t, 10> dim_index = {i0, i1, i2, i3, i4, i5, i6, i7, i8, i9};

                uint64_t element_index = 0;
                for (uint8_t i = 0; i < this->m_dimension; i++)
                {

                    // check the indices are inside the array bounds
                    if ((dim_index[i] < 0) || (dim_index[i] > this->m_shape[i] - 1))
                    {
                        throw std::out_of_range("An array index is missing or is out of bounds.");
                    }

                    element_index += dim_index[i] * this->m_strides[i];
                }

                return this->data[element_index];
            }

            /*
            Returns a Poco JSON object summary of the Array.
            */
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                auto json_obj = encode_header();
                // to be consistent with python side, no value field
                json_obj->set("shape", this->encode_shape());
                // new info, only for C++, but comptable with python
                json_obj->set("element_type", this->m_element_type_name);
                return json_obj;
            };

            /*
            Returns a Poco JSON object representation of the Array.
            */
            virtual Poco::JSON::Object::Ptr encode() const override
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
            static typename Array<T, ELEMENT_TYPE, ELEMENT_TYPE_NAME>::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                Poco::JSON::Object::Ptr array_definition;
                ShapeType shape;
                std::string encoded_data;
                typename Array<T, ELEMENT_TYPE, ELEMENT_TYPE_NAME>::Ptr array;

                // treat any failure as a failure to decode
                try
                {

                    // check sal type is valid for this class
                    if (json->getValue<std::string>("type") != std::string(TYPE_NAME_ARRAY))
                        throw SALException("type does not match, array is expected here");

                    // extract array definition
                    array_definition = json->getObject("value");

                    // check array element type and array encoding are valid for this class
                    if (array_definition->getValue<std::string>("type") != std::string(ELEMENT_TYPE_NAME))
                        throw std::exception();
                    if (array_definition->getValue<std::string>("encoding") != std::string("base64"))
                        throw std::exception();
                    if (!array_definition->isArray("shape"))
                        throw std::exception();

                    // decode shape
                    shape =
                        Array<T, ELEMENT_TYPE, ELEMENT_TYPE_NAME>::decode_shape(array_definition->getArray("shape"));

                    // create and populate array
                    array = new Array<T, ELEMENT_TYPE, ELEMENT_TYPE_NAME>(shape);
                    Array<T, ELEMENT_TYPE, ELEMENT_TYPE_NAME>::decode_data(
                        array->data, array_definition->getValue<std::string>("data"));
                    return array;
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL Array attribute.");
                }
            };

        protected:
            const std::string m_element_type_name;
            const AttributeType m_element_type;
            uint8_t m_dimension; // CONSIDER: size_t otherwise lots of compiler warning
            ShapeType m_shape;
            ShapeType m_strides;
            std::vector<T> data;

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
            static ShapeType decode_shape(Poco::JSON::Array::Ptr encoded)
            {
                ShapeType shape(encoded->size());
                for (uint8_t i = 0; i < encoded->size(); i++)
                    shape[i] = encoded->getElement<uint64_t>(i);
                return shape;
            };

            /*
            Encodes the data buffer as a base64 string.
            */
            const std::string encode_data() const //  reinterpret_cast stop mark this f as const
            {
                std::stringstream s;
#if POCO_VERSION >= 0x01080000
                // Poco::BASE64_URL_ENCODING is a enum, with value 1
                Poco::Base64Encoder encoder(s, Poco::BASE64_URL_ENCODING);
#else
                // POCO 1.6 on Centos7 does not have such
                Poco::Base64Encoder encoder(s);
#endif
                encoder.write(reinterpret_cast<const char*>(this->data.data()), this->data.size() * sizeof(T));
                encoder.close();
                return s.str();
            };

            /*
            Decodes the data buffer from a base64 string.
            TODO: const data?
            */
            static void decode_data(std::vector<T>& data, const std::string b64)
            {
                std::stringstream s(b64);
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
                    if (json->getValue<std::string>("type") != string(TYPE_NAME_ARRAY))
                        throw std::exception();

                    // extract array definition
                    array_definition = json->getObject("value");

                    // check array element type and array encoding are valid for this class
                    if (array_definition->getValue<std::string>("type") != string(TYPE_NAME_STRING))
                        throw std::exception();
                    if (array_definition->getValue<std::string>("encoding") != string("list"))
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
                    throw SALException("JSON object does not define a valid SAL Array attribute.");
                }
            };

        protected:
            const string element_type_name;
            uint8_t dimensions;
            vector<uint64_t> shape;
            vector<uint64_t> stride;
            vector<std::string> data;

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
                        array->data[offset + i] = json->getElement<std::string>(i);
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
            Attribute::Ptr& operator[](const std::string& key)
            {
                return this->attributes.at(key);
            };
            Attribute::Ptr& get(const std::string& key)
            {
                return (*this)[key];
            };
            template <class T> typename T::Ptr get_as(const std::string& key)
            {
                return typename T::Ptr(this->get(key).cast<T>());
            };
            void set(const std::string& key, const Attribute::Ptr& attribute)
            {
                this->attributes[key] = attribute;
            };
            const bool has(const std::string& key) const
            {
                return this->attributes.count(key);
            };
            void remove(const std::string& key)
            {
                this->attributes.erase(key);
            };

            /*
            Returns a Poco JSON object summary of the Array.
            */
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                auto json_obj = encode_header();
                // new info, only for C++, but comptable with python
                json_obj->set("count", this->attributes.size());
                // consider also write an array of keys
                return json_obj;
            };

            /*
            Returns a Poco JSON object representation of the Branch.
            */
            virtual Poco::JSON::Object::Ptr encode() const override
            {

                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                Poco::JSON::Object::Ptr value = new Poco::JSON::Object();

                // encode each attribute
                for (std::map<std::string, Attribute::Ptr>::const_iterator i = attributes.begin();
                     i != this->attributes.end(); ++i)
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
                std::vector<std::string> keys;
                std::vector<std::string>::iterator key;
                Dictionary::Ptr container;

                // treat any failure as a failure to decode
                try
                {

                    // check sal type is valid for this class
                    if (json->getValue<std::string>("type") != TYPE_NAME_DICTIONARY)
                        throw SALException("data type does not match");

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
                    throw SALException("JSON object does not define a valid SAL Array attribute.");
                }
            };

        protected:
            std::map<std::string, Attribute::Ptr> attributes;
        };

        /*
        Attempts to decode a JSON object into a SAL object attribute.
        */
        Attribute::Ptr decode(Poco::JSON::Object::Ptr json)
        {
            // CONSIDER:  type enum  is more efficient in comparison then type name
            std::string id;

            try
            {
                id = json->getValue<std::string>("type");
            }
            catch (...)
            {
                throw SALException("JSON object does not define a valid SAL attribute.");
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

            // TODO: split into another
            if (id == TYPE_NAME_ARRAY)
            {

                Poco::JSON::Object::Ptr array_definition;
                std::string element_id;

                try
                {
                    array_definition = json->getObject("value");
                    element_id = array_definition->getValue<std::string>("type");
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL attribute.");
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
            throw SALException("JSON object does not define a valid SAL attribute.");
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