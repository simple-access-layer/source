#pragma once

#include "sal_data.h"

/** TODO:
 *
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
        /// NOTE: This class is data object content
        /// split out from Alex Leaf class (mixing of Node meta data and content)
        /// member naming can map to OPC-UA attributeIDss
        /// https://github.com/FreeOpcUa/python-opcua/blob/master/opcua/ua/attribute_ids.py

        /// This is base class for all user-defined class like Signal
        /// consider:  derive from Attribute, single root principle
        class DataObject : public Attribute
        {
        public:
            typedef Poco::SharedPtr<DataObject> Ptr;
            DataObject(const std::string _dtype_name)
                    : Attribute(ATTR_DATA_OBJECT, _dtype_name)
            {
                m_group_name = "signal"; // todo: a better default group name
            }


            bool is_full_object() const
            {
                return !m_is_summary;
            }

            template <typename TargetT> typename TargetT::Ptr cast()
            {
                return TargetT::decode(m_json);
            }

            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j = encode_summary();
                // todo: if constant class_type
                // j->set("status", m_array->encode());

                return j;
            }

            static DataObject::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                DataObject::Ptr p = new DataObject("data_object");
                bool is_summary = DataObject::is_summary(json);
                if (is_summary)
                {
                    p->m_is_summary = true;
                }
                else
                {
                    p->m_is_summary = false;
                    decode_content(json, p);
                }
                p->m_json = json;
                return p;
            }

            /// NOTE:
            // data class filed should be `get()` into Signal class or Dictionary Attribute class
            // code duplication, leave contains only one object::Dictionary instance
            object::Attribute::Ptr& operator[](const std::string& key)
            {
                return this->attributes.at(key);
            };
            object::Attribute::Ptr& get(const std::string& key)
            {
                return (*this)[key];
            };
            template <class T> typename T::Ptr get_as(const std::string& key)
            {
                return typename T::Ptr(this->get(key).cast<T>());
            };
            void set(const std::string& key, const object::Attribute::Ptr& attribute)
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

            static bool is_summary(const Poco::JSON::Object::Ptr j)
            {
                auto obj_type = String::decode(j->getObject("_type"))->value();
                return obj_type == "summary";
            }

            /// core.dataclass._new_dict() with common header info (metadata)
            /// however, Atomic/Scalar types does not have these metadata
            virtual Poco::JSON::Object::Ptr encode_metadata(bool _is_summary = false) const;


            // meta data and description have been constructed as member variable
            // consider move all meta data into a Dictionary data object
            static void remove_meta_attributes(DataObject::Ptr obj)
            {
                obj->remove("_class");
                obj->remove("_group");
                obj->remove("_version");
                obj->remove("_type");
                obj->remove("description");
            }

            /*
            Decodes a Poco JSON object representation of the data object and returns a SAL data object.
            leaf node data type can be signal
            */
            static void decode_content(const Poco::JSON::Object::Ptr json, DataObject::Ptr obj)
            {
                std::vector<std::string> keys;

                // treat any failure as a failure to decode
                try
                {
                    json->getNames(keys);
                    for (std::vector<std::string>::iterator key = keys.begin(); key != keys.end(); ++key)
                    {
                        // skip null elements
                        if (json->isNull(*key))
                            continue;

                        if (!json->isObject(*key))
                            throw SALException("all valid attributes must be JSON object not number or std::string");

                        // dispatch object to the appropriate decoder and add to container
                        obj->set(*key, sal::object::decode(json->getObject(*key)));
                    }

                    // remove extracted meta data in the for loop
                    remove_meta_attributes(obj);
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL data object.");
                }
            };

        protected:
            std::string m_dtype;
            // CONSIDER: keep path name here to identify this unique signal

            /// raw json to be further casted into user defined type like Signal
            Poco::JSON::Object::Ptr m_json;

            /// NOTE: in Alex' code, DictionaryAttribute methods are mixed into this class
            /// It is recommended to let user user `d = cast<Dictionary>();`

            /// Object::Dictionary holds only value types, this holds ptr
            std::map<std::string, Attribute::Ptr> attributes;
        };

        Poco::JSON::Object::Ptr DataObject::encode_metadata(bool _is_summary) const
        {
            /// constant members from python DataObject ReportSummary

            // d['_type'] = TYPE_SUMMARY
            Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

            // to be python compatible, remove this block after further refactoring,
            if (is_number())
            {
                auto a = new String("scalar");
                json->set("_class", a->encode());
            }
            else
            {
                auto a = new String(this->type_name());
                json->set("_class", a->encode());
            }

            if (_is_summary)
            {
                auto a_type_name = new String("summary");
                json->set("_type", a_type_name->encode());
            }
            else
            {
                auto a_type_name = new String("object");
                json->set("_type", a_type_name->encode());
            }

            auto a_group_name = new String(m_group_name);
            json->set("_group", a_group_name->encode());
            // todo: this is per object version
            auto a_version = new UInt64(SAL_API_VERSION);
            json->set("_version", a_version->encode());

            // todo: deal with description is empty?  also UUID for data object
            auto a_description = new String(m_description);
            json->set("description", a_description->encode());
            return json;
        }
    } // namespace dataclass

    /// declared in Attribute class but implemented here close to encode_metadata()
    void object::Attribute::decode_metadata(const Poco::JSON::Object::Ptr j, Attribute::Ptr attr)
    {
        // _version is also fixed at compiling time, but can be checked
        attr->m_description = String::decode(j->getObject("description"))->value();
        auto obj_type = String::decode(j->getObject("_type"))->value();
        attr->m_is_summary = obj_type == "summary";
        // _group, _class
        attr->m_group_name = String::decode(j->getObject("_group"))->value();
        // m_type_name is fixed at object creation time
        // better to check, Atomic<> change type to scaler during encoding
        // attr->m_type_name = String::decode(j->getObject("_class"))->value();
    }
} // namespace sal