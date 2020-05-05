#pragma once

#include "sal_data.h"
#include "sal_exception.h"

#include "Poco/DateTimeFormatter.h"

namespace sal
{
    /// tree-node structure supports std::string path like in file system
    /// is there any third-party library provide this data structure?
    namespace node
    {
        typedef enum
        {
            NODE_BRANCH,
            NODE_LEAF
        } NodeType;


        // TODO: those global names should be removed
        // static char JSON_CONTENT_REPORT[] = "report"; // should be named as "summary"
        // static char JSON_CONTENT_OBJECT[] = "object";

        // static char OBJ_TYPE_FULL[] = "full";
        // static char OBJ_TYPE_SUMMARY[] = "summary";

        /// corresponding to python decorator `dataobject.register()`
        /// Refactored: rename from NodeType to NodeInfo mimic FileInfo class
        /// TODO: data byte size, permission
        class NodeInfo
        {

        public:
            // TODO: set a proper default values for members
            NodeInfo()
                    : version(SAL_API_VERSION){};
            NodeInfo(const std::string _cls, const std::string _group, const uint64_t _version)
                    : cls(_cls)
                    , group(_group)
                    , version(_version){};

            std::string cls;   /// todo: make them const
            std::string group; /// permission
            uint64_t version;  // API version ?

            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j = new Poco::JSON::Object();
                ;
                j->set("class", cls);
                j->set("group", group);
                j->set("version", version);
                return j;
            }

            /// Attribute::encode_header() for object
            /*
            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j;
                j->set("_class", cls);
                j->set("_group", group);
                j->set("_version", version);
                return j;
            }
            */

            static NodeInfo decode(Poco::JSON::Object::Ptr j)
            {
                NodeInfo i{j->getValue<std::string>("class"), j->getValue<std::string>("group"),
                           j->getValue<uint64_t>("version")};
                return i;
            }
        };

        ///  map to std::filesystem::perms or  OPC-UA AccessLevel, UserAccessLevel
        /// RolePermissions
        /// https://github.com/FreeOpcUa/python-opcua/blob/master/opcua/ua/uatypes.py
        /// this should be a member of NodeObject, may be implemented at client class is easier
        enum PermissionFlag : uint32_t
        {
            CurrentRead = 0,
            CurrentWrite = 1,
            HistoryRead = 2,
            HistoryWrite = 3,
            SemanticChange = 4,
            StatusWrite = 5,
            TimestampWrite = 6
        };

        /// std::filesystem can only check `last_write_time()`
        /// however, server may not support those time stamps
        /// consider, merge timestamp into RevisionInfo
        class TimeInfo
        {
        public:
            TimeInfo(){}; // TODO: set a proper default values for members
            Poco::DateTime modified;

            std::string timestamp() const
            {
                return Poco::DateTimeFormatter::format(modified, TimeInfo::ts_format());
            }
            static std::string ts_format()
            {
                return "%Y-%m-%dT%H:%M:%S.%F";
            }
#if defined(SAL_TIMEINFO_EXTRA)
            Poco::DateTime created;
            Poco::DateTime accessed;
            //
            static TimeInfo decode(Poco::JSON::Object::Ptr j)
            {
                TimeInfo i;
                int timeZoneDifferential;
                auto modified_ts = j->getValue<std::string>("modified");
                i.modified = Poco::DateTimeParser::parse(TimeInfo::ts_format(), modified_ts, timeZoneDifferential);
                return i;
            }
#endif
            Poco::JSON::Object::Ptr encode()
            {
                auto ts = Poco::DateTimeFormatter::format(modified, TimeInfo::ts_format());
                Poco::JSON::Object::Ptr j = new Poco::JSON::Object();
                j->set("modified", ts);
                return j;
            }

            static TimeInfo decode(std::string modified_ts)
            {
                TimeInfo i;
                int timeZoneDifferential;
                i.modified = Poco::DateTimeParser::parse(TimeInfo::ts_format(), modified_ts, timeZoneDifferential);
                return i;
            }
        };

