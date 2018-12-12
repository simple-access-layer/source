Development
===========

.. note:: Please contact the Software Engineering Group for more information, this section will be expand as development continues.

Some of the content in this section are just notes, these will be expanded in the future or on request.

Principles
----------

Keep it simple!

Minimise code closer to the data storage - reduce risk of mistakes that could affect all the data. Heavy lifting of data e.g. chunking is handled client side. Also SAL is designed to be managed and developed by a resource limited (skill and/or time) team e.g. a common problem in research organisations.

Validation before write - data is validated

Total abstraction of user and storage - keeps user code stable when backend changes, admins have freedom to evolve backend without affecting users.

Simple to learn - minimal core api, easy to remember, intuitive and symmetrical behaviour - data out is identical to data out.

Architecture
------------

[PICTURE]

Designed with object stores in mind, data is opaque to storage system and ideally persistence layer

data classes and validation sit in layer above

rest api on top

(server memory is a limit on object size - server does not currently stream validate, instances object on server before passing to persistence layer for serialisation to storage)

intend large data to be chunked client side where data relationships are understood. Rich clients handle complex inter object behaviours - to minimise code in server core.

layered

inverted tree of complexity

Data Requests
~~~~~~~~~~~~~

The main code path in the server relates to handling data-tree requests. When a client makes a data-tree request the following actions occur on the server:

 1) if authentication is required, the clients authentication token is inspected:

    - if the token is invalid the request is rejected and the session ends.
    - if valid, the users permission groups are requested from the authentication provider.

 2) if the request is to write data:

    - the object is de-serialised and validated
    - if the object does not conform to specification the request is rejected and the session ends.

 3) the request is passed to the persistence provider.

    - the persistence provider checks read/write permissions for the client's group memberships
    - the request is rejected if none of the user groups have permission to perform the operation
    - data is store/retrieved according to the requested operation

 4) the persistence provider response is serialised and returned to the client.

Server API
----------

The persistence and authentication providers are plug-able components of the server. Each provider is a Python class derived from a common base class that defines the relevant interface. To implement a new provider the developer must sub-class the relevant provider base class and provide implementations of the required sections of the interface.

The provider base classes are described below.

Persistence Provider
~~~~~~~~~~~~~~~~~~~~

.. autoclass:: sal.server.interface.PersistenceProvider
   :members:

Authentication Provider
~~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: sal.server.interface.AuthenticationProvider
   :members:

Must only raise SAL exceptions


Adding Data Classes
-------------------

TBD

Consider a new data class carefully!

Validation is performed by object instantiation - make sure your object __init__ properly validates the data before it is passed to the persistence layer.


Future Development
------------------

The next sections describe future directions along which the SAL server is intended to expand.

Large Signal Handling
~~~~~~~~~~~~~~~~~~~~~

The SAL server must have a maximum object size set, to prevent server memory being exhausted by a single requests (note streaming validation may remove this concern). Equally client memory is limited so huge objects can not be set to an arbitrary client. The SAL data system, by design, does not understand the contents of the data classes. It therefore does not know how to intelligently provide ranges of e.g. signal data or frames of videos, it simply returns objects. The reason for this is server simplicity.

Large objects are defined as objects that may exceed a typical clients memory when a small number are read (at present large objects are ~512MB, so max ~4-5 objects on a windows 4GB machine).

The plan for large signal handling is to perform client side chunking of the data. For example a LargeSignal class would hold references to multiple Signal objects that hold chunks of the full LargeSignal data range. E.g. a 2D signal of 10Mx10M elements could be chunked as follows::

  path_to_signal/signalname -> LargeSignal object
  path_to_signal/signalname_chunks/chunk_1_1 -> Signal object holding x range = [0:5M), y range = [0:5M)
  path_to_signal/signalname_chunks/chunk_2_1 -> Signal object holding x range = [5M:10M), y range = [0:5M)
  path_to_signal/signalname_chunks/chunk_1_2 -> Signal object holding x range = [0:5M), y range = [5M:10M)
  path_to_signal/signalname_chunks/chunk_2_2 -> Signal object holding x range = [5M:10M), y range = [5M:10M)

In essence, the LargeSignal is a descriptor that holds the meta data and units etc... like a Signal, but has a table of chunk descriptions instead of data. The chunks are stored in a branch with the same name as the LargeSignal with e.g. _chunks appended. By relying on this convention, rather than baking the chunk path into the LargeSignal, we avoid needing to rewrite the object contents if the data is moved. I.E. we avoid tight coupling the tree organisation and the internals of the LargeSignal object.

Horizontal Scaling
~~~~~~~~~~~~~~~~~~

Provided a persistence layer uses a locking scheme and revision meta data that is accessible from the backing store, it should be possible to run multiple SAL server instances with the same persistence layer on different hardware nodes. The critical thing is the locking, each server will need to know when it can update the shared resource. This could either be locks stored on the backing store directly (probably inefficient) or a common redis/database instance shared between the SAL head nodes. The performance of the locking server will likely be the limiting factor on the horizontal scaling wrt to writing. Reading (if the data access is carefully designed to be lockless) should be fully scalable.


Long Operations
~~~~~~~~~~~~~~~

In the future there is a risk that long operations (e.g. large delete of copy operations) may be interrupted by one of more timeouts between the client and server. This may result in the client being unable to track the result of an operation or the server.

Simply increasing the timeout of various components is not a scalable solution and invariable a timeout will be encountered that may not be in the developer/administrators control.

The following solution is considered for the future - add a task manager between the REST API and the persistence layer. When a user makes a request, the request triggers the creation of a task that is placed on the task queue. The current state of the queue is visible via a REST api endpoint. Following a request, the server waits for a fixed amount of time for a requested operation to be completed by the persistence layer. If the operation is completed in this time, the server returns the a normal response, as per the current behaviour. If the operation lasts longer than the maximum wait time, the REST API returns a "202 Accepted" status code, a link to the task in the queue endpoint of the REST API and an access token generated for that task. The client can then inspect the queue and wait for the task to be completed.

Need to consider how to obtain the response of the task - a task result would need to persist for a window of time for the client to obtain. After which the response is deleted? Only the client performing the original operation should have access to the results, hence the access token mentioned above.


Statistics
~~~~~~~~~~

A separate statistics handler should be implemented - this should be plug-able - that either logs to file, memory, a messaging host etc... depending on the plugin. It will need to be a separate process with a pipe or port via which each server worker process can send data.
