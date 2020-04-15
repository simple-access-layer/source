#pragma once

#include "sal_data.h"
#include "sal_exception.h"


namespace sal
{
    using namespace std;

    /// tree-node structure supports string path like in file system
    /// is there any third-party library provide this data structure?
    namespace node
    {
        // where does this used?
        char VALID_CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789-_.";

        typedef enum
        {
            NODE_BRANCH,
            NODE_LEAF
        } NodeType;
        /// node type names
        char JSON_TYPE_LEAF[] = "leaf";
        char JSON_TYPE_BRANCH[] = "branch";

        //
        char JSON_CONTENT_REPORT[] = "report";
        char JSON_CONTENT_OBJECT[] = "object";

        char OBJ_TYPE_FULL[] = "object";
        char OBJ_TYPE_SUMMARY[] = "summary";

        /// corresponding to python decorator `dataobject.register()`
        /// Refactored: rename from NodeType to NodeInfo mimic FileInfo class
        /// TODO: data byte size, permission
        class NodeInfo
        {

        public:
            NodeInfo(); // TODO: set a proper default values for members
            NodeInfo(const string _cls, const string _group, const int _version)
                    : cls(_cls)
                    , group(_group)
                    , version(_version){};

            const string cls;       /// ?
            const string group;     /// permission
            const uint64_t version; // API version
        };

        /// however, server may not support those time stamp
        class TimeInfo
        {
        public:
            TimeInfo(); // TODO: set a proper default values for members
            Poco::Timestamp created;
            Poco::Timestamp modified;
            Poco::Timestamp accessed;
        };

        /// integer version, how about timestamp, why not a fixed width int
        class RevisionInfo
        {
        public:
            RevisionInfo(); // TODO: proper default member values
            const uint64_t current;
            const uint64_t latest;
            vector<uint64_t> version_history;
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
        class NodeObject
        {
        protected:
            const NodeType m_type;
            string m_description; // ?  `name` in data tree?
            // TODO: if all meta data applied to Leaf only, move to class Leaf
            NodeInfo m_nodeInfo;
            TimeInfo m_timeInfo;
            RevisionInfo m_revisionInfo;
            // status, error, etc

        public:
            typedef Poco::SharedPtr<NodeObject> Ptr;

            NodeObject(const NodeInfo& _nodeInfo, const string _description, const NodeType _type)
                    : m_nodeInfo(_nodeInfo)
                    , m_description(_description)
                    , m_type(_type){};
            virtual ~NodeObject();

            bool is_leaf()
            {
                return m_type == NodeType::NODE_LEAF;
            }
            bool is_branch()
            {
                return m_type == NodeType::NODE_BRANCH;
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
                    : NodeObject(nInfo, desc, NODE_BRANCH)
            {
            }
            virtual ~Branch(){};

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
                    NodeInfo nInfo;
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
            // static const NodeType type = NODE_LEAF;

            /*
            Constructors and destructor.
            */
            Leaf(const NodeInfo& nInfo, const string desc, bool _is_summary)
                    : NodeObject(nInfo, desc, NODE_LEAF)
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

            /*
            from a SAL data object to a Poco JSON object representation.
            todo:
            static Poco::JSON::Object::Ptr Leaf::encode(const Leaf::Ptr)
            {

            }
            */

            /*
            Decodes a Poco JSON object representation of the data object and returns a SAL data object.
            */
            static Leaf::Ptr decode(Poco::JSON::Object::Ptr json)
            {

                vector<string> keys;
                vector<string>::iterator key;
                Leaf::Ptr obj;

                // treat any failure as a failure to decode
                try
                {

                    // extract class, group, version and object type
                    object::String::Ptr cls = object::decode_as<object::String>(json->getObject("_class"));
                    object::String::Ptr group = object::decode_as<object::String>(json->getObject("_group"));
                    object::UInt64::Ptr version = object::decode_as<object::UInt64>(json->getObject("_version"));
                    object::String::Ptr type = object::decode_as<object::String>(json->getObject("_type"));
                    object::String::Ptr description = object::decode_as<object::String>(json->getObject("description"));

                    // create object and populate
                    NodeInfo nInfo{cls->value(), group->value(), version->value()};
                    obj = new Leaf(nInfo, description->value(), type->value() == OBJ_TYPE_SUMMARY);
                    json->getNames(keys);
                    for (key = keys.begin(); key != keys.end(); ++key)
                    {

                        // skip null elements
                        if (json->isNull(*key))
                            continue;

                        // all valid attributes definitions are JSON objects
                        if (!json->isObject(*key))
                            throw std::exception();

                        // dispatch object to the appropriate decoder
                        obj->set(*key, sal::object::decode(json->getObject(*key)));
                    }

                    // remove extracted items
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
        object::Attribute::Ptr decode_object(Poco::JSON::Object::Ptr json)
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

            // if (type == JSON_TYPE_BRANCH)
            //    return Branch::decode(object);
            // if (type == JSON_TYPE_LEAF)
            //    return Leaf::decode(object);
            return object::decode(object);

            // todo: define a sal exception and replace
            throw runtime_error("JSON object does not define a valid SAL object.");
        }


        /*
        Attempts to decode a JSON object into the specified SAL object.

        Returns null pointer if cast is invalid.
        */
        template <class T> typename T::Ptr decode_object_as(Poco::JSON::Object::Ptr json)
        {
            return typename T::Ptr(decode_object(json).cast<T>());
        };


        /*
        Attempts to decode a JSON object into a SAL object.
        */
        NodeObject::Ptr decode_node(Poco::JSON::Object::Ptr json){
            // TODO: implement me
        };
    }; // namespace node
} // namespace sal