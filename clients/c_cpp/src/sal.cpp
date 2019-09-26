/*
tasks to do:

* json serialisation and deserialisation
* report objects
* typed object serialisation and deserialisation
* exceptions
* authentication methods
* list
* get
* put
* copy
* delete
* tests
* document
* beta test
* release


Architecture

Client class to connect to server.
Object class - Anything client returns is derived from this - reports and data objects.
Report class - represents a leaf or branch report.


*/
#include "sal.h"

namespace sal {

    char VALID_CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789-_.";

    namespace object {

        // attribute identifier strings in serialised objects
        char ATTR_INT8[] = "int8";
        char ATTR_INT16[] = "int16";
        char ATTR_INT32[] = "int32";
        char ATTR_INT64[] = "int64";
        char ATTR_UINT8[] = "uint8";
        char ATTR_UINT16[] = "uint16";
        char ATTR_UINT32[] = "uint32";
        char ATTR_UINT64[] = "uint64";
        char ATTR_FLOAT32[] = "float32";
        char ATTR_FLOAT64[] = "float64";
        char ATTR_BOOL[] = "bool";
        char ATTR_STRING[] = "string";
        char ATTR_ARRAY[] = "array";
        char ATTR_BRANCH[] = "branch";

    };

};
