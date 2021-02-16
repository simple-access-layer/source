"""
Constants used during testing which are shared between modules

.. note::
    These are not included in conftest.py as apparently explicitly importing
    from there is unwise - I am not sure why this is the case but it seems
    reasonable.
"""

# b64 encoding of {username}:{password}
B64_USER_CRED = 'dXNlcm5hbWU6cGFzc3dvcmQ='
# b64 encoding of {admin}:{admin_password}
B64_ADMIN_CRED = 'YWRtaW46YWRtaW5fcGFzc3dvcmQ='