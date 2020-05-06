#pragma once

#include "sal_data.h"

/** TODO:
 *  summary interface should be able to reuse Attribute base class impl
 *  encode_summary() is need as there is
 *  full data object, encode and decode needs
 */

namespace sal
{
    // using namespace std;
    using namespace exception;
    using namespace object;

    /// corresponding to python sal.dataclass.signal module
    namespace dataclass
    {
        // consider: merge with AttributeType,  or at least, shift value
        typedef enum
        {
            SignalBase = 30,
            ConstantError,
            SymmetricError,
            AsymmetricError,     // consider merge with SymmetricError,  into ArrayError
            CalculatedDimension, // equal spaced/step grid along dimension axis
            ArrayDimensions,
            ConstantMask,
            ArrayMask
        } SignalType;

        char TYPE_NAME_SIGNAL_DIMENSION[] = "signal_dimension";
        char TYPE_NAME_SIGNAL_MASK[] = "signal_mask";
        char TYPE_NAME_SIGNAL_ERROR[] = "signal_error";


        /// there is no need to implement derived class
        /// consider: named as SignalBase
        /// member naming can map to OPC-UA attributeIDss
        /// https://github.com/FreeOpcUa/python-opcua/blob/master/opcua/ua/attribute_ids.py
        class DataObject : public Attribute
        {
        public:
            typedef Poco::SharedPtr<DataObject> Ptr;
            DataObject(const std::string _dtype_name)
                    : Attribute(ATTR_SIGNAL, _dtype_name)
            //, m_group_name("signal")
            {
                m_group_name = "signal";
            }
            // TODO: units and datatype compatible check
        protected:
            std::string m_dtype;
            // CONSIDER: keep path name here to identify this unique signal
        };

        /** Mask is really bad name, it is SignalQuality in the current python code
         * in the future, more meta data, misc data may needed
         * so is the the member `key`, it is just string name list of quality
        status: uint8 = 0
        key: array<string> = ["unvalidated", "good", "medium", "poor", "unusable"]
        */
        template <class DType> class Mask : public DataObject
        {
        public:
            typedef Poco::SharedPtr<Mask> Ptr;
            Mask()
                    : DataObject("signal_mask")
            {
                m_group_name = "signal_mask";
                m_dtype = to_dtype_name<DType>();
            }

            static Mask::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                Mask::Ptr p; //  todo new Mask();
                return p;
            }
            // override encode() to be compatible with python, diff _class
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }

        protected:
            DType m_data;
            StringArray m_key;
        };

        /// Error is not a good name, it is value variance
        /// Scalar (constant along the dimension) and Array types
        template <class DType> class Error : public DataObject
        {
        public:
            typedef Poco::SharedPtr<Error> Ptr;
            // Asymmetric error constructor
            Error(DType lower, DType upper, bool is_relative)
                    : DataObject("signal_error")
            {
                m_group_name = "signal_error";
                m_dtype = to_dtype_name<DType>();
                m_is_symmetric = false;
            }
            // symmetric error constructor
            Error(DType amplitude, bool is_relative)
                    : Error(amplitude, amplitude, is_relative)
            {
                m_is_symmetric = true;
            }

            const DType& upper() const
            {
                return m_upper;
            }
            const DType& lower() const
            {
                return m_lower;
            }
            //    const DType& amplitude() const
            bool relative() const
            {
                return m_is_relative;
            }

            bool constant() const
            {
                // return std::is_arithmetic<DType>::value;  // C++11, bool is also regarded as arithmetic
                return std::is_integral<DType>::value || std::is_floating_point<DType>::value;
            }
            bool symmetric() const
            {
                return m_is_symmetric;
                // also compare constant upper and lower value?
            }

            ///
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }

            static Error::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                // todo: new Error()
                return nullptr;
            }

        protected:
            DType m_lower;
            DType m_upper;
            bool m_is_relative;
            bool m_is_symmetric;
        };

        /// one temporal or spatial dimension data array
        /// for a multi-dim coordinate system
        /// TODO: there are two types of dimension, calculated or 1D array
        template <typename DType> class Dimension : public DataObject
        {
        public:
            typedef Poco::SharedPtr<Dimension> Ptr;
            ///
            Dimension(typename Array<DType>::Ptr coord, bool temoral, std::string units)
                    : DataObject(TYPE_NAME_SIGNAL_DIMENSION)
                    , m_temoral(temoral)
                    , m_units(units)
                    , m_data(coord)
            {
                m_group_name = "signal_dimension";
                m_class_name = "coordinate_array";
                m_is_calculated = false;
                m_dtype = to_dtype_name<DType>();
            }

            /// Constructor for the calculated dimension type
            Dimension(DType start, DType length, DType step, bool temoral, std::string units)
                    : DataObject(TYPE_NAME_SIGNAL_DIMENSION)
                    , m_temoral(temoral)
                    , m_units(units)
                    , m_start(start)
                    , m_length(length)
                    , m_step(step)
            {
                m_group_name = "signal_dimension";
                m_class_name = "calculated";
                m_is_calculated = true;
                m_dtype = to_dtype_name<DType>();
            }

            // TODO:
            // getters

            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }

            /// see python documentation
            static Dimension::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                auto class_name = String::decode(json->getObject("_class"))->value();
                auto unit_name = String::decode(json->getObject("units"))->value();
                auto temporal = Bool::decode(json->getObject("temporal"))->value();
                if (class_name == "coordinate_array")
                {
                    // length = array_shape
                    auto a = Array<DType>::decode(json->getObject("data"));
                    Dimension::Ptr p = new Dimension(a, temporal, unit_name);
                    return p;
                }
                else if (class_name == "calculated") // not tested
                {
                    auto start = Atomic<DType>::decode(json->getObject("start"))->value();
                    auto le = Atomic<DType>::decode(json->getObject("length"))->value();
                    auto step = Atomic<DType>::decode(json->getObject("step"))->value();
                    Dimension::Ptr p = new Dimension(start, le, step, temporal, unit_name);
                    return p;
                }
                else
                {
                    std::string msg = "Dimension class type `" + class_name + "` is not supported in decoding";
                    throw SALException(msg.c_str());
                }
            }

        protected:
            std::string m_units;
            bool m_temoral = false;
            std::string m_class_name;
            bool m_is_calculated;

            typename Array<DType>::Ptr m_data; // for _class == coordinate_array

            DType m_start; //  _class == calculated
            DType m_step;
            DType m_length; // ending = m_start + m_length
        };

        /// default template type = float32
        template <typename DType> class Signal : public DataObject
        {
            /// non public constructor, instance must be created from the static `decode()` factory method
            Signal()
                    : DataObject("signal")
            {
            }

        public:
            typedef Poco::SharedPtr<Signal> Ptr;
            const ShapeType shape() const
            {
                return m_data->shape();
            }

            virtual bool shape_compatible(ShapeType shape) const
            {
                return true; // todo
            }

            /// TODO: error, mask, dimensions getter, but no setter!

            ///
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j; // todo: new
                return j;
            }

            // optional todo:
            // to_VTK() for visualization in paraview
            // to_mesh() // need to be override in derived class like for JET reactor

        protected:
            std::vector<typename Dimension<DType>::Ptr> m_dimensions;
            std::string m_units;
            IArray::Ptr m_data;
            typename Error<DType>::Ptr m_error;
            typename Mask<DType>::Ptr m_mask;

        public:
            static Signal<DType>::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                Signal<DType>::Ptr sig = new Signal<DType>();
                // all meta data "_class, _group" does not needs to be decoded
                sig->m_units = String::decode(json->getObject("units"))->value();
                sig->m_data = Array<DType>::decode(json->getObject("data"));
                Signal::decode_dimensions(json->getObject("dimensions"), sig->m_dimensions);
                // optional fields
                if (json->has("error") and json->getObject("error"))
                {
                    sig->m_error = Error<DType>::decode(json->getObject("error"));
                }
                if (json->has("mask") and json->getObject("mask"))
                {
                    sig->m_mask = Mask<DType>::decode(json->getObject("mask"));
                }
                return sig;
            }

        protected:
            static void decode_dimensions(const Poco::JSON::Object::Ptr json,
                                          std::vector<typename Dimension<DType>::Ptr>& dims)
            {
                const Poco::JSON::Array::Ptr ja = json->getArray("value");
                assert(ja && ja->size() > 0);
                for (const auto& d : *ja) // Null Pointer error here
                {
                    const auto dj = d.extract<Poco::JSON::Object::Ptr>();
                    dims.push_back(Dimension<DType>::decode(dj));
                }
            }
        };

        // consider return Attribute::Ptr from decode_summary()
        static SummaryInterface::Ptr decode_summary(Poco::JSON::Object::Ptr json);
        // python classmethod `from_dict()`
        // static Attribute::Ptr decode(Poco::JSON::Object::Ptr json);


    } // namespace dataclass

} // namespace sal