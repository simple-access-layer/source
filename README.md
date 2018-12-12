Simple Access Layer (SAL)
=========================

A data access and storage middleware.

Provides a heirarchical organisational structure into which various, administrator defined data sources can be stored. Users can locate, read and write data using a simple API.

The SAL server provides a REST API to access and modify data (where permitted). Client libraries for python are provided that automatically deserialise and convert the SAL data classes into equivalent python objects.

The backend persistance storage is easily replacable to enable data to be easily stored using a range of different technologies - object stores, filesystems, databases.

Background
----------

This middeware was developed for the Joint European Torus to provide a unified method of access on top of a range of legacy storage technologies. The missleware was built to be a general access method where JET was one user. As such it should be generally useful to the wider community.

Dr Alex Meakins - 12-12-2018.
