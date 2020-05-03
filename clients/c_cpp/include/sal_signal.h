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

            virtual bool shape_compatible(ShapeType shape) const;
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

            // override encode() to be compatible with python, diff _class
            // encode_summary() const override {}
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

            virtual bool shape_compatible(ShapeType shape) const override
            {
                return true;
            }

            // override encode_summary() to be compatible with python, diff _class
            // override encode() to be compatible with python, diff _class
            // static decode()

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

        protected:
            DType m_lower;
            DType m_upper;
            bool m_is_relative;
            bool m_is_symmetric;
        };

        /// temporal or spatial dimension for multiple-dimension data array
        /// to build up coordinate system
        /// TODO: there are two types of dimension, calculated or 1D array
        template <class DType> class Dimension : public DataObject
        {
        public:
            typedef Poco::SharedPtr<Dimension> Ptr;
            Dimension(DType start, DType length, DType step, std::string units)
                    : DataObject(TYPE_NAME_SIGNAL_DIMENSION)
                    , m_start(start)
                    , m_length(length)
                    , m_step(step)
            {
                m_group_name = "signal_dimension";
                m_dtype = to_dtype_name<DType>();
            }

            // TODO:
            // getters
            // static decode()
            // encode()
            // encode_summary()
        protected:
            DType m_start;
            DType m_step;
            DType m_length; // ending = m_start + m_length
            bool m_temoral = false;
            std::string m_units;
            std::string m_dtype;

            typename Array<DType>::Ptr m_data;
            bool m_is_calculated;
        };

        /// default template type = float64
        template <typename DType = double> class Signal : public DataObject
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

            // TODO: error, mask, dimensions getter, but no setter!
            // encode() const

            // optional todo:
            // to_VTK() for visualization in paraview
            // to_mesh() // need to be override in derived class like for JET reactor

        protected:
            std::vector<Dimension<DType>> m_dimensions;
            std::string units;
            IArray::Ptr m_data;
            Poco::Nullable<Error<DType>> m_error; // std::optional<>
            Poco::Nullable<Mask<DType>> m_mast;

        public:
            static Signal::Ptr decode(Poco::JSON::Object::Ptr json)
            {
                Signal::Ptr sig;
                // TODO:
                return sig;
            }
        };


        // consider return Attribute::Ptr from decode_summary()
        static SummaryInterface::Ptr decode_summary(Poco::JSON::Object::Ptr json);
        // python classmethod `from_dict()`
        static Attribute::Ptr decode(Poco::JSON::Object::Ptr json);

    } // namespace dataclass

} // namespace sal