#pragma once

#include "sal_data.h"
#include "sal_exception.h"

#include "Poco/DateTimeFormatter.h"

namespace sal
{
    using namespace std;

    /// tree-node structure supports string path like in file system
    /// is there any third-party library provide this data structure?
    namespace node
    {
        /// valid char used in node path
        char VALID_CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789-_.";

        typedef enum
        {
            NODE_BRANCH,
            NODE_LEAF
        } NodeType;


        // TODO: those global names should be removed
        static char JSON_CONTENT_REPORT[] = "report"; // should be named as "summary"
        static char JSON_CONTENT_OBJECT[] = "object";

        static char OBJ_TYPE_FULL[] = "full";
        static char OBJ_TYPE_SUMMARY[] = "summary";

        /// corresponding to python decorator `dataobject.register()`
        /// Refactored: rename from NodeType to NodeInfo mimic FileInfo class
        /// TODO: data byte size, permission
        class NodeInfo
        {

        public:
            // TODO: set a proper default values for members
            NodeInfo()
                    : version(SAL_API_VERSION){};
            NodeInfo(const string _cls, const string _group, const uint64_t _version)
                    : cls(_cls)
                    , group(_group)
                    , version(_version){};

            string cls;       /// todo: make them const
            string group;     /// permission
            uint64_t version; // API version ?

            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j;
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

        ///  map to std::filesystem::perms
        /// OPC-UA AccessLevel, may be implemented at client class is easier
        /// this should be a member of NodeObject
        enum PermissionFlag : uint32_t
        {

        };

        /// std::filesystem can only check `last_write_time()`
        /// however, server may not support those time stamps
        /// consider, merge timestamp into RevisionInfo
        class TimeInfo
        {
        public:
            TimeInfo(){}; // TODO: set a proper default values for members
            Poco::DateTime modified;
            // const static std::string format = "%Y-%m-%dT%H:%M:%S.%f";

            std::string timestamp() const
            {
                return Poco::DateTimeFormatter::format(modified, format());
            }
            std::string format() const
            {
                return "%Y-%m-%dT%H:%M:%S.%f";
            }
#if defined(SAL_TIMEIFO_EXTRA)
            Poco::DateTime created;
            Poco::DateTime accessed;
#endif
            Poco::JSON::Object::Ptr encode()
            {
                auto ts = Poco::DateTimeFormatter::format(modified, format());
                Poco::JSON::Object::Ptr j;
                j->set("modified", ts);
                return j;
            }
            static TimeInfo decode(Poco::JSON::Object::Ptr j)
            {
                TimeInfo i;
                int timeZoneDifferential;
                auto modified_ts = j->getValue<std::string>("modified");
                std::string format = "%Y-%m-%dT%H:%M:%S.%f";
                i.modified = Poco::DateTimeParser::parse(format, modified_ts, timeZoneDifferential);
                return i;
            }
        };

        /// integer version, how about timestamp, why not a fixed width int
        /// https://simple-access-layer.github.io/documentation/datamodel/datatree.html#permissions-tree
        class RevisionInfo
        {
        public:
            RevisionInfo(){}; // TODO: proper default member values
            uint64_t current;
            uint64_t latest;
            vector<uint64_t> version_history;

            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j;
                j->set("latest", latest);
                j->set("current", current);
                j->set("modified", version_history);
                return j;
            }
            static RevisionInfo decode(Poco::JSON::Object::Ptr)
            {
                RevisionInfo i;
                // todo:
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
            const string timestamp;        // todo: use something better std::chrono::time_point?
            const vector<string> branches; // string should be node::Path typedef
            const vector<NodeInfo> leaves; // the only place use LeafType, move to Branch class

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
            std::string m_name;
            std::string m_url; // URL as UUID

            // if NodeObject does not derive from Attribute
            std::string m_description;

        public:
            typedef Poco::SharedPtr<NodeObject> Ptr;
            NodeObject(){};
            NodeObject(const NodeInfo& _nodeInfo, const string _description)
                    : m_nodeInfo(_nodeInfo)
                    //, object::Attribute(object::ATTR_NODE, _description)
                    , m_node_type(NODE_LEAF){};
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
                return m_name;
            }
            std::string& name()
            {
                return m_name;
            }