        /// integer version, how about timestamp, why not a fixed width int
        /// https://simple-access-layer.github.io/documentation/datamodel/datatree.html#permissions-tree
        class RevisionInfo
        {
        public:
            // TODO: proper default member values
            RevisionInfo()
                    : current(3)
                    , latest(3)
            {
                // version_history = {};
            }
            uint64_t current;
            uint64_t latest;
            std::vector<uint64_t> version_history;

            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j = new Poco::JSON::Object();
                j->set("latest", latest);
                j->set("current", current);
                j->set("modified", version_history);
                return j;
            }
            static RevisionInfo decode(Poco::JSON::Object::Ptr j)
            {
                RevisionInfo i;
                i.current = j->getValue<uint64_t>("current");
                i.latest = j->getValue<uint64_t>("latest");
                auto arr = j->getArray("modified");
                if (arr)
                {
                    for (size_t idx = 0; idx < arr->size(); idx++)
                    {
                        i.version_history.push_back(arr->getElement<uint64_t>(idx));
                    }
                }
                return i;
            }
        };

        /**
        /// split this class into Object (for decode,encode) and Branch ()
        class Report
        {

        public:
            typedef Poco::SharedPtr<Report> Ptr;

            Report();
            ~Report();

            const NodeType type;
            const std::string timestamp;        // todo: use something better std::chrono::time_point?
            const std::vector<std::string> branches; // std::string should be node::Path typedef
            const std::vector<NodeInfo> leaves; // the only place use LeafType, move to Branch class

            // needs copy and move constructors
        };
        */

        /// base class for both Branch and Leaf, rename to NodeObject
        class NodeObject // : public object::Attribute
        {
        protected:
            // TODO: if all meta data applied to Leaf only, move to class Leaf
            NodeType m_node_type;

            NodeInfo m_nodeInfo;
            TimeInfo m_timeInfo;
            RevisionInfo m_revisionInfo;
            // status, error, etc
            std::string m_display_name;
            std::string m_url; // URL as NodeId or UUID as in OpcUa

            // if NodeObject does not derive from Attribute
            std::string m_description;
            NodeObject(){};

        public:
            typedef Poco::SharedPtr<NodeObject> Ptr;

            NodeObject(const NodeInfo& _nodeInfo, const std::string _description)
                    //, object::Attribute(object::ATTR_NODE, _description)
                    : m_node_type(NODE_LEAF)
                    , m_nodeInfo(_nodeInfo)
            {
            }

            virtual ~NodeObject() = default;


            bool is_leaf()
            {
                return m_node_type == NodeType::NODE_LEAF;
            }
            bool is_branch()
            {
                return m_node_type == NodeType::NODE_BRANCH;
            }


            // TODO: simulate the property to meta info
            /// setter
            NodeInfo& nodeInfo()
            {
                return m_nodeInfo;
            }
            /// getter
            const NodeInfo& nodeInfo() const
            {
                return m_nodeInfo;
            }

            std::string name() const
            {
                return m_display_name;
            }
            std::string& name()
            {
                return m_display_name;
            }

            virtual Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j = new Poco::JSON::Object();
                auto obj = m_nodeInfo.encode();
                obj->set("display_name", m_display_name);
                j->set("object", obj); // object is not a good name -> metadata

                j->set("revision", m_revisionInfo.encode());
                j->set("timestamp", m_timeInfo.timestamp());
                j->set("description", m_description);
                // j->set("url", m_url);

                // j->set("permission", );
                return j;
            };


            std::string display_name_from_url(const std::string url) const
            {
                return url; // todo
            }
            ///
            void decode_report(const Poco::JSON::Object::Ptr j)
            {
                this->m_description = j->getValue<std::string>("description");
                this->m_nodeInfo = NodeInfo::decode(j->getObject("object"));
                this->m_revisionInfo = RevisionInfo::decode(j->getObject("revision"));
#if SAL_TIMEINFO_EXTRA
                this->m_timeInfo = TimeInfo::decode(j->getObject("timestamp"));
#else
                this->m_timeInfo = TimeInfo::decode(j->getValue<std::string>("timestamp"));
#endif
                // newly added into C++
                // this->m_display_name = display_name_from_url(j->getValue<std::string>("url"));
            }

            /// SummaryInterface, not necessary for nodeoject
            /// core.dataclass._new_dict() with common header info
            /*
            Returns a Poco JSON object summary of the data object.
            corresponding to python DataSummary class `to_dict()`
            only container derived class like Array and Dictionary need to override
            */
            virtual Poco::JSON::Object::Ptr encode_summary() const
            {
                return encode();
            }
        };



        /// Container of DataEntry (Attribute)
        class Leaf : public NodeObject
        {
            /// Object::Dictionary holds only value types, this holds ptr
            std::map<std::string, object::Attribute::Ptr> attributes;

            object::Attribute::Ptr m_data; // ptr to Signal or Dictionary Attribute class
            std::string m_data_type_name;  // dictionary | signal

            Leaf(){};

        public:
            typedef Poco::SharedPtr<Leaf> Ptr;

            /*
            Constructors and destructor.
            */
            Leaf(const NodeInfo& nInfo, const std::string desc, bool _is_summary = true)
                    : NodeObject(nInfo, desc)
            {
                // m_is_summary = true;
            }
            virtual ~Leaf(){};

            bool is_full_object() const
            {
                return false;
            }

            /**
            from a SAL data object to a Poco JSON object representation.
            */
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j = NodeObject::encode();
                return j;
            }

