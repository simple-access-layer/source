.. _rest-api:

REST API
========

Client interactions with the data server are performed by via the SAL REST-like HTTP API.

.. note::  **The current API version is 2.**

Before reading this specification, readers should be familiar with the SAL :ref:`data-model`, the JSON data format and have a rudimentary understanding of the Hypertext Transfer Protocol (HTTP).


Making Requests
---------------

To make an HTTP request you will need to identify a suitable HTTP library or tool to make HTTP requests e.g. the python requests package or the c/c++ curl library.

To interact with the server an HTTP request is constructed. The request describes the operation to be performed by the server, such as creating a node or requesting data. If the request is valid, the server will carry out the appropriate action and an HTTP response will be returned. The specific request-response behaviour of the server for each operation will be described in the later sections of this document.


Failed Requests
---------------

To simplify error handling across the API, a common failure response is returned if a request fails. The response will include the following contents::

  Status Code: <STATUS>
  Content-Type: application/json

  {
    message: <MESSAGE>,
    status: <STATUS>,
    exception: <EXCEPTION>
  }

The values returned in the response body are as follows:

 - ``STATUS``: A relevant HTTP status code, e.g. 404 Not Found
 - ``MESSAGE``: A text string describing the reason for the failure.
 - ``EXCEPTION``: A text string specifying the exception raised on the server by the failed request.

For example the following failure response is returned if the user requests a data node that does not exist::

  Status Code: 404 Not Found
  Content-Type: application/json

  {
    message: "The supplied path does not point to a valid node.",
    status: 404,
    exception: "NodeNotFound"
  }


SAL Exceptions
~~~~~~~~~~~~~~

[detail exceptions]

.. _rest-api-authentication:

Authentication
--------------

The SAL server supports optional authentication. SAL uses token based authentication. Tokens are acquired by providing login credentials (username and password) to an ``/auth`` endpoint. Typically a token is time limited and will need to be renewed if the token expires. The token lifetime is configured by the administrator of the server.


Obtaining an Authorisation Token
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This ``/auth`` endpoint is only available if the SAL server requires authentication to access data. Where authentication is required, users must post their credentials to this endpoint to obtain an authorisation token. This token must be provided as part of the request when interacting with endpoints requiring authentication.

Request
+++++++

To authenticate, the client must provide user credentials via HTTP Basic Authentication. The request should take the following form::

  GET /auth HTTP/1.1
  Host: <SERVER>
  Authorization: Basic <ENCODED_DATA>

Headers:

  - ``Authorization``: A string constructed according to the HTTP Basic Authentication specification (see below).

The contents of the ``Authorization`` header must be constructed as follows:

  - The username and password are combined with a single colon (:).
  - The resulting string is encoded using Base64.
  - The string `Basic` is then prepended to the encoded string, separated with a space.

For example, if the browser uses ``Aladdin`` as the username and ``OpenSesame`` as the password, then the field's value is the base64-encoding of ``Aladdin:OpenSesame``, or ``QWxhZGRpbjpPcGVuU2VzYW1l``. Then the Authorization header will appear as::

  Authorization: Basic QWxhZGRpbjpPcGVuU2VzYW1l

Success Response
++++++++++++++++

If the authentication request is successful a response will be generated including the following contents:

::

  Status Code: 200 OK
  Content-Type: application/json

  {
      'authorisation':
      {
          'user': <USERNAME>
          'token': <TOKEN>
      }
  }

Failure Response
++++++++++++++++

If the authentication request fails a response will be generated including the following contents::

  Status Code: 401 Unauthorized
  Content-Type: application/json

  {
    message: "Authentication failed.",
    status: 401,
    exception: "AuthenticationFailed"
  }


Authenticating Requests
~~~~~~~~~~~~~~~~~~~~~~~

On a server that requires authentication the user must supply an authentication token that is obtained from the auth endpoint (see :ref:`rest-api-authentication`). There is a common pattern for authenticating all data tree requests. To authenticate a request the user must pass a token either via a query argument or via a request header. The choice of which method is used is up to the user.

