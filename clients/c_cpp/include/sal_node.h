#pragma once

#include "sal_data.h"
#include "sal_exception.h"
#include <cassert>

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
        /// each item in the Branch children list contains this information
        /// Refactored: rename from LeafType to NodeInfo mimic FileInfo class
        class NodeInfo
        {

        public:
            // TODO: set a proper default values for members
            NodeInfo()
                    : version(SAL_API_VERSION){};
            NodeInfo(const std::string _cls, const std::string _group, const uint64_t _version,
                     const std::string _name = "")
                    : cls(_cls)
                    , group(_group)
                    , version(_version)
                    , name(_name){};

            std::string cls;   /// todo: make them const
            std::string group; /// permission
            uint64_t version;  // API version ?
            std::string name;

            Poco::JSON::Object::Ptr encode() const
            {
                Poco::JSON::Object::Ptr j = new Poco::JSON::Object();
                j->set("class", cls);
                j->set("group", group);
                j->set("version", version);
                // if (name.size())
                j->set("name", name);
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
                std::string _name;
                if (j->has("name"))
                    _name = j->getValue<std::string>("name");
                NodeInfo i{j->getValue<std::string>("class"), j->getValue<std::string>("group"),
                           j->getValue<uint64_t>("version"), _name};
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
                if (j->has("object")) // Branch json from server has no object info
                {
                    this->m_nodeInfo = NodeInfo::decode(j->getObject("object"));
                }
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


        ///  This is NOT data object i.e. containter of DataEntry (Attribute)
        ///  but meta data of NodeObject, decoded from `list()` report
        class Leaf : public NodeObject
        {
            /// CONSIDER:  remove those two fields,
            object::Attribute::Ptr m_data; // ptr to data content
            std::string m_data_type_name;

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

            /**
            from a SAL data object to a Poco JSON object representation.
            */
            virtual Poco::JSON::Object::Ptr encode() const override
            {
                Poco::JSON::Object::Ptr j = NodeObject::encode();
                return j;
            }


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
            std::vector<NodeInfo> leaves;

            /// This private constructor will be initialized in decode()
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
                Poco::JSON::Object::Ptr j = new Poco::JSON::Object();
                // todo: "children" each child name type and version
                Poco::JSON::Array::Ptr j_leaves = new Poco::JSON::Array();
                Poco::JSON::Array::Ptr j_branches = new Poco::JSON::Array();
                for (const auto& l : leaves)
                {
                    auto jj = l.encode();
                    // jj->set("name", l->name());
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

            // children node content
            void decode_content(Poco::JSON::Object::Ptr jObj)
            {
                auto j = jObj->getObject("children");
                assert(j->has("leaves") && j->has("branches"));
                auto j_leaves = j->getArray("leaves");
                // leaves and branches should be empty
                for (auto& l : *j_leaves)
                {
                    auto lp = NodeInfo::decode(l.extract<Poco::JSON::Object::Ptr>());
                    leaves.push_back(lp);
                }

                // branches.clear();
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
                Branch::Ptr branch = new Branch();
                // check sal type is valid for this class
                // if (json->getValue<std::string>("_class") != "node")
                //    throw SALException("data type does not match node objects");

                branch->decode_report(json);

                // std::string content_type = json->getValue<std::string>("summary");
                // if (content_type == "report") // todo: check the name

                branch->decode_content(json);
                // treat any failure as a failure to decode
                try
                {
                }
                catch (...)
                {
                    throw SALException("can not decode a valid SAL branch object from json object");
                }
                return branch;
            };
        };

        /*
         A factory-pattern method attempts to decode a JSON object (http response)
          into a SAL node object.
          branch has only one kind of json representation (Report in Python)
          https://sal.jet.uk/data/pulse/83373/ppf/signal/jetppf/magn?content=report
          leaf has `report` returned by `list()`,
          and also `object (full, summary)` returned by `get()`
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

            if (content_type == "report") //
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