#if 0
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
            const bool has(const std::string& key) const
            {
                return this->attributes.count(key);
            };
            void remove(const std::string& key)
            {
                this->attributes.erase(key);
            };

            // meta data and description have been constructed as member variable
            // consider move all meta data into a Dictionary data object
            void remove_meta_attributes()
            {
                Leaf* obj = this;
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
            static Leaf::Ptr decode(const Poco::JSON::Object::Ptr json)
            {
                std::vector<std::string> keys;
                Leaf::Ptr obj;

                // treat any failure as a failure to decode
                try
                {
                    // extract meta data: class, group, version and object type
                    object::String::Ptr cls = object::decode_as<object::String>(json->getObject("_class"));
                    object::String::Ptr group = object::decode_as<object::String>(json->getObject("_group"));
                    object::UInt64::Ptr version = object::decode_as<object::UInt64>(json->getObject("_version"));
                    auto type = object::decode_as<object::String>(json->getObject("_type"));
                    object::String::Ptr description = object::decode_as<object::String>(json->getObject("description"));

                    // create object and populate
                    NodeInfo nInfo{cls->value(), group->value(), version->value()};
                    obj = new Leaf(nInfo, description->value(), type->value() == OBJ_TYPE_SUMMARY);

                    json->getNames(keys);
                    for (vector<std::string>::iterator key = keys.begin(); key != keys.end(); ++key)
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
                    obj->remove_meta_attributes();

                    return obj;
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL data object.");
                }
            };
#endif
            static Leaf::Ptr decode(Poco::JSON::Object::Ptr json)
            {
                Leaf::Ptr leaf = new Leaf();
                leaf->decode_report(json);
                /* treat any failure as a failure to decode
                try
                {

                    // no other content for leaf in report mode
                }
                catch (...)
                {
                    throw SALException("JSON object does not define a valid SAL leaf node object.");
                }
                */
                return leaf;
            };

        }; // namespace node

        /// Only information, no operation
        class Branch : public NodeObject
        {
            // full object, empty if constructed from summary json
            std::vector<std::string> branches;
            std::vector<NodeObject::Ptr> leaves;

            Branch()
            {
                m_node_type = NODE_BRANCH;
                // m_is_summary = true;
            }

        public:
            typedef Poco::SharedPtr<Branch> Ptr;

            /**
            Constructors and destructor.
            */
            Branch(const NodeInfo& nInfo, const std::string desc)
                    : NodeObject(nInfo, desc)
            {
                m_node_type = NODE_BRANCH;
                // m_is_summary = true;
            }
            virtual ~Branch(){};

            /**
            from a SAL data object to a Poco JSON object representation.
            */
            virtual Poco::JSON::Object::Ptr encode() const
            {
                // note: summary JSON is not part of the full object JSON
                Poco::JSON::Object::Ptr j = NodeObject::encode();
                j->set("children", encode_content());
                return j;
            }

            // https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf
            Poco::JSON::Object::Ptr encode_content() const
            {
                Poco::JSON::Object::Ptr j;
                // todo: "children" each child name type and version
                Poco::JSON::Array::Ptr j_leaves;
                Poco::JSON::Array::Ptr j_branches;
                for (const auto& l : leaves)
                {
                    auto jj = l->nodeInfo().encode();
                    jj->set("name", l->name());
                    j_leaves->add(jj);
                }
                j->set("leaves", j_leaves);

                for (const auto& l : branches)
                {
                    j_branches->add(l);
                }
                j->set("branches", j_branches);
                return j;
            }

            // children node content should not be
            void decode_content(Poco::JSON::Object::Ptr j)
            {
                auto j_leaves = j->getArray("leaves");
                for (auto& l : *j_leaves)
                {
                    // todo: not working code
                    auto lp = Leaf::decode(l.extract<Poco::JSON::Object::Ptr>());
                    leaves.push_back(lp);
                }

                branches.clear();
                auto j_branches = j->getArray("branches");
                for (const auto& l : *j_branches)
                {
                    branches.push_back(l.extract<std::string>());
                }
            }

            /*
            Decodes a Poco JSON object representation of the data object and returns a SAL data object.
            */
            static Branch::Ptr decode(Poco::JSON::Object::Ptr json)
            {
                Branch::Ptr branch;
                // check sal type is valid for this class
                // if (json->getValue<std::string>("_class") != "node")
                //    throw SALException("data type does not match node objects");

                // treat any failure as a failure to decode
                try
                {
                    // std::string description = json->getValue<std::string>("description");
                    // NodeInfo nInfo = NodeInfo::decode(json);
                    // branch = new Branch(nInfo, description);
                    branch = new Branch();
                    branch->decode_report(json);

                    // std::string content_type = json->getValue<std::string>("summary");
                    // if (content_type == "report") // todo: check the name

                    branch->decode_content(json);
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL branch object.");
                }
                return branch;
            };
        };

        /*
         A factory-pattern method attempts to decode a JSON object (http response)
          into a SAL node object.
          branch has only one kind of serialization mode
          https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn?content=report
          leaf has report, object (full, summary)
         https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=full
         https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla?object=summary
         "content" = object
         https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn/ipla "content" = report
        */
        NodeObject::Ptr decode(Poco::JSON::Object::Ptr json)
        {

            std::string content_type;
            std::string node_type;
            std::string url;
            Poco::JSON::Object::Ptr object;

            try
            {
                content_type = json->getValue<std::string>("content");

                node_type = json->getValue<std::string>("type");

                if (!json->isObject("object"))
                    throw std::exception();
                object = json->getObject("object");
                if (json->has("request"))
                {
                    url = json->getObject("request")->getValue<std::string>("url");
                    object->set("url", url);
                }
            }
            catch (...)
            {
                // todo: define a more specific sal exception and replace
                throw SALException("JSON object does not define a valid SAL object.");
            }

            if (content_type == "object")
            {
                /*
                std::string data_type;
                data_type = object->getValue<std::string>("_class");

                if (node_type == "leaf")
                    return Leaf::decode(object);
                    */
            }
            else if (content_type == "report") //
            {
                if (node_type == "branch")
                    return Branch::decode(object);
                if (node_type == "leaf")
                    return Leaf::decode(object);
            }
            else
            {
                throw SALException("content type is unkown in json data");
            }

            throw SALException("JSON object does not define a valid SAL object.");
        }


        /*
        Attempts to decode a JSON object into the specified SAL object.

        Returns null pointer if cast is invalid.
        */
        template <class T> typename T::Ptr decode_object_as(Poco::JSON::Object::Ptr json)
        {
            return typename T::Ptr(object::decode(json).cast<T>());
        };

    }; // namespace node
} // namespace sal