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

static const uint64_t SAL_API_VERSION = 1;

namespace sal
{
    // using namespace std;
    using namespace exception;

    /// TODO: rename namespace object -> namespace data
    /// responding to pyton sal.dataclass.core module
    namespace object
    {
        /// register all the data types, covering all JSON types, to assist serialization
        /// Type data can be registered to a static data store `std::map<AttributeType, std::string>`
        /// at compiling time.

        /// the attribute type enumeration used to identify the type of Attribute object being handled
        /// https://en.wikipedia.org/wiki/OPC_Unified_Architecture#Built-in_data_types
        typedef enum // C language comptable
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

            ATTR_STRING,     ///!> JSON string type, UTF8 encoding assumed
            ATTR_ARRAY,      ///!> JSON array type, with same element type
            ATTR_DICTIONARY, ///!> JSON object type, container of children json types

            ATTR_SIGNAL, ///!> high level data model for SAL physical pulse signal
            // ATTR_NODE    ///!> high level data model for tree node
        } AttributeType;

        /** attribute identifier strings in serialised objects
          those type name should equal to `numpy.typename`
          see: https://numpy.org/devdocs/user/basics.types.html
          because they can be shared by both C adn C++.
          why not const char*, maybe caused by Poco::JSON
        */
        // static char TYPE_NAME_NULL[] = "null";
        // static char TYPE_NAME_SCALAR[] = "scalar";
        // numpy.dtype 's typename
        static char TYPE_NAME_INT8[] = "int8";
        static char TYPE_NAME_INT16[] = "int16";
        static char TYPE_NAME_INT32[] = "int32";
        static char TYPE_NAME_INT64[] = "int64";
        static char TYPE_NAME_UINT8[] = "uint8";
        static char TYPE_NAME_UINT16[] = "uint16";
        static char TYPE_NAME_UINT32[] = "uint32";
        static char TYPE_NAME_UINT64[] = "uint64";
        static char TYPE_NAME_FLOAT32[] = "float32";
        static char TYPE_NAME_FLOAT64[] = "float64";
        static char TYPE_NAME_BOOL[] = "bool";
        // end of numpy.dtype 's typename
        static char TYPE_NAME_STRING[] = "string";
        static char TYPE_NAME_ARRAY[] = "array"; /// has extra class member element_type_name
        static char TYPE_NAME_DICTIONARY[] = "dictionary";
        // static char TYPE_NAME_SIGNAL[] = "signal";

        /// data types that can be element of array type,
        /// not in used yet, maybe removed later
        static std::map<std::string, AttributeType> dtype_map = {
            {"int64", ATTR_INT64},     {"int32", ATTR_INT32},     {"int16", ATTR_INT16},   {"int8", ATTR_INT8},
            {"uint64", ATTR_UINT64},   {"uint32", ATTR_UINT32},   {"uint16", ATTR_UINT16}, {"uint8", ATTR_UINT8},
            {"float64", ATTR_FLOAT64}, {"float32", ATTR_FLOAT32}, {"bool", ATTR_BOOL},     {"string", ATTR_STRING},
        };

        // forward declaratoin
        class Dictionary;
        class IArray;
        class Null;

        ///  type trait to get AttributeType at compiling time
        template <typename DT> AttributeType to_dtype()
        {
            // TODO: using DT = std::remove_cv<DType>::type;
            if (std::is_same<DT, int64_t>::value)
                return ATTR_INT64;
            else if (std::is_same<DT, int32_t>::value)
                return ATTR_INT32;
            else if (std::is_same<DT, int16_t>::value)
                return ATTR_INT16;
            else if (std::is_same<DT, int8_t>::value)
                return ATTR_INT8;
            else if (std::is_same<DT, uint64_t>::value)
                return ATTR_UINT64;
            else if (std::is_same<DT, uint32_t>::value)
                return ATTR_UINT32;
            else if (std::is_same<DT, uint16_t>::value)
                return ATTR_UINT16;
            else if (std::is_same<DT, uint8_t>::value)
                return ATTR_UINT8;
            else if (std::is_same<DT, float>::value)
                return ATTR_FLOAT32;
            else if (std::is_same<DT, double>::value)
                return ATTR_FLOAT64;
            else if (std::is_same<DT, bool>::value)
                return ATTR_BOOL;
            else if (std::is_same<DT, std::string>::value)
                return ATTR_STRING;
            else if (std::is_same<DT, Dictionary>::value)
                return ATTR_DICTIONARY;
            else if (std::is_base_of<IArray, DT>::value)
                return ATTR_ARRAY;
            else if (std::is_same<DT, Null>::value)
                return ATTR_NULL;
            else
            {
                if (std::is_base_of<IArray, DT>::value)
                    return ATTR_NULL;
                else
                {
                    throw SALException("type is not a derived from Attribute base class");
                    //#error "type is not derived from Attribute base class"
                }
            }
        }



