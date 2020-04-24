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

        /// node type names,
        // TODO: those global names should be removed
        static char JSON_CONTENT_REPORT[] = "report";
        static char JSON_CONTENT_OBJECT[] = "object";

        static char OBJ_TYPE_FULL[] = "object";
        static char OBJ_TYPE_SUMMARY[] = "summary";

        /// corresponding to python decorator `dataobject.register()`
        /// Refactored: rename from NodeType to NodeInfo mimic FileInfo class
        /// TODO: data byte size, permission
        class NodeInfo
        {

        public:
            // TODO: set a proper default values for members
            NodeInfo(const string _cls, const string _group, const uint64_t _version)
                    : cls(_cls)
                    , group(_group)
                    , version(_version){};

            const string cls;       /// ?
            const string group;     /// permission
            const uint64_t version; // API version ?

            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j;
                j->set("_class", cls);
                j->set("_group", group);
                j->set("_class", cls);
                return j;
            }

            static NodeInfo decode(Poco::JSON::Object::Ptr)
            {
                NodeInfo i{"", "", 1};
                // todo:
                return i;
            }
        };

        /// should map to std::filesystem::perms
        /// this should be a member of NodeInfo
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
            Poco::Timestamp modified;

            std::string timestamp() const
            {
                static std::string format = "%Y-%m-%dT%H:%M:%S.%f";
                return Poco::DateTimeFormatter::format(modified, format);
            }
#if 0
            Poco::Timestamp created;
            Poco::Timestamp accessed;
            Poco::JSON::Object::Ptr encode()
            {
                static std::string format = "%Y-%m-%dT%H:%M:%S.%f";
                auto ts = Poco::DateTimeFormatter::format(modified, format);
                Poco::JSON::Object::Ptr j;
                j->set(ts);
                return j;
            }
            static TimeInfo decode(Poco::JSON::Object::Ptr)
            {
                TimeInfo i;
                // todo:
                return i;
            }
#endif
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

        /** bad name: this should be Branch
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
        class NodeObject : public object::Attribute
        {
        protected:
            // TODO: if all meta data applied to Leaf only, move to class Leaf
            NodeType m_node_type;

            NodeInfo m_nodeInfo;
            TimeInfo m_timeInfo;
            RevisionInfo m_revisionInfo;
            // status, error, etc

        public:
            typedef Poco::SharedPtr<NodeObject> Ptr;

            NodeObject(const NodeInfo& _nodeInfo, const string _description)
                    : m_nodeInfo(_nodeInfo)
                    , object::Attribute(object::ATTR_NODE, _description)
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

            virtual Poco::JSON::Object::Ptr encode() const
            {
                throw std::runtime_error("must be override by derived");
            };
            /// SummaryInterface
            /// core.dataclass._new_dict() with common header info
            /*
            Returns a Poco JSON object summary of the data object.
            corresponding to python DataSummary class `to_dict()`
            only container derived class like Array and Dictionary need to override
            */
            virtual Poco::JSON::Object::Ptr encode_summary() const override
            {
                Poco::JSON::Object::Ptr j = m_nodeInfo.encode();
                j->set("revision", m_revisionInfo.encode());
                j->set("timestamp", m_timeInfo.timestamp());
                // permission
                // j->set("permission", );
                return j;
            }
        };


        /// Only information, no operation
        class Branch : public NodeObject
        {
            vector<string> branches; // string should be node::Path typedef
            vector<NodeInfo> leaves; // the only place use LeafType, move to Branch class
            // vector<NodeObject::Ptr> branches;
            // vector<NodeObject::Ptr> leaves;
        public:
            typedef Poco::SharedPtr<Branch> Ptr;
            // static const NodeType type = NODE_BRANCH;

            /*
            Constructors and destructor.
            */
            Branch(const NodeInfo& nInfo, const string desc)
                    : NodeObject(nInfo, desc)
            {
                m_node_type = NODE_BRANCH;
            }
            virtual ~Branch(){};

            /**
            from a SAL data object to a Poco JSON object representation.
            */
            virtual Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j;
                // todo:
                return j;
            }

            /*
            Decodes a Poco JSON object representation of the data object and returns a SAL data object.
            */
            static Branch::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                Branch::Ptr branch;

                // treat any failure as a failure to decode
                try
                {
                    string description = json->getValue<string>("description");
                    // TODO:
                    NodeInfo nInfo{"", "", 1};
                    return new Branch(nInfo, description);
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL data object.");
                }
            };
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
            Leaf(const NodeInfo& nInfo, const string desc, bool _is_summary)
                    : NodeObject(nInfo, desc)
                    , is_summary(_is_summary)
            {
            }
            virtual ~Leaf(){};

            const bool is_summary;


            /*
            Constructors and destructor.


            Leaf(string _cls, string _group, uint64_t _version, bool _summary, string _description)
                    : cls(_cls)
                    , group(_group)
                    , version(_version)
                    , summary(_summary)
                    , description(_description){};

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
                Poco::JSON::Object::Ptr j;
                // todo:
                return j;
            }


            /*
            Decodes a Poco JSON object representation of the data object and returns a SAL data object.
            */
            static Leaf::Ptr decode(const Poco::JSON::Object::Ptr json)
            {

                vector<string> keys;
                Leaf::Ptr obj;

                // treat any failure as a failure to decode
                try
                {
                    // TODO: no need to convert json into data class, then extract value
                    // extract class, group, version and object type
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

                        // all valid attributes definitions are JSON objects
                        if (!json->isObject(*key))
                            throw std::exception();

                        // dispatch object to the appropriate decoder and add to container
                        obj->set(*key, sal::object::decode(json->getObject(*key)));
                    }

                    // remove extracted items, just because those are smart pointer type
                    obj->remove("_class");
                    obj->remove("_group");
                    obj->remove("_version");
                    obj->remove("_type");
                    obj->remove("description");

                    return obj;
                }
                catch (...)
                {
                    // todo: define a sal exception and replace
                    throw runtime_error("JSON object does not define a valid SAL data object.");
                }
            };

        }; // namespace node


        /*
         A factory-pattern method attempts to decode a JSON object into a SAL object.
        */
        NodeObject::Ptr decode(Poco::JSON::Object::Ptr json)
        {

            string content;
            string type;
            Poco::JSON::Object::Ptr object;

            try
            {
                content = json->getValue<string>("content");
                if (content != JSON_CONTENT_OBJECT)
                    throw std::exception();

                type = json->getValue<string>("type");

                if (!json->isObject("object"))
                    throw std::exception();
                object = json->getObject("object");
            }
            catch (...)
            {
                // todo: define a sal exception and replace
                throw runtime_error("JSON object does not define a valid SAL object.");
            }

            if (type == "branch")
                return Branch::decode(object);
            if (type == "leaf")
                return Leaf::decode(object);
            // return node::decode(object);

            // todo: define a sal exception and replace
            throw runtime_error("JSON object does not define a valid SAL object.");
        }


        /*
        Attempts to decode a JSON object into the specified SAL object.

        Returns null pointer if cast is invalid.
        */
        template <class T> typename T::Ptr decode_object_as(Poco::JSON::Object::Ptr json)
        {
            return typename T::Ptr(object::decode(json).cast<T>());
        };


        /*
        Attempts to decode a JSON object into a SAL object.
        NodeObject::Ptr decode_node(Poco::JSON::Object::Ptr json)
        {
            NodeObject::Ptr np;
            return np;
            // TODO: implement me
        };
        */
    }; // namespace node
} // namespace sal