To pass the token via a query argument, the query string ``auth=<TOKEN>`` should be added to the url. For example::

  GET /data/node/path?auth=eyJhbGciOiJIUzI1NiIXVCJ9...TJVA95OrM7E20RMHrHDcEfxjoYZgeFONFh7HgQ

Alternatively, to pass the token directly via the request, the user must add the header as follows ::

  Authorization: Bearer <TOKEN>

For example::

  Authorization: Bearer eyJhbGciOiJIUzI1NiIXVCJ9...TJVA95OrM7E20RMHrHDcEfxjoYZgeFONFh7HgQ


If a valid token is not supplied, the request will fail and a response will be generated including the following contents::

  Status Code: 403 Forbidden
  Content-Type: application/json

  {
    message: "Access denied.",
    status: 403,
    exception: "PermissionDenied"
  }

Inspecting Server
-----------------

The server settings and properties are available from the server root.

At the start of a session, clients should inspect the server API version to ensure the server and client are using the same version of the API. The authentication requirements for the server are also available via this endpoint.

Request
~~~~~~~

The request should take the following form::

  GET / HTTP/1.1
  Host: <SERVER>

The root endpoint does not require authentication.

Success Response
~~~~~~~~~~~~~~~~

If a successful request is made, the server will return a response including the following contents::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "host": <HOST>,
    "api":
    {
      "version": <REST_API_VERSION>
      "requires_auth": <AUTHENTICATED>,
      "resources": <LIST OF RESOURCE ENDPOINTS>,
      "classes": <DICTIONARY DESCRIBING DATA CLASSES>
    },
    "service":
    {
      "name": "Simple Access Layer (SAL) Server",
      "version": <SERVER VERSION>
    },
    "request":
    {
      "url": <REQUEST_URL>
    }
  }



Data Tree Operations
--------------------

The following operations are used to obtain data from, or modify the contents of the data tree.

Where operations return data about the data-tree, the response follows a common pattern and will contain the following structure::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": <CONTENT_TYPE>,
    "type": <NODE_TYPE>,
    "object":
    {
      <OBJECT>
    },
    "request":
    {
      "url": <REQUEST_URL>
    }
  }

The ``CONTENT_TYPE`` takes the values ``report`` or ``object``. A report describes the contents of the node. An object response contains the node contents as a JSON encoded object.

The ``type`` attribute specifies whether the response is describing a branch or leaf node. ``NODE_TYPE`` will take the values ``branch`` or ``leaf`` respectively.

The ``object`` attribute will contain the report contents or an encoded data object.

.. _rest-api-encoding:

Data Object Encoding
~~~~~~~~~~~~~~~~~~~~

Data class structures contain data types that are not directly representable in JSON. For instance JSON has no native ``int8`` type, only a generic ``number`` type that represents both integer and floating point numbers as strings. Data types sent via JSON are automatically promoted to the closest JSON type. To ensure the data class structures are correctly transported and the type information can be recovered, the data class structures are encoded before transmission via json.

The original type is store alongside the JSON type promoted data. The different SAL data class types are represented as follows.

Branches:
+++++++++

Branch types are represented with the following JSON structure::

  {
    "type": "branch",
    "value: {
       <CHILD_STRUCTURE>
    }
  }

The ``CHILD_STRUCTURE`` contains the JSON representing the contents of the branch.

Atomic Types
++++++++++++

Atomic types are represented with the following JSON structure::

  {
    "type": <TYPE>,
    "value": <VALUE>
  }

Where:

  - ``TYPE``: The original atomic data type name.
  - ``VALUE``: The atomic value promoted to the appropriate JSON type.

The atomic types, their string identifiers and the JSON type the value is promoted to are as follows:

.. list-table::
   :widths: auto
   :header-rows: 1

   * - Data Type
     - ID String
     - JSON Type
   * - ``uint8`` / ``uint16`` / ``uint32`` / ``uint64``
     - "uint8" / "uint16" / "uint32" / "uint64"
     - Number
   * - ``int8`` / ``int16`` / ``int32`` / ``int64``
     - "int8" / "int16" / "int32" / "int64"
     - Number
   * - ``float32`` / ``float64``
     - "float32" / "float64"
     - Number
   * - ``bool``
     - "bool"
     - Number
   * - ``string``
     - "string"
     - String

Arrays
++++++

Except for lists of strings, arrays of data are not mapped directly to equivalent JSON types. Numerical array data is computationally inefficient to process if represented as lists of strings. To avoid this cost on the client and server, the raw data arrays are encoded using BASE64 into a single JSON string. BASE64 encoding/decoding is extremely fast compared to decoding number strings.

Arrays consisting of atomic types excluding strings are represented as follows::

  {
    "type": "array",
    "value":
    {
      "type": <TYPE>,
      "shape": [<SHAPE>],
      "encoding": "base64",
      "data": <BASE64_DATA>
    }
  }

Where:

  - ``TYPE``: The type ID for the data type.
  - ``SHAPE``: A list defining the shape of the data array.
  - ``BASE64_DATA``: BASE64 encoded array data.

The data consists of a contiguous array of bytes with c-ordering for multi-dimensional arrays. The byte order is little endian. This is encoded with BASE64 to produce a JSON compatible string representation.

Arrays of strings are handled differently as the elements of the arrays are already in string form and therefore do not require conversion::

  {
    "type": "array",
    "value":
    {
      "type": "string",
      "shape": [<SHAPE>],
      "encoding": "list",
      "data": [<STRING_DATA>]
    }
  }

Where:

  - ``SHAPE``: A list defining the shape of the data array.
  - ``STRING_DATA``: Nested lists of strings.

Multidimensional string arrays consist of nested lists with the same index ordering as a c-style multi-dimensional array.

Optional Attributes
+++++++++++++++++++

Where an attribute is specified as optional in the data class definition that attribute may take a null value i.e. it contains no data. This is represented in JSON using by a ``null`` JSON type::

  <ATTRIBUTE_NAME>: null

Where:

  - ``ATTRIBUTE_NAME``: Is the name of the optional attribute.

Examples
++++++++

The following data object structure will be serialised in the example (see :ref:`data-model` for information on data class specifications)::

  object:
    {
    _class: string = "example_class"
    _group: string = "example_group"
    _type: string = "object"
    _version: uint64 = 1
    description: string = "An example data object."
    float-data: array<float32> = [[2.5, 7.9, 2.1], [653.1, 600.6, 11304.0]]
    string-data: array<$TYPE> = [["apple", "pear"], ["cider", "pie"]]
    atomic:
    {
       int: int64: -7783627899
       float: float64 = 1.5767e-27
    }
  }

The encoded JSON form of this data would be::

  {
    "_class":
    {
      "type": "string"
      "value": "example_class"
    },
    "_group":
    {
      "type": "string"
      "value": "example_group"
    },
    "_type":
    {
      "type": "string"
      "value": "object"
    },
    "_version":
    {
      "type": "uint64"
      "value": 1
    },
    "description":
    {
      "type": "string"
      "value": "An example data object."
    },
    "float-data":
    {
      "type": "array",
      "value":
      {
        "type": "float32",
        "shape": [2, 3],
        "encoding": "base64",
        "data": "AAAgQM3M/EBmZgZAZkYjRGYmFkQAoDBG"
      }
    },
    "string-data":
    {
      "type": "array",
      "value":
      {
        "type": "string",
        "shape": [2, 2],
        "encoding": "list",
        "data": [["apple", "pear"], ["cider", "pie"]]
      }
    },
    "atomic":
    {
      "type": "branch"
      "value":
      {
        "int":
        {
          "type": "int64"
          "value": -7783627899
        },
        "float":
        {
          "type": "float64"
          "value": 1.5767e-27
        },
      }
    },
  }


List Node Contents
~~~~~~~~~~~~~~~~~~

This operation lists the contents of the specified data-tree node.

Request
+++++++