        ///  type trait to get data type name (numpy.dtype) at compiling time
        /// used in Array<T> and Atomic<T> instantation
        template <typename DT> const char* to_dtype_name()
        {
            /// TODO: using DT = std::remove_cv<DType>::type;
            /// decay() from pointer or reference to type

            /// NOTE: std::enable_if<>, std::byte, std::complex, not necessary
            /// `if constexpr ()` or constexpr template is only for C++17
            if (std::is_same<DT, int64_t>::value)
                return "int64";
            else if (std::is_same<DT, int32_t>::value)
                return "int32";
            else if (std::is_same<DT, int16_t>::value)
                return "int16";
            else if (std::is_same<DT, int8_t>::value)
                return "int8";
            else if (std::is_same<DT, uint64_t>::value)
                return "uint64";
            else if (std::is_same<DT, uint32_t>::value)
                return "uint32";
            else if (std::is_same<DT, uint16_t>::value)
                return "uint16";
            else if (std::is_same<DT, uint8_t>::value)
                return "uint8";
            else if (std::is_same<DT, float>::value)
                return "float32";
            else if (std::is_same<DT, double>::value)
                return "float64";
            else if (std::is_same<DT, bool>::value)
                return "bool";
            else if (std::is_same<DT, std::string>::value)
                return "string";
            else
            {
                throw SALException("data type valid as Array element or Atomic value");
            }
        }

        /// abstract class corresponding to python DataSummary class.
        /// Attribute class (base data class) implements this interface,
        /// to avoid another summary class tree in parallel with data class tree as in Python
        /// static Attribute::Ptr decode_summary(Poco::JSON::Object::Ptr json)
        class SummaryInterface
        {
        public:
            typedef Poco::SharedPtr<SummaryInterface> Ptr;
            virtual Poco::JSON::Object::Ptr encode_summary() const = 0;

            /// stringified json object returned from `encode_summary()`
            /// C API can get `const char*` string buffer
            /// consider: return std::shared_ptr<std::string> for better performance
            virtual std::string summary() const = 0;

        protected:
            virtual Poco::JSON::Object::Ptr encode_header(bool _is_summary = false) const = 0;
        };

        /// It is low-level data entry, data.signal is high-level data container
        //  Node class has a dictionary of Attributes
        /// The base data class (without any data) should also have a Type and TypeName
        // SKIP: rename to DataObject to align with Python implementation
        // SKIP: implement metadata registration to simulate python decorator
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

            /// no corresponding on python side, maybe useful for quick type comparison
            inline const AttributeType& type() const noexcept
            {
                return m_type;
            }

