#pragma once

#include "sal_data.h"

/** TODO:
 *
 *  full data object, encode and decode needs
 *  move ISummary interface from Attribute to DataClass
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
        /// metadata map to to Python class sal.core.dataclass.DataClass
        /// consider:  derive from Attribute, single root principle
        /// Attribute (type and value) + meta data make a DataObject
        class DataObject : public Attribute
        {
        public:
            typedef Poco::SharedPtr<DataObject> Ptr;
            DataObject(const std::string _class_name, const std::string _group_name = "data_object")
                    : Attribute(ATTR_DATA_OBJECT, "data_object")
                    , m_class_name(_class_name)
                    , m_group_name(_group_name)
            {
            }

            /*
            DataObject(const Attribute& attr)
                    : Attribute(ATTR_DATA_OBJECT, _class_name, _group_name)
            {
            }
            */

            /// @{
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
            /// @}


            /// @{ getter for metadata attributes
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

            /// group_name getter, corresponding to `_group`
            inline std::string group_name() const noexcept
            {
                return m_group_name;
            }

            /// class_name getter, corresponding to `_class`
            inline std::string class_name() const noexcept
            {
                if (is_number())
                {
                    return "scalar";
                }
                else
                {
                    return m_class_name;
                }
            }
            /// @}

            static bool is_summary_content(const Poco::JSON::Object::Ptr j)
            {
                auto obj_type = String::decode(j->getObject("_type"))->value();
                return obj_type == "summary";
            }
            static bool is_full_content(const Poco::JSON::Object::Ptr j)
            {
                auto obj_type = String::decode(j->getObject("_type"))->value();
                return obj_type == "object";
            }
            static bool is_data_object(const Poco::JSON::Object::Ptr j)
            {
                if (j->has("type"))
                {
                    auto t = j->getValue<std::string>("type");
                    return t == "branch";
                }
                return false;
            }
            static std::string parse_class_name(const Poco::JSON::Object::Ptr j)
            {
                return String::decode(j->getObject("_class"))->value();
            }

            /// core.dataclass._new_dict() with common header info (metadata)
            virtual Poco::JSON::Object::Ptr encode_metadata(bool _is_summary = false) const;
            /// decoded the attribute header/metadata/common attributes
            static void decode_metadata(const Poco::JSON::Object::Ptr j, DataObject::Ptr attr);

            /// todo:  should distinguish toplevel obj and sub,
            /// may needs a field "m_is_root" for toplevel data, not subobject.
            /// also renamed as "encode_payload()", make it non-static
            static Poco::JSON::Object::Ptr wrap_payload(const Poco::JSON::Object::Ptr j)
            {
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                json->set("type", "branch");
                json->set("value", j);
                return json;
            }
            /// unbox
            static Poco::JSON::Object::Ptr unwrap_payload(const Poco::JSON::Object::Ptr j)
            {
                if (j->has("object")) // root of sub-data-object, returned from get(), type == "leaf"
                    return j->getObject("object");
                if (j->has("value")) // sub-data-object, type == "branch"
                    return j->getObject("value");
                else
                {
                    throw SALException(
                        "`value` or `object` key is not found in json to retrive payload (object content)");
                }
            }

            inline bool is_full_object() const
            {
                return !m_is_summary;
            }


            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata(true);
                for (const auto& it : attributes)
                {
                    j->set(it.first, it.second->encode_summary());
                }
                return DataObject::wrap_payload(j);
            }
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j = encode_metadata();
                for (const auto& it : attributes)
                {
                    j->set(it.first, it.second->encode());
                }
                return DataObject::wrap_payload(j);
            }

            /// decode the raw json into another type
            /// data type has not been registered as in python, downcast check is not available
            template <typename TargetT> typename TargetT::Ptr cast()
            {
                return TargetT::decode(m_json);
            }
            static DataObject::Ptr decode(const Poco::JSON::Object::Ptr j)
            {
                const Poco::JSON::Object::Ptr json = DataObject::unwrap_payload(json);
                DataObject::Ptr p = new DataObject("data_object");
                DataObject::decode_metadata(json, p);
                bool is_summary = DataObject::is_summary_content(json);
                if (is_summary)
                {
                    p->m_is_summary = true; // todo: check any difference in decoding
                }
                else
                {
                    p->m_is_summary = false;
                }
                try
                {
                    decode_content(json, p);
                }
                catch (std::exception& e)
                {
                    SAL_REPORT_DECODE_ERROR(e, json);
                }
                // a new key: URL may has been injected
                // keep the pointer to json data to cast to user type later
                p->m_json = json;
                return p;
            }

            // meta data and description have been constructed as member variable
            // consider move all meta data into a Dictionary data object
            static void remove_metadata(DataObject::Ptr obj)
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

                json->getNames(keys);
                for (std::vector<std::string>::iterator key = keys.begin(); key != keys.end(); ++key)
                {
                    // skip null elements
                    if (json->isNull(*key))
                        continue;

                    if (!json->isObject(*key))
                        throw SALException("all valid attributes must be JSON object not number or std::string");

                    /// dispatch object to the appropriate decoder and add to container
                    auto item_json = json->getObject(*key);
                    if (is_data_object(item_json))
                    {
                        obj->set(*key, decode(item_json));
                    }
                    else
                    {
                        obj->set(*key, sal::object::decode(item_json));
                    }
                }

                // remove extracted meta data in the for loop
                remove_metadata(obj);
            };

        protected:
            std::string m_dtype;
            std::string m_url; // CONSIDER: keep path name here to identify this unique signal

            std::string m_class_name;
            std::string m_group_name; // GROUP
            uint64_t m_version;
            std::string m_description;

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
            const auto attr = this;
            Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

            // to be python compatible, remove this block after further refactoring,
            auto a = new String(attr->class_name());
            json->set("_class", a->encode());

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

            auto a_group_name = new String(attr->m_group_name);
            json->set("_group", a_group_name->encode());
            // todo: this is per object version
            auto a_version = new UInt64(SAL_API_VERSION);
            json->set("_version", a_version->encode());

            // todo: deal with description is empty?  also UUID for data object
            auto a_description = new String(attr->m_description);
            json->set("description", a_description->encode());
            return json;
        }

        /// in python it is classmethod, because metadata are static variable
        /// in C++, these metadata are instance members for the moment
        bool is_compatible()
        {
            return false;
        }

        /*
        Attempts to decode a JSON object into the specified SAL object.
        Returns null pointer if cast is invalid.
        */
        template <class T> typename T::Ptr decode_object_as(const Poco::JSON::Object::Ptr json)
        {
            // todo: type matching validation
            return T::decode(json);
        };

        void DataObject::decode_metadata(const Poco::JSON::Object::Ptr j, DataObject::Ptr attr)
        {
            attr->m_description = String::decode(j->getObject("description"))->value();
            auto obj_type = String::decode(j->getObject("_type"))->value();
            attr->m_is_summary = obj_type == "summary";

            // _group, _class, _version should be static
            // _version is also fixed at compiling time, but can be checked
            attr->m_group_name = String::decode(j->getObject("_group"))->value();
            // m_type_name is fixed at object creation time
            // better to check, Atomic<> change type to scaler during encoding
            // attr->m_type_name = String::decode(j->getObject("_class"))->value();
        }
    } // namespace dataclass
} // namespace sal