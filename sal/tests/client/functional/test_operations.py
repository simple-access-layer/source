"""
Tests the SAL operations: list, get, put, copy, delete 
"""

def test_list():

    """
    GIVEN
        A server with data nodes
    WHEN
        A client performs a list operation on a path
    THEN
        The client returns either a BranchReport or a LeafReport
    """

    # Include '/data:head', '/data', and '/data:10' examples


def test_list_with_auth():

    """
    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a list operation
    THEN
        The token is included in a request header
        AND the client returns either a BranchReport or a LeafReport
    """

def test_get():

    """
    GIVEN
        A server with data nodes
    WHEN
        A client performs a get operation
    THEN
        The client returns either a Branch or a DataObject
    """

    # Include '/data:head', '/data', and '/data:10' examples


def test_get_with_auth():

    """
    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a get operation
    THEN
        The token is included in a request header
        AND the client returns either a Branch or a DataObject
    """


def test_put_create():

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a put operation with a path that does not exist on
        the server
    THEN
        A new data node is created on the server
    """


def test_put_update():

    """
    GIVEN
        A server with data nodes which requires authentication
    WHEN
        The client performs a put operation with a path that does exist on the
        server 
    THEN
        The server data node corresponding to the path is replaced
    """


def test_put_with_auth():

    """
    GIVEN
        A server with data nodes
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a put operation with a path that does not exist on
        the server
    THEN
        The token is included in a request header
        AND a new data node is created on the server
    """


def test_copy():

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a copy operation from a source to a target
    THEN
        Data is copied from the source to the target on the server
    """

    # Include '/data:head', '/data', and '/data:10' examples for both target
    # and source


def test_copy_with_auth():

    """
    GIVEN
        A server with data nodes which requires authentication
        AND a client with a token in a file in .sal located in home directory
    WHEN
        The client performs a copy operation from a source to a target
    THEN
        Data is copied from the source to the target on the server
    """


def test_delete():

    """
    GIVEN
        A server with data nodes
    WHEN
        The client performs a delete operation
    THEN
        Data is delete from the specified path 
    """

