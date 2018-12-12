
Deployment
==========

The SAL server is a Python WSGI application which requires a WSGI compatible web-server. The specific server setup will strongly depend on the requirements of the persistence and authentication providers selected for the deployment.

The recommended WSGI host for the server application is gunicorn with an nginx reverse proxy in-front to buffer requests.

.. note:: Please contact the Software Engineering Group for more information.

Steps:

  - setup nginx + gunicorn (+ https if using authentication)

    - use processes, not threads, depending on persistence layer SAL server may not be thread-safe

  - choose a persistence provider

    - setup required dependencies

  - choose an authentication provider (optional)

    - setup required dependencies

  - create python file for server and instance server with required configuration