The request should take the following form::

  GET /data/<PATH>?revision=<REVISION>&auth=<TOKEN> HTTP/1.1
  Host: <SERVER>
  Authorization: Bearer <TOKEN>

Query Arguments:

  - ``revision``: An integer revision number. Specifying 0 or ``head`` will return the head revision. (optional)
  - ``auth``: The user's authentication token. (optional)

Headers:

  - ``Authorization``: See :ref:`rest-api-authentication`. (optional)

Here ``PATH`` is the path to the required node without the revision element. Revisions are specified via an optional argument in the query string. If the revision argument is not present the head revision of the node is returned by default.

For example, a request for revision 5 of the node located at ``/group/item`` with authentication by header::

  GET /data/group/item?revision=5 HTTP/1.1
  Host: sal.domain.local
  Authorization: Bearer eyJhbGciOiJIUzI1NiIXVCJ9...TJVA95OrM7E20RMHrHDcEfxjoYZgeFONFh7HgQ

If the authentication request is successful a response will be generated. The contents of the response will depend on the type of node being pointed to by the request path.

Success Response (Branch Node)
++++++++++++++++++++++++++++++

Branch nodes will return a response as follows::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "report",
    "type": "branch",
    "object":
    {
      "description": <DESCRIPTION>,
      "children":
      {
        "branches": [<BRANCHES>],
        "leaves": [<LEAVES>]
      },
      "timestamp": <TIMESTAMP>,
      "revision":
      {
        "latest": <LATEST_REVISION>,
        "current": <CURRENT_REVISION>,
        "modified": [<ALL_REVISIONS>]
      }
    },
    "request":
    {
      "url": <REQUEST_URL>
    }
  }

Where:

  - ``DESCRIPTION``: A text string describing the node contents.
  - ``BRANCHES``: A list of child branch node names.
  - ``LEAVES``: A list of child leaf nodes. Each leaf is described by a structure containing the leaf name and data class. See below.
  - ``TIMESTAMP``: The date and time of node creation or update in ISO 8601 format.
  - ``LATEST_REVISION``: The highest revision number available for the node.
  - ``CURRENT_REVISION``: The revision number returned in the report.
  - ``ALL_REVISIONS``: A list of all the available revisions of the node.
  - ``REQUEST_URL``: The url passed as part of the request.

The leaves in the leaf node list are described with the following structure::

  {
    name: <NAME>
    class: <CLASS>,
    group: <GROUP>,
    version: <VERSION>
  }

Where ``NAME`` is the node name. ``CLASS``, ``GROUP`` and ``VERSION`` are data class identifiers corresponding to the data class stored under the leaf node.

The following is an example of a populated branch node report::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "report",
    "type": "branch",
    "object":
    {
      "description": "Data from main ADC.",
      "children":
      {
        "branches": ["config", "calibration"],
        "leaves":
        [
          {
            name: "voltage",
            class: "signal",
            group: "signal",
            version: 1
          },
          {
            name: "current",
            class: "signal",
            group: "signal",
            version: 1
          },
        ]
      },
      "timestamp": "2017-11-03T11:49:55.629648",
      "revision":
      {
        "latest": 9,
        "current": 9,
        "modified": [1, 9]
      }
    },
    "request":
    {
      "url": "http://sal.domain.local/data/pulse/4000/adc/main"
    }
  }


Success Response (Leaf Node)
++++++++++++++++++++++++++++

A leaf node responses take the form::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "report",
    "type": "leaf",
    "object":
    {
      "description": <DESCRIPTION>,
      "object":
      {
        "class": <CLASS>,
        "group": <GROUP>,
        "version": <VERSION>
      },
      "timestamp": <TIMESTAMP>,
      "revision":
      {
        "latest": <LATEST_REVISION>,
        "current": <CURRENT_REVISION>,
        "modified": [<ALL_REVISIONS>]
      }
    },
    "request":
    {
      "url": <REQUEST_URL>
    }
  }