            // CONSIDER: buffer pointer interface for C-API, currently implemented for Array only
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
            virtual Poco::JSON::Object::Ptr encode_header(bool _is_summary = false) const override final
            {
                /// constant members from python DataObject ReportSummary

                // d['_type'] = TYPE_SUMMARY
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

                // to be python compatible, remove this block after further refactoring,
                if (is_number())
                {
                    json->set("_class", "scalar");
                }
                else
                {
                    json->set("_class", this->type_name());
                }

                if (_is_summary)
                    json->set("_type", "summary");
                else
                {
                    json->set("_type", "object");
                }

                json->set("_group", m_group_name);
                json->set("_version", SAL_API_VERSION);
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
                auto json = encode_header(false);

                json->set("type", this->type_name());
                // only if data value is small, put into summary, not compatible with python
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
            // bool is regarded as a arithmetic in C++
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
            std::string m_type_name;  // CLASS is the typename, it is different for types
            std::string m_group_name; // GROUP

            /// member for summary interface
            bool m_is_summary = false;
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
        template <class T> class Atomic : public Attribute
        {
            T m_value;

        public:
            typedef Poco::SharedPtr<Atomic<T>> Ptr;
            /*
            Constructors and destructor.
            */
            Atomic()
                    : Attribute(to_dtype<T>(), to_dtype_name<T>())
                    , m_value(T()){}; // using T() as the default value is more general
            Atomic(T _value)
                    : Attribute(to_dtype<T>(), to_dtype_name<T>())
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
            static typename Atomic<T>::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                // treat any failure as a failure to decode
                try
                {
                    // check sal type is valid for this class
                    if (json->getValue<std::string>("type") != std::string(to_dtype_name<T>()))
                        throw SALException("type name in json does not match template datatype");
                    return new Atomic<T>(json->getValue<T>("value"));
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL scalar attribute.");
                }
            };
        };

        /// TODO: using scalar as CLASS type, map to python sal implementation
        typedef Atomic<int8_t> Int8;
        typedef Atomic<int16_t> Int16;
        typedef Atomic<int32_t> Int32;
        typedef Atomic<int64_t> Int64;

        typedef Atomic<uint8_t> UInt8;
        typedef Atomic<uint16_t> UInt16;
        typedef Atomic<uint32_t> UInt32;
        typedef Atomic<uint64_t> UInt64;

        typedef Atomic<float> Float32;
        typedef Atomic<double> Float64;
        typedef Atomic<bool> Bool;

        // TODO: specialization String to have different CLASS name
        // std::string is also not support atomic operation
        typedef Atomic<std::string> String;

        /// a typedef to ease future refactoring on data structure
        using ShapeType = std::vector<uint64_t>;


        class IArray : public Attribute
        {
        public:
            typedef Poco::SharedPtr<IArray> Ptr;
            IArray()
                    : Attribute(ATTR_ARRAY, TYPE_NAME_ARRAY)
            {
            }
            virtual ~IArray() // virtual destructor when virtual functions are present.
            {
            }
            /// those functions below could be made non-virtual for better performance
            virtual ShapeType shape() const = 0;
            virtual size_t dimension() const = 0;
            virtual AttributeType element_type() const = 0;
            virtual std::string element_type_name() const = 0;

            /// @{
            /** infra-structure for C-API */
            virtual uint64_t size() const = 0;

            virtual size_t byte_size() const = 0;

            /// read-only pointer to provide read view into the data buffer
            virtual const void* data_pointer() const = 0;

            /// modifiable raw pointer to data buffer, use it with care
            virtual void* data_pointer() = 0;

            virtual void* data_at(int i0, int64_t i1 = -1, int64_t i2 = -1, int64_t i3 = -1, int64_t i4 = -1,
                                  int64_t i5 = -1, int64_t i6 = -1, int64_t i7 = -1, int64_t i8 = -1,
                                  int64_t i9 = -1) = 0;
            /// @}
        };

