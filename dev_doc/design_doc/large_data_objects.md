# Large Data Access

## Goals

Enable a SAL server to respond to a client `get` operation on a leaf node which contains a large (>200 Mb) `DataObject`.  This should be done without changing the SAL client API, and should minimize the memory demands on the server.

### Definitions
`get`: One of the SAL operations (`list, get, copy, put, delete`) for interacting with a node.  **This is not equivalent to an HTTP GET request.**

large `DataObject`: An instance of a subclass of a SAL `DataObject` which is > 200 Mb.

## Requirements

- Client `get` operation should be independent of node size i.e., it should not require additional parameters or headers to be passed.  The client should not need to know if they are getting data from a node with a large `DataObject`. 
- A large `DataObject` is assumed to fit within client-side memory.
- The client should return an existing `DataObject` subclass.

## Architecture

### Overview
- The suggested approach will modify the server `get` operation, the client `get` operation, and the `DataObject` and it's subclasses.

#### Core and dataclass changes
- Create a base class for node objects (`Branch, BranchReport, DataObject, LeafReport`) called `Node`.
- Add abstract `to_dict` and `from_dict` to `Node` (from the subclasses).
- Move serialisation into `Node` by adding concrete `serialise` and `deserialise` methods which use the subclass implementations of `to_dict` and `from_dict`.
- Override `serialise` and `deserialise` for `DataObject`, as this requires the data type encoding in the JSON.
- Add `to_json` and `from_json` to `Node`, using `serialise` and `deserialise`.  This can be concrete in `Node` but overriden in classes with generator data.  `to_json` can return either a byte string or generator representation of JSON, with the generator representation required for Flask to respond with a chunked transfer encoding.

- A design needs to be determined for a class/classes wtih generator data.  Possibility include:
    * A class decorator which takes a class as a parameter.  The defined class will have the same attributes and methods as the parameter class, but with specific methods (e.g. to_json1) changed. The same could be achieved with either a mixin or simple inheritance from the parameter class, although the latter would introduce duplication.  In all cases the `DataSummary` could be the same as is used for the original `DataObject` (e.g., `StringSummary` as the summary class for `StringGenerator`).
    * A function which creates generator classes from each class in the registry.  This would be fine for `DataObject` subclasses where `to_json` is the same, but might be more challenging for others (e.g., `Signal`).
    * A container class.    

#### Server changes
- Change `DataTree.get` to return a `flask.Response` object, rather than a `dict`.  A `flask.Response` object can take either byte string or generator representations of JSON for the `response` parameter: this parameter will be provided with `to_json` from a `Node` subclass.

### UML class diagrams

### Server Responses

The current success response from a leaf node is:

```http
Status Code: 200 OK
Content-Type: application/json
Content-Length: <LENGTH>

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
```

This would change to:

```http
Status Code: 200 OK
Content-Type: application/json
Content-Length: <LENGTH>

{
    "content": "object",
    "type": "leaf",
    "object":
    {
        <ENCODED_OBJECT>
    }
}
```

In addition there will be a new chunked response:

```http
Status Code: 200 OK
Content-Type: application/json
Transfer-Encoding: chunked

{
    "content": "chunked_object",
    "type": "leaf",
    "chunked_object":
    [
        <ENCODED_OBJECT1>,
        <ENCODED_OBJECT2>
    ]
}
```
where each `<ENCODED_OBJECT>` is a JSON representation of a complete object (i.e. equivalent to what is in the `object` field in the current request).


## Constraints

- REST API
- What is supported by `flask` and `flask_restful`
- Achievable using JSON
- `Transfer-Encoding` header only supported in HTTP 1.1

## Issues with suggested approach

- This will remove the ability to add content to the response within `DataTree.get`, as the serialised content returned here by `PersistenceProvider.get` will be either a byte string or generator.

## Alternative Approaches

- Client side chunking using `Range` header
- Partial JSON objects
- `PersistenceProvider.get` has additional return of generator, which is then converted to JSON within `DataTree.get` (or function called from `DataTree.get`).