Where:

  - ``DESCRIPTION``: A text string describing the node contents.
  - ``CLASS``, ``GROUP``, ``VERSION``: Data class identifiers corresponding to the data class stored under the leaf node.
  - ``TIMESTAMP``: The date and time of node creation or update in ISO 8601 format.
  - ``LATEST_REVISION``: The highest revision number available for the node.
  - ``CURRENT_REVISION``: The revision number returned in the report.
  - ``ALL_REVISIONS``: A list of all the available revisions of the node.
  - ``REQUEST_URL``: The url passed as part of the request.

The following is an example of a populated leaf node report::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "report",
    "type": "leaf",
    "object":
    {
      "description": "Main system current.",
      "object":
      {
        "class": "signal",
        "group": "signal",
        "version": 1
      },
      "timestamp": "2014-09-18T15:46:39.000000",
      "revision":
      {
        "latest": 9,
        "current": 9,
        "modified": [1, 9]
      }
    },
    "request":
    {
      "url": "http://sal.domain.local/data/pulse/4000/adc/main/current"
    }
  }


Get Node Objects
~~~~~~~~~~~~~~~~

This operation returns the contents of the specified data-tree node. This may be either a data object or branch object.

Request
+++++++

The request should take the following form::

  GET /data/<PATH>?revision=<REVISION>&object=<TYPE>&auth=<TOKEN> HTTP/1.1
  Host: <SERVER>
  Authorization: Bearer <TOKEN>

Query Arguments:

  - ``object``: The representation of object to obtain. Takes either ``full`` or ``summary``. (required)
  - ``revision``: An integer revision number. Specifying 0 or ``head`` will return the head revision. (optional)
  - ``auth``: The user's authentication token. (optional)

Headers:

  - ``Authorization``: See :ref:`rest-api-authentication`. (optional)

Here ``PATH`` is the path to the required node without the revision element. Revisions are specified via an optional argument in the query string. If the revision argument is not present the head revision of the node is returned by default.

If the authentication request is successful a response will be generated. The contents of the response will depend on the type of node being pointed to by the request path.

Success Response (Branch Node)
++++++++++++++++++++++++++++++

Branch nodes will return a Branch object encoded in the response::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "object",
    "type": "branch",
    "object":
    {
      "description": <DESCRIPTION>
    },
    "request":
    {
      "url": <REQUEST_URL>
    }
  }

Where:

  - ``DESCRIPTION``: A text string describing the node contents.
  - ``REQUEST_URL``: The url passed as part of the request.

The following is an example of a populated branch node object response::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "object",
    "type": "branch",
    "object":
    {
      "description": "Data from main ADC."
    },
    "request":
    {
      "url": "http://sal.domain.local/data/pulse/4000/adc/main"
    }
  }

Success Response (Leaf Node)
++++++++++++++++++++++++++++

Leaf nodes will return a data object encoded in the response. This data object may be the full object or summary object according to the requested representation::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "object",
    "type": "leaf",
    "object":
    {
      <ENCODED_OBJECT>
    },
    "request":
    {
      "url": <REQUEST_URL>
    }
  }

Where:

  - ``ENCODED_OBJECT``: The data object encoded using the type-encoded JSON scheme described in :ref:`rest-api-encoding`.
  - ``REQUEST_URL``: The url passed as part of the request.

The following is an example of a populated leaf node object response::

  Status Code: 200 OK
  Content-Type: application/json

  {
    "content": "object",
    "type": "leaf",
    "object":
    {
      "_type":
      {
        "type": "string",
        "value": "full"
      },
      "_class":
      {
        "type": "string",
        "value": "scalar"
      },
      "_group":
      {
        "type": "string",
        "value": "core"
      },
      "_version":
      {
        "type": "uint64",
        "value": 1
      },
      "description":
      {
        "type": "string",
        "value": "Amplifier gain."
      },
      "value":
      {
        "type": "uint64",
        "value": 2.356
      }
    },
    "request":
    {
      "url": "http://sal.domain.local/data/pulse/4000/amplifier/main/gain?object=full"
    }
  }

This example is leaf node containing a full Scalar object. The object is type encoded as described in :ref:`rest-api-encoding`.

Put Branch Node
~~~~~~~~~~~~~~~

Creates or updates a branch node.

Overwriting an existing branch node will result in the description of the branch node being replaced with the description supplied in the request. The children of the node will be unaffected.

If creating a new node, the parent node in the data tree must be a branch node otherwise the request will fail.

Request
+++++++

The request should take the following form::

  POST /data/<PATH>?auth=<TOKEN> HTTP/1.1
  Host: <SERVER>
  Authorization: Bearer <TOKEN>

  {
    "content": "object",
    "type": "branch",
    "object":
    {
      "description": <DESCRIPTION>
    }
  }

Query Arguments:

  - ``auth``: The user's authentication token. (optional)

Headers:

  - ``Authorization``: See :ref:`rest-api-authentication`. (optional)

Here:

  - ``PATH``: The path to the node on which the new branch should be created.
  - ``DESCRIPTION``: A text string describing the branch contents.

Success Response
++++++++++++++++

If the write is successful a response will be returned contain the following::

   Status Code: 204 No Content


Put Leaf Node
~~~~~~~~~~~~~

Creates or updates a leaf node node.

Overwriting an existing leaf node will result in the data object stored on the leaf node being replaced unless the system being operated supports revisions (see :ref:`data-model`).

If creating a new node, the parent node in the data tree must be a branch node otherwise the request will fail.

Request
+++++++

The request should take the following form::

  POST /data/<PATH>?auth=<TOKEN> HTTP/1.1
  Host: <SERVER>
  Authorization: Bearer <TOKEN>

  {
    "content": "object",
    "type": "leaf",
    "object":
    {
      <ENCODED DATA OBJECT>
    }
  }


Query Arguments:

  - ``auth``: The user's authentication token. (optional)

Headers:

  - ``Authorization``: See :ref:`rest-api-authentication`. (optional)

Here ``PATH`` is the path to the node on which the new leaf should be created.

[parent node must be a branch node]

Success Response
++++++++++++++++

If the write is successful a response will be returned contain the following::

   Status Code: 204 No Content


Copy Existing Node
~~~~~~~~~~~~~~~~~~

Duplicates the specified node at a new location.

The copy operation performs a recursive copy of the entire sub-tree under a branch node, replacing any descendants of the target node if they exist.

If the copy would create a new node, the parent node in the data tree must be a branch node otherwise the request will fail.

Request
+++++++

The request should take the following form::

  POST /data/<TARGET>?source=<SOURCE>&source_revision=<REVISION>&auth=<TOKEN> HTTP/1.1
  Host: <SERVER>
  Authorization: Bearer <TOKEN>

Query Arguments:

  - ``source``: The path to the source node. (required)
  - ``source_revision``: An integer specifying the revision of source node to copy. (optional)
  - ``auth``: The user's authentication token. (optional)

Headers:

  - ``Authorization``: See :ref:`rest-api-authentication`. (optional)

Here ``SOURCE`` is the path to the source node without the revision element. Revisions are specified via the optional argument ``source_revision`` in the query string. If the revision argument is not present the head revision of the node is returned by default.

Here ``TARGET`` is the path on to which the source node should be copied.

Success Response
++++++++++++++++

If the write is successful a response will be returned contain the following::

   Status Code: 204 No Content


Delete Node
~~~~~~~~~~~

Deletes the specified node.

Deletion of a branch node will result in all of its descendants being recursively deleted.

Request
+++++++

The request should take the following form::

  DELETE /data/<PATH>?auth=<TOKEN> HTTP/1.1
  Host: <SERVER>
  Authorization: Bearer <TOKEN>

Query Arguments:

  - ``auth``: The user's authentication token. (optional)

Headers:

  - ``Authorization``: See :ref:`rest-api-authentication`. (optional)

Here ``PATH`` is the path of the node to be deleted.

Success Response
++++++++++++++++

If the write is successful a response will be returned contain the following::

   Status Code: 204 No Content


Permission Tree Operations
--------------------------

TBD