        /*
        It is a multi-dimension array based on std::vector<T>
        No default constructor without parameter is allowed,
        so shape of the array, as std::vector<uint64_t>,  consistent with python numpy.array
        TODO: proxy pattern for m_data, so big data can not fit into memory can be supported.
        */
        template <class T> class Array : public IArray
        {
        public:
            typedef Poco::SharedPtr<Array<T>> Ptr;

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
                    : IArray()
                    , m_element_type(to_dtype<T>())
                    , m_element_type_name(to_dtype_name<T>())
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

            inline virtual ShapeType shape() const
            {
                return this->m_shape;
            };
            /// consider: plural name
            inline virtual size_t dimension() const
            {
                return this->m_shape.size();
            };

            inline virtual AttributeType element_type() const override
            {
                return m_element_type;
            }

            inline virtual std::string element_type_name() const
            {
                return m_element_type_name;
            }

            /// @{
            /*
            Returns the length of the array buffer, element_size, not byte size
            flattened 1D array from all dimensions
            */
            inline virtual uint64_t size() const
            {
                return this->data.size();
            };

            /** infra-structure for C-API */
            inline virtual size_t byte_size() const
            {
                return this->data.size() * sizeof(T);
            };

            /// read-only pointer to provide read view into the data buffer
            inline virtual const void* data_pointer() const
            {
                return this->data.data();
            }

            /// modifiable raw pointer to data buffer, use it with care
            inline virtual void* data_pointer()
            {
                return this->data.data();
            }

            /// todo: more than 5 dim is kind of nonsense,
            /// using array as index can be more decent
            inline virtual void* data_at(int i0, int64_t i1 = -1, int64_t i2 = -1, int64_t i3 = -1, int64_t i4 = -1,
                                         int64_t i5 = -1, int64_t i6 = -1, int64_t i7 = -1, int64_t i8 = -1,
                                         int64_t i9 = -1) override
            {
                return std::addressof(this->at(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9));
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

            std::vector<T> has two versions
            reference at (size_type n);
            const_reference at (size_type n) const;

            This method performs bounds checking and accepts a variable
            number of array indices corresponding to the dimensionality of
            the array.

            Data access is slower than direct buffer indexing, however it
            handles striding for the user.

            Due to the method of implementing this functionality in C++, it
            only supports arrays with a maximum of 10 dimensions.
            */
            virtual T& at(int i0, int64_t i1 = -1, int64_t i2 = -1, int64_t i3 = -1, int64_t i4 = -1, int64_t i5 = -1,
                          int64_t i6 = -1, int64_t i7 = -1, int64_t i8 = -1, int64_t i9 = -1) throw()
            {
                // NOTE: index are signed integer, assigning -1 means max unsigned interger
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
                    if ((dim_index[i] < 0) || (static_cast<uint64_t>(dim_index[i]) > this->m_shape[i] - 1UL))
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
                Poco::JSON::Object::Ptr json_obj = encode_summary();

                Poco::JSON::Object::Ptr array_definition = new Poco::JSON::Object();
                array_definition->set("type", this->m_element_type_name);
                array_definition->set("shape", this->encode_shape());
                array_definition->set("encoding", this->encoding());
                array_definition->set("data", this->encode_data());

                if (is_summary())
                    throw SALException("this is an summary without data");

                json_obj->set("type", this->type_name());
                json_obj->set("value", array_definition);

                return json_obj;
            };

            std::string encoding() const
            {
                if (std::is_same<std::string, T>::value)
                    return "list"; // TODO: I do not know why?
                else
                {
                    return "base64";
                }
            }

            /*
            Decodes a Poco JSON object representation of the Array and returns an Array object.
            */
            static typename Array<T>::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                Poco::JSON::Object::Ptr array_definition;
                ShapeType shape;
                std::string encoded_data;
                typename Array<T>::Ptr array;

                // treat any failure as a failure to decode
                try
                {
                    // check sal type is valid for this class
                    if (json->getValue<std::string>("type") != std::string(TYPE_NAME_ARRAY))
                        throw SALException("type does not match, array is expected here");

                    // extract array definition
                    array_definition = json->getObject("value");

                    // check array element type and array encoding are valid for this class
                    if (array_definition->getValue<std::string>("type") != to_dtype_name<T>())
                        throw std::exception();
                    auto _encoding = array_definition->getValue<std::string>("encoding");
                    if (_encoding != "base64" or _encoding != "list") // TODO: supported_encodings()
                        throw SALException("encoding is not supported");
                    if (!array_definition->isArray("shape"))
                        throw SALException("decoded shape is not an array");

                    // decode shape
                    shape = Array<T>::decode_shape(array_definition->getArray("shape"));

                    // create and populate array
                    array = new Array<T>(shape);
                    Array<T>::decode_data(array->data, array_definition->getValue<std::string>("data"));
                    return array;
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL Array attribute.");
                }
            };

#if SAL_USE_EIGEN
            // return a const view of buffer
            typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> MatrixType;
            typedef Eigen::Map<const MatrixType> ConstMatrixView;

            /// a read-only of underlying data an Eigen::Matrix
            ConstMatrixView to_eigen_matrix() const
            {
                if (m_dimension == 2)
                    return ConstMatrixView(this->data.data(), m_shape[0], m_shape[1]);
                else
                    throw SALException("only 2-dim array can expose as constant view of Eigen::Matrix");
            }

            /// as a writable Map of Eigen::Matrix
            Eigen::Map<MatrixType> as_eigen()
            {
                if (m_dimension == 2)
                    return Eigen::Map<MatrixType>(this->data.data(), m_shape[0], m_shape[1]);
                else
                    throw SALException("only 2-dim array can expose as constant view of Eigen::Matrix");
            }

#endif
        protected:
            // change element type is not possible without re-create the array object
            const AttributeType m_element_type;
            const std::string m_element_type_name;

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
            but not for std::string data type
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
            `const std::vector<T>& data` is not possible
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
        /// TODO: ATTR_INT8, TYPE_NAME_INT8 should be decided by compiler
        /// std::map<type_index, const char*>, to simplify coding in higher level signal class
        typedef Array<int8_t> Int8Array;
        typedef Array<int16_t> Int16Array;
        typedef Array<int32_t> Int32Array;
        typedef Array<int64_t> Int64Array;

        typedef Array<uint8_t> UInt8Array;
        typedef Array<uint16_t> UInt16Array;
        typedef Array<uint32_t> UInt32Array;
        typedef Array<uint64_t> UInt64Array;

        typedef Array<float> Float32Array;
        typedef Array<double> Float64Array;

        // std::vector<bool> is a specialized std::vector<>
        typedef Array<bool> BoolArray;          // has error decoding, different allocator
        typedef Array<std::string> StringArray; // has different encoding "list"

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
            bool has(const std::string& key) const
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

        ///
        static IArray::Ptr decode_array(Poco::JSON::Object::Ptr json)
        {
            Poco::JSON::Object::Ptr array_definition;
            std::string el_type_name;

            try
            {
                array_definition = json->getObject("value");
                el_type_name = array_definition->getValue<std::string>("type");
            }
            catch (...)
            {
                // todo: define a sal exception and replace
                throw SALException("JSON object does not define a valid SAL Array attribute.");
            }

            // this can be removed if Array<T> is working
            if (el_type_name == TYPE_NAME_INT8)
                return Int8Array::decode(json);
            else if (el_type_name == TYPE_NAME_INT16)
                return Int16Array::decode(json);
            else if (el_type_name == TYPE_NAME_INT32)
                return Int32Array::decode(json);
            else if (el_type_name == TYPE_NAME_INT64)
                return Int64Array::decode(json);
            else if (el_type_name == TYPE_NAME_UINT8)
                return UInt8Array::decode(json);
            else if (el_type_name == TYPE_NAME_UINT16)
                return UInt16Array::decode(json);
            else if (el_type_name == TYPE_NAME_UINT32)
                return UInt32Array::decode(json);
            else if (el_type_name == TYPE_NAME_UINT64)
                return UInt64Array::decode(json);
            else if (el_type_name == TYPE_NAME_FLOAT32)
                return Float32Array::decode(json);
            else if (el_type_name == TYPE_NAME_FLOAT64)
                return Float64Array::decode(json);
            // else if (element_id == TYPE_NAME_BOOL)
            //    return BoolArray::decode(json);  // TODO: compiling error, needs std::enable_if<>
            else if (el_type_name == TYPE_NAME_STRING)
                return StringArray::decode(json); // TODO: decoding may not working, needs std::enable_if<>
            else
                throw SALException("data type string `" + el_type_name + "` is not supported");
        }

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

            if (id == TYPE_NAME_ARRAY)
                return decode_array(json);

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