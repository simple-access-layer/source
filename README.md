Simple Access Layer (SAL)
=========================

A data access and storage middleware.

Provides a hierarchical organisational structure into which various, administrator defined data sources can be stored. Users can locate, read and write data using a simple API.

The SAL server provides a REST API to access and modify data (where permitted). Client libraries for python are provided that automatically de-serialise and convert the SAL data classes into equivalent python objects.

The backend persistence storage is easily replaceable to enable data to be easily stored using a range of different technologies - object stores, filesystems, databases.

Background
----------

This middleware was developed for the Joint European Torus to provide a unified method of access on top of a range of legacy storage technologies. The middleware was built to be a general access method where JET was one user. As such it should be generally useful to the wider community.

Installation
------------

To install SAL for normal use, please use the release version on pypi.org. This can be installed using pip:

```
pip install sal
```

Contributing
------------

Contributions to the SAL project are always welcome, but please note that we require the copyright for any contributions to be transferred to the project. 

Licensing
---------
SAL is licenced under EUPL-1.2. Please see the LICENSE.txt in the repository.

Dr Alex Meakins - 7th January 2019.