            virtual Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j;
                auto oj = m_nodeInfo.encode();
                oj->set("name", m_name);
                j->set("object", oj); // object is not a good name -> metadata
                j->set("revision", m_revisionInfo.encode());
                j->set("timestamp", m_timeInfo.timestamp());
                j->set("description", m_description);

                // j->set("permission", );
                // j->set("url", );
                return j;
            };


            std::string name_from_url(const std::string url) const
            {
                return url; // todo
            }
            ///
            void decode_report(const Poco::JSON::Object::Ptr j)
            {
                this->m_nodeInfo = NodeInfo::decode(j->getObject("object"));
                this->m_name = name_from_url(j->getValue<std::string>("url"));
                this->m_revisionInfo = RevisionInfo::decode(j->getObject("revision"));
                this->m_timeInfo = TimeInfo::decode(j->getObject("timestamp"));
                this->m_description = j->getValue<std::string>("description");
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
            map<string, object::Attribute::Ptr> attributes;

        public:
            typedef Poco::SharedPtr<Leaf> Ptr;

            /*
            Constructors and destructor.
            */
            Leaf(const NodeInfo& nInfo, const string desc, bool _is_summary = true)
                    : NodeObject(nInfo, desc)
            {
                // m_is_summary = true;
            }
            virtual ~Leaf(){};


            /*
            Constructors and destructor.
            */

            // code duplication, leave contains only one object::Dictionary instance
            object::Attribute::Ptr& operator[](const string& key)
            {
                return this->attributes.at(key);
            };
            object::Attribute::Ptr& get(const string& key)
            {
                return (*this)[key];
            };
            template <class T> typename T::Ptr get_as(const string& key)
            {
                return typename T::Ptr(this->get(key).cast<T>());
            };
            void set(const string& key, const object::Attribute::Ptr& attribute)
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

            /**
            from a SAL data object to a Poco JSON object representation.
            */
            virtual Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j = NodeObject::encode();
                return j;
            }

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
                vector<string> keys;
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
                    for (vector<string>::iterator key = keys.begin(); key != keys.end(); ++key)
                    {
                        // skip null elements
                        if (json->isNull(*key))
                            continue;

                        if (!json->isObject(*key))
                            throw SALException("all valid attributes must be JSON object not number or string");

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
                    throw runtime_error("JSON object does not define a valid SAL data object.");
                }
            };

        }; // namespace node

        /// Only information, no operation
        class Branch : public NodeObject
        {
            // full object, empty if constructed from summary json
            vector<std::string> branches;
            vector<NodeObject::Ptr> leaves;

        public:
            typedef Poco::SharedPtr<Branch> Ptr;

            /**
            Constructors and destructor.
            */
            Branch()
            {
                m_node_type = NODE_BRANCH;
                // m_is_summary = true;
            }
            Branch(const NodeInfo& nInfo, const string desc)
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
                    // string description = json->getValue<string>("description");
                    // NodeInfo nInfo = NodeInfo::decode(json);
                    // branch = new Branch(nInfo, description);
                    branch = new Branch();
                    branch->decode_report(json);

                    // std::string content_type = json->getValue<string>("summary");
                    // if (content_type == "report") // todo: check the name

                    branch->decode_content(json);
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw SALException("JSON object does not define a valid SAL node object.");
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

            string content_type;
            string node_type;
            string url;
            Poco::JSON::Object::Ptr object;

            try
            {
                content_type = json->getValue<string>("content");

                node_type = json->getValue<string>("type");

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
                // todo: define a sal exception and replace
                throw runtime_error("JSON object does not define a valid SAL object.");
            }

            if (content_type == "object")
            {
                /*
                string data_type;
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
                throw runtime_error("content type is unkown in json data");
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