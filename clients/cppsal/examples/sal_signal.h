#pragma once

#include "sal_data.h"
#include "sal_data_object.h"

/** WARNING:
 *  This header is not part of core C++ API, no API stabilization is guaranteed
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
        // currently not in use, maybe remove it
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

        static const char* GROUP_NAME_SIGNAL_DIMENSION = "signal_dimension";
        static const char* GROUP_NAME_SIGNAL_MASK = "signal_mask";
        static const char* GROUP_NAME_SIGNAL_ERROR = "signal_error";

        /** Mask is really misleading name, it is SignalQuality in the current python code
         * in the future, more meta data, misc data may needed
         * so is the the member `key`, it is just string name list of quality
        status: uint8 = 0
        key: array<string> = ["unvalidated", "good", "medium", "poor", "unusable"]
        */
        template <class DType> class Mask : public DataObject
        {
        public:
            typedef Poco::SharedPtr<Mask> Ptr;
            Mask(const std::string _class_name)
                    : DataObject(_class_name, GROUP_NAME_SIGNAL_MASK)
            {
                // m_group_name = GROUP_NAME_SIGNAL_MASK;
                m_dtype = to_dtype_name<DType>();
            }

            // override encode() to be compatible with python, diff _class
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata(true);
                // todo: if constant class_type
                j->set("status", m_array->encode_summary());
                j->set("key", m_key->encode_summary());
                return DataObject::wrap_payload(j);
            }
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata();
                // todo:
                j->set("status", m_array->encode());
                j->set("key", m_key->encode());
                return DataObject::wrap_payload(j);
            }

            static Mask::Ptr decode(const Poco::JSON::Object::Ptr j)
            {
                Mask::Ptr p = nullptr;
                try
                {
                    auto json = j->getObject("value");
                    p = new Mask(DataObject::parse_class_name(json));

                    Attribute::decode_metadata(json, p);
                    if (!DataObject::is_summary_object(json))
                    {
                        p->m_array = UInt8Array::decode(json->getObject("status"));
                        p->m_key = StringArray::decode(json->getObject("key"));
                    }
                }
                catch (const std::exception& e)
                {
                    SAL_REPORT_DECODE_ERROR(e, j);
                }
                return p;
            }

        protected:
            DType m_constant;        // todo: no such server data I can find
            UInt8Array::Ptr m_array; // enum integer value, is that always Unit8Array?
            StringArray::Ptr m_key;  // enum string name
        };

        /// CONSIDER: a base Error class for all Error<T> classes
        /// NOTE: different from python, c++ has only one class,
        //// but using class_name to behave like 3 classes

        /// Error is not a good name, it is value variance
        /// template parameter can be Scalar constant and Array::Ptr types
        template <class DType> class Error : public DataObject
        {
            // for summary decoding only
            Error(std::string class_name)
                    : DataObject(class_name, "signal_error")
            {
            }

        public:
            typedef Poco::SharedPtr<Error> Ptr;

            // Asymmetric error constructor
            Error(DType lower, DType upper, bool is_relative)
                    : DataObject("error_constant", GROUP_NAME_SIGNAL_ERROR)
                    , m_is_relative(is_relative)
                    , m_is_symmetric(false)
                    , m_is_constant(true)
            {
                // m_group_name = "GROUP_NAME_SIGNAL_ERROR";
                m_dtype = to_dtype_name<DType>();
            }
            // symmetric constant error constructor
            Error(DType amplitude, bool is_relative)
                    : Error(amplitude, amplitude, is_relative)
            //, m_is_constant(true)
            {
            }

            /* array upper and lower error constructor */
            Error(typename Array<DType>::Ptr lower, typename Array<DType>::Ptr upper, bool is_relative)
                    : DataObject("", GROUP_NAME_SIGNAL_ERROR)
                    , m_lower_array(lower)
                    , m_upper_array(upper)
                    , m_is_relative(is_relative)
                    , m_is_symmetric(false)
                    , m_is_constant(false)
            {
            }

            // symmetric constant error constructor
            Error(typename Array<DType>::Ptr amplitude, bool is_relative)
                    : DataObject("", GROUP_NAME_SIGNAL_ERROR)
                    , m_lower_array(amplitude)
                    , m_upper_array(amplitude)
                    , m_is_relative(is_relative)
                    , m_is_symmetric(true)
                    , m_is_constant(false)
            {
            }

            /// defined getters, no setter
            std::string class_name() const
            {
                return m_type_name;
            }
            const DType constant_upper() const
            {
                return m_upper;
            }
            const DType constant_lower() const
            {
                return m_lower;
            }
            const typename Array<DType>::Ptr array_upper() const
            {
                return m_upper_array;
            }
            const typename Array<DType>::Ptr array_lower() const
            {
                return m_lower_array;
            }
            //
            const DType constant_amplitude() const
            {
                return (m_lower + m_upper) * 0.5;
            }

            bool relative() const
            {
                return m_is_relative;
            }
            bool constant() const
            {
                return m_is_constant;
            }
            bool symmetric() const
            {
                return m_is_symmetric;
                // also compare constant upper and lower value?
            }

            ///
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata();
                // todo: diff class_name
                if (class_name() == "error_constant")
                {
                    j->set("lower", Atomic<DType>(m_lower).encode());
                    j->set("upper", Atomic<DType>(m_upper).encode());
                }
                return DataObject::wrap_payload(j);
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata(true);
                // todo: diff class_name
                if (class_name() == "error_constant")
                {
                    j->set("lower", Atomic<DType>(m_lower).encode());
                    j->set("upper", Atomic<DType>(m_upper).encode());
                }
                return DataObject::wrap_payload(j);
            }

            static Error::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                auto class_name = String::decode(json->getObject("_class"))->value();
                bool is_summary = DataObject::is_summary_object(json);
                if (is_summary)
                {
                    Error::Ptr p;
                    p = new Error(class_name);
                    Attribute::decode_metadata(json, p);
                    return p;
                }
                else
                {
                    return decode_full_object(json);
                }
            }

            static Error::Ptr decode_full_object(const Poco::JSON::Object::Ptr json)
            {
                Error::Ptr p;
                auto class_name = String::decode(json->getObject("_class"))->value();
                bool is_relative = Bool::decode(json->getObject("relative"))->value();
                if (class_name == "error_constant")
                {
                    DType lower = Atomic<DType>::decode(json->getObject("lower"))->value();
                    DType upper = Atomic<DType>::decode(json->getObject("upper"))->value();

                    p = new Error<DType>(lower, upper, is_relative);
                }
                else if (class_name == "error_symmetrical")
                {
                    auto lower = Array<DType>::decode(json->getObject("lower"));
                    // p = new Error<typename Array<DType>::Ptr>(lower, is_relative);
                    p = new Error<DType>(lower, is_relative);
                }
                else if (class_name == "error_asymmetric")
                {
                    auto lower = Array<DType>::decode(json->getObject("lower"));
                    auto upper = Array<DType>::decode(json->getObject("upper"));
                    // p = new Error<typename Array<DType>::Ptr>(lower, upper, is_relative);
                    p = new Error<DType>(lower, upper, is_relative);
                }
                else
                {
                    std::string msg = "Error class type `" + class_name + "` has not been defined";
                    throw SALException(msg.c_str());
                }
                p->m_type_name = class_name;
                return p;
            }

        protected:
            DType m_lower, m_upper;                                  // used only if is_constant() == true
            typename Array<DType>::Ptr m_lower_array, m_upper_array; // if not constant

            bool m_is_relative;
            bool m_is_symmetric;
            bool m_is_constant;
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
                    : DataObject("coordinate_array", GROUP_NAME_SIGNAL_DIMENSION)
                    , m_temporal(temoral)
                    , m_units(units)
                    , m_data(coord)
            {
                // m_group_name = "signal_dimension";
                // m_class_name = "coordinate_array";
                m_is_calculated = false;
                m_dtype = to_dtype_name<DType>();
            }

            /// Constructor for the calculated dimension type
            Dimension(DType start, DType length, DType step, bool temoral, std::string units)
                    : DataObject("calculated", GROUP_NAME_SIGNAL_DIMENSION)
                    , m_temporal(temoral)
                    , m_units(units)
                    , m_start(start)
                    , m_length(length)
                    , m_step(step)
            {
                // m_group_name = "signal_dimension";
                // m_class_name = "calculated";
                m_is_calculated = true;
                m_dtype = to_dtype_name<DType>();
            }

            // TODO:
            // getters

            Poco::JSON::Object::Ptr _encode(bool is_summary) const
            {
                Poco::JSON::Object::Ptr j = encode_metadata(is_summary);
                j->set("units", String(m_units).encode());
                j->set("temporal", Bool(m_temporal).encode());
                /// NOTE: length is misleading/not necessary for "coordinate_array"
                j->set("length", Atomic<DType>(m_length).encode());
                if (class_name() == "coordinate_array")
                {
                    if (is_summary)
                    {
                        // do nothing
                    }
                    else
                    {
                        j->set("data", m_data->encode());
                    }
                }
                else if (class_name() == "calculated")
                {
                    j->set("start", Atomic<DType>(m_start).encode());
                    j->set("step", Atomic<DType>(m_step).encode());
                }
                else
                {
                    std::string msg = "Dimension class type `" + class_name() + "` is not defined";
                    throw SALException(msg.c_str());
                }
                return DataObject::wrap_payload(j);
            }
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                return _encode(false);
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                return _encode(true);
            }

            /// see python documentation
            static Dimension::Ptr decode(const Poco::JSON::Object::Ptr j)
            {
                /// Note: there is another json wrapper over the payload data
                // but `type` = "branch"
                const Poco::JSON::Object::Ptr json = j->getObject("value");

                std::string unit_name = String::decode(json->getObject("units"))->value(); // why error?
                auto temporal = Bool::decode(json->getObject("temporal"))->value();
                Dimension::Ptr p;
                const auto class_name = String::decode(json->getObject("_class"))->value();
                if (class_name == "coordinate_array")
                {
                    typename Array<DType>::Ptr a;
                    if (DataObject::is_summary_object(json))
                    {
                        // length = array_shape
                        a = nullptr;
                        p = new Dimension(a, temporal, unit_name);
                        p->m_is_summary = true;
                    }
                    else
                    {
                        a = Array<DType>::decode(json->getObject("data"));
                        p = new Dimension(a, temporal, unit_name);
                        p->m_is_summary = false;
                    }
                }
                else if (class_name == "calculated") // todo: not tested by server response
                {
                    // should no diff between summary and full object json
                    auto start = Atomic<DType>::decode(json->getObject("start"))->value();
                    auto le = Atomic<DType>::decode(json->getObject("length"))->value();
                    auto step = Atomic<DType>::decode(json->getObject("step"))->value();
                    Dimension::Ptr p = new Dimension(start, le, step, temporal, unit_name);
                }
                else
                {
                    std::string msg = "Dimension class type `" + class_name + "` is not supported in decoding";
                    throw SALException(msg.c_str());
                }
                Attribute::decode_metadata(json, p);
                return p;
            }

        protected:
            std::string m_units;
            bool m_temporal = false;
            // std::string m_class_name;
            bool m_is_calculated;

            typename Array<DType>::Ptr m_data; /// for _class == coordinate_array

            /// fields for  _class == calculated
            DType m_start;
            DType m_step;
            DType m_length; // ending = m_start + m_length
        };

        /// default template type = float32
        template <typename DType> class Signal : public DataObject
        {
            /// non public constructor, instance must be created from the static `decode()` factory method
            Signal()
                    : DataObject("signal", "signal")
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
                Poco::JSON::Object::Ptr j = encode_metadata();
                j->set("units", String(m_units).encode());
                j->set("dimensions", encode_dimensions());
                if (m_mask)
                    j->set("mask", m_mask->encode());
                if (m_error) // maybe, nullptr is encoded as null
                    j->set("error", m_error->encode());
                j->set("data", m_data->encode());
                return DataObject::wrap_payload(j);
            }
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata(true);
                j->set("units", String(m_units).encode_summary());
                // todo: summary version of this function is yet done
                throw SALException("summary of dimensions is yet implemented");
                j->set("dimensions", encode_dimensions());
                if (m_mask)
                    j->set("mask", m_mask->encode_summary());
                if (m_error)
                    j->set("error", m_error->encode_summary());
                return DataObject::wrap_payload(j);
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
                Attribute::decode_metadata(json, sig);
                sig->m_units = String::decode(json->getObject("units"))->value();
                if (DataObject::is_summary_object(json))
                {
                    sig->m_is_summary = true;
                }
                else
                {
                    sig->m_data = Array<DType>::decode(json->getObject("data"));
                    sig->m_is_summary = false;
                }

                Signal::decode_dimensions(json->getObject("dimensions"), sig->m_dimensions);
                // optional fields
                if (json->has("error") and json->getObject("error"))
                {
                    sig->m_error = Error<DType>::decode(json->getObject("error"));
                }
                if (json->has("mask") and json->getObject("mask"))
                {
                    /// consider: Mask does not needs to be a template class?
                    sig->m_mask = Mask<DType>::decode(json->getObject("mask"));
                }
                return sig;
            }

        protected:
            Poco::JSON::Array::Ptr encode_dimensions() const
            {
                Poco::JSON::Array::Ptr j = new Poco::JSON::Array();
                for (const auto& d : m_dimensions)
                {
                    j->add(d->encode());
                }
                return j;
            }

            ///  summary and full object has different "dimensions" data structure
            ///  https://simple-access-layer.github.io/documentation/datamodel/dataclasses/signal.html
            static void decode_dimensions(const Poco::JSON::Object::Ptr json,
                                          std::vector<typename Dimension<DType>::Ptr>& dims)
            {
                // json->stringify(std::cout, 2);
                /// NOTE: dimension data is in "value" subobject
                if (json->isArray("value"))
                {
                    /* keep this code, as Array is the better way to organize multiple Dimension */
                    const Poco::JSON::Array::Ptr ja = json->getArray("value");
                    for (const auto& d : *ja)
                    {
                        const auto dj = d.extract<Poco::JSON::Object::Ptr>();
                        dims.push_back(Dimension<DType>::decode(dj));
                    }
                }
                else
                {
                    const Poco::JSON::Object::Ptr ja = json->getObject("value");
                    assert(ja && ja->size() > 0);
                    std::vector<std::string> keys;
                    ja->getNames(keys);
                    for (const auto& key : keys) // not Array type but a map with key "0", "1"
                    {
                        const auto dj = ja->getObject(key);
                        // dj->stringify(std::cout, 2);  // assuming key is "0", "1", etc
                        if (key.size() == 1) // need also filter out some key like "count"
                            dims.push_back(Dimension<DType>::decode(dj));
                    }
                }
                assert(dims.size() > 0);
            }
        };

        // consider return Attribute::Ptr from decode_summary()
        static SummaryInterface::Ptr decode_summary(Poco::JSON::Object::Ptr json);
        // python classmethod `from_dict()`
        // static Attribute::Ptr decode(Poco::JSON::Object::Ptr json);

    } // namespace dataclass

} // namespace sal