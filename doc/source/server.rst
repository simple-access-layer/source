Server
======

The SAL server is a Python WSGI application that serves data to clients via an HTTP REST API. The persistence and authentication layers of the SAL server are plug-able, allowing the SAL server to support a range of storage back-ends and authentication systems. These layers are fully abstracted from the user via the public facing data model, allowing the layers of the architecture to be replaced without the user being aware of any change.

.. note:: The SAL server is under active development. In places, this documentation describes features that are not yet fully implemented in the current repository. The ultimate source of the state of the server is the git repository.

Please see the following sections for more information about the server REST API, how to deploy the server and the development guide.

.. toctree::
   :maxdepth: 2

   server/rest_api
   server/deployment
   server/development
