# Large Data Access

## Goals

Enable a SAL server to respond to a client `get` operation on a leaf node which contains a large (>200 Mb) `DataObject`.  This should be done without changing the SAL client API, and should minimize the memory demands on the server.

### Definitions
`get`: One of the SAL operations (`list, get, copy, put, delete`) for interacting with a node.  **This is not equivalent to an HTTP GET request.**

large `DataObject`: An instance of a subclass of a SAL `DataObject` which is > 200 Mb.

## Requirements

- Client `get` operation should be independent of node size i.e., it should not require additional parameters or headers to be passed.
- A large `DataObject` is assumed to fit within client-side memory.


## Architecture

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

The new chunked response would be:

```http
Status Code: 200 OK
Content-Type: application/json
Transfer-Encoding: chunked

{
    "content": "chunked_objects",
    "type": "leaf",
    "chunks":
    [
        <ENCODED_OBJECT1>,
        <ENCODED_OBJECT2>
    ]
    "request":
    {
        "url": <REQUEST_URL>
    }
}
```
where each `<ENCODED_OBJECT>` is a JSON representation of a complete object (i.e. equivalent to what is in the `object` field in the current request).


## Constraints

- REST API
- What is supported by `flask` and `flask_restful`
- Achievable using JSON
- `Transfer-Encoding` header only supported in HTTP 1.1


## Alternative Approaches

- Client side chunking using `Range` header
- Partial JSON objects