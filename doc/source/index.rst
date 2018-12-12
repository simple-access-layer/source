.. image:: sal_logo.png
   :alt: The SAL Logo.
   :align: center

.. raw:: html

   <br>

Simple Access Layer (SAL)
=========================

Simple Access Layer (SAL) is a data access middleware and storage system, focused on storing scientific data for large experiments. The user interface and the SAL data model are intended to be easy to learn and extend.

SAL has the following features:

  - structured data stored in a filesystem-like hierarchical organisational structure.

    - a set of common research related data-types: signals, images, dictionaries, meshes etc...
    - support for data revisioning, storing the full history of changes to the data

  - client/server architecture

    - server exports an HTTP REST API
    - Python client

  - easy to extend

    - layered architecture

      - plug-able authentication layer
      - plug-able storage layer

    - Python based server

The core of SAL is its data model. This defines the behaviour of the data system and the organisation of the data within in. The SAL server implements this data model and exposes it to users via an HTTP REST API.

If you are reading this documentation as a user of a SAL system, please read the data model, data classes and client sections of this documentation. If a client is not available for your chosen language the REST API can be used to access data directly, see the REST API section.

Administrators and developers should see the server section in addition to the sections listed above.

Contents
--------

.. toctree::
   :maxdepth: 2

   clients
   server
   datamodel


Indices and Tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

