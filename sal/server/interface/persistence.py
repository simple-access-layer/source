from sal.core.exception import UnsupportedOperation

"""
Persistence provider must implement data model

data model:
    hierarchical tree based organisation
    tree is revisioned, full history is stored
    organisational elements - branches
    data storage elements - leaves

Leaves hold a single data object
data objects are treated as a black box, persistence layer should only call base class methods - specifically summary() to generate/store summary objects


basic operations:
    list - used to explore tree, returns report on contents of specified node
    get - used to obtain data from branch and leaf nodes (Branch, DataObject and DataSummary objects)
    put - used to add data to the tree (Branch, DataObject)
    move - moves subtree from source to target
    delete - deletes a subtree from the system

    todo: add permission handling methods

persistence implementations must NOT perform post operations - where data is submitted to one part of the tree and under a different end point

"""


# TODO: define permission object
# TODO: add permission configuration methods
class PersistenceProvider:
    """
    Base class for persistence providers.

    A persistence provider must implement the SAL data model. Data is organised
    in a tree structure consisting of branch and leaf nodes. Branch nodes are
    organisational elements that hold references to other branch and leaf nodes
    (their "children"). Leaf nodes hold data. The organisation is much like a
    traditional filesystem.

    Each leaf node holds a single DataObject subclass instance. The system is
    agnostic to the data stored inside the data object and only interacts with
    the base classes.

    Nodes are addressed via a path string, see the path module for the
    construction of a valid path string.

    The persistence provider may implement revisioning and permission handling,
    though these are optional.

    If an internal error occurs in the persistence provider, it must raise an
    InternalError exception.
    """

    NAME = 'Persistence Provider'
    VERSION = '0.0.0'

    def list(self, path, group=None):
        """
        Returns node meta data for the specific path.

        The object returned depends on the type of node pointed to by the path.
        If the path identifies a branch node, a BranchInfo object is returned.
        For leaf nodes, a LeafInfo object is returned.

        If the persistence provider supports permissions, a group id may be
        provided. The operation will be carried out according to the
        permissions of the specified group.

        :param path: A valid node path.
        :param group: A permission group (default: guest).
        :return: A BranchInfo or LeafInfo object.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        raise UnsupportedOperation

    def get(self, path, summary=False, group=None):
        """
        Returns node data for the specific path.

        The object returned depends on the type of node pointed to by the path.
        If the path identifies a branch node, a Branch object is returned. Leaf
        nodes return the data object stored on that node.

        For leaf nodes, users can request the full data object or a reduced
        summary object (containing only the object meta data). If the summary
        argument is set to false (the default) the full data object is
        returned, if true a data summary is returned. The summary argument has
        no effect for branch nodes.

        If the persistence provider supports permissions, a group id may be
        provided. The operation will be carried out according to the
        permissions of the specified group.

        :param path: A valid node path.
        :param summary: Return a summary object (default: False).
        :param group: A permission group (default: guest).
        :return: A Branch, DataObject or DataSummary object.
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        raise UnsupportedOperation

    def put(self, path, content, group=None):
        """
        Creates/updates node data at the specific path.

        Put may be used to create or update branch and leaf nodes. Overwriting
        an existing node with new content will update the existing node. In the
        case of a branch node this will update the branch metadata without
        affecting the descendant nodes. Writing a new data object to a leaf
        node will replace the existing data object.

        To create a branch node, a Branch object must be supplied as the put
        content.

        To create a leaf node, a suitable DataObject subclass must be provided.
        A leaf node will be created to encapsulate the DataObject.

        Put will not create any missing intermediate nodes to the target path,
        these must be created manually before the put operation is performed.
        These cannot be created automatically as the branch metadata, such as
        the description, can not be automatically populated.

        If the persistence provider supports permissions, a group id may be
        provided. The operation will be carried out according to the
        permissions of the specified group.

        :param path: A valid node path.
        :param content: A Branch or DataObject instance.
        :param group: A permission group (default: guest).
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        raise UnsupportedOperation

    def delete(self, path, group=None):
        """
        Delete the node specified by the path.

        If a branch node is deleted, all its descendants are also deleted.

        If the persistence provider supports permissions, a group id may be
        provided. The operation will be carried out according to the
        permissions of the specified group.

        :param path: A valid node path.
        :param group: A permission group (default: guest).
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        raise UnsupportedOperation

    def copy(self, target, source, group=None):
        """
        Copy the node specified by the source path to the target path location.

        Copy will not create any missing intermediate nodes to the target path,
        these must be created manually before the copy operation is performed.
        These cannot be created automatically as the branch metadata, such as
        the description, cannot be automatically populated.

        If the persistence provider supports permissions, a group id may be
        provided. The operation will be carried out according to the
        permissions of the specified group. The group must have both read and
        delete permission for the source and write permission for the target.

        :param target: A target node path.
        :param source: A source node path.
        :param group: A permission group (default: guest).
        :raises InvalidPath: If the supplied path is invalid.
        :raises NodeNotFound: If the path does not point ot a node.
        :raises PermissionDenied: If the group does not have permission to access the node.
        """

        raise UnsupportedOperation

