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

        // variable identifier strings in serialised objects
        char VAR_KEY_INT8[] = "int8";
        char VAR_KEY_INT16[] = "int16";
        char VAR_KEY_INT32[] = "int32";
        char VAR_KEY_INT64[] = "int64";
        char VAR_KEY_UINT8[] = "uint8";
        char VAR_KEY_UINT16[] = "uint16";
        char VAR_KEY_UINT32[] = "uint32";
        char VAR_KEY_UINT64[] = "uint64";
        char VAR_KEY_FLOAT32[] = "float32";
        char VAR_KEY_FLOAT64[] = "float64";
        char VAR_KEY_BOOL[] = "bool";
        char VAR_KEY_STRING[] = "string";
        char VAR_KEY_ARRAY[] = "array";
        char VAR_KEY_BRANCH[] = "branch";

    };

};
