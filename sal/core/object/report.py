import re
from ..time import validate_timestamp, encode_timestamp


class BranchReport:
    """
    Describes the contents of a branch node.

    :param description: The branch description.
    :param branches: A list of branch nodes names.
    :param leaves: A list of leaf (name, ObjectReport) tuples.
    :param timestamp: The time the node was modified in ISO 8601 format or a datetime object.
    :param revision_current: Revision number of returned data.
    :param revision_latest: Head revision number.
    :param revision_modified: List of revision numbers in which the node was modified.
    """

    def __init__(self, description, branches, leaves, timestamp, revision_current=1, revision_latest=1, revision_modified=None):

        # types and defaults
        description = str(description)
        branches = list(branches)
        leaves = list(leaves)
        timestamp = validate_timestamp(timestamp)

        revision_current = int(revision_current)
        revision_latest = int(revision_latest)

        revision_modified = revision_modified or (1, )
        tuple(revision_modified)

        # revisions numbers cannot be less than 1
        if revision_current < 1:
            raise ValueError('Current revision cannot be less than 1.')

        if revision_latest < 1:
            raise ValueError('Latest revision cannot be less than 1.')

        for revision in revision_modified:
            if revision < 1:
                raise ValueError('Modification revisions cannot be less than 1.')

        # leaves list must contain (name, ObjectReport) tuples
        for leaf in leaves:
            name, obj_report = leaf
            if not isinstance(name, str) or not isinstance(obj_report, ObjectReport):
                raise TypeError('Leaf must be a tuple containing objects of type (str, ObjectReport).')

        # ensure leaf and branch lists are sorted by name
        branches.sort(key=self._branch_sorter)
        leaves.sort(key=self._leaf_sorter)

        # make lists immutable
        branches = tuple(branches)
        leaves = tuple(leaves)

        self.description = description
        self.branches = branches
        self.leaves = leaves
        self.timestamp = timestamp
        self.revision_current = revision_current
        self.revision_latest = revision_latest
        self.revision_modified = revision_modified

    def __repr__(self):

        # assemble child list
        if not self.leaves and not self.branches:
            children = '  <none>'
        else:
            branches = ['  {}/ <branch>'.format(name) for name in self.branches]
            leaves = ['  {}  <{} ({}, v{})>'.format(name, obj.cls, obj.group, obj.version) for name, obj in self.leaves]
            children = '\n'.join(branches + leaves)

        return (
            'node: branch\n'
            'description: {}\n'
            'revision: {} (latest = {})\n'
            'children:\n'
            '{}'
            ''.format(self.description, self.revision_current, self.revision_latest, children)
        )

    def _leaf_sorter(self, s):
        """
        Sort key that sorts leaf names in a human friendly order.
        """

        return self._branch_sorter(s[0])

    def _branch_sorter(self, s):
        """
        Sort key that sorts branch names in a human friendly order.
        """

        return [self._try_int(c) for c in re.split('([0-9]+)', s)]

    def _try_int(self, s):
        """
        Converts a string to an int if possible.

        The string is left untouched if it is unable to be converted to an int.

        :param s: String.
        :return: String or integer.
        """

        try:
            return int(s)
        except:
            return s

    def to_dict(self):
        """
        Serialises the object into a dictionary.

        :return: Serialised object.
        """

        return {
            'description': self.description,
            'children': {
                'branches': self.branches,
                'leaves': [
                    {
                        'name': name,
                        'class': obj.cls,
                        'group': obj.group,
                        'version': obj.version
                    } for name, obj in self.leaves
                ]
            },
            'timestamp': encode_timestamp(self.timestamp),
            'revision': {
                'latest': self.revision_latest,
                'current': self.revision_current,
                'modified': self.revision_modified
            },
        }

    @classmethod
    def from_dict(cls, d):

        leaves = [(
            leaf['name'],
            ObjectReport(
                leaf['class'],
                leaf['group'],
                leaf['version']
            )
        ) for leaf in d['children']['leaves']]

        return cls(
            description=d['description'],
            branches=d['children']['branches'],
            leaves=leaves,
            timestamp=d['timestamp'],
            revision_latest=d['revision']['latest'],
            revision_current=d['revision']['current'],
            revision_modified=d['revision']['modified']
        )


class LeafReport:
    """
    Describes the contents of a leaf node.

    :param description: The branch description.
    :param cls: Data object class.
    :param group: Data object group.
    :param version: Data object version.
    :param timestamp: The time the node was modified in ISO 8601 format or a datetime object.    
    :param revision_current: Revision number of returned data.
    :param revision_latest: Head revision number.
    :param revision_modified: List of revision numbers in which the node was modified.
    """

    def __init__(self, description, cls, group, version, timestamp, revision_current=1, revision_latest=1, revision_modified=None):

        # types and defaults
        description = str(description)
        cls = str(cls)
        group = str(group)
        version = int(version)
        timestamp = validate_timestamp(timestamp)

        revision_current = int(revision_current)
        revision_latest = int(revision_latest)

        revision_modified = revision_modified or (1, )
        tuple(revision_modified)

        # revisions numbers cannot be less than 1
        if revision_current < 1:
            raise ValueError('Current revision cannot be less than 1.')

        if revision_latest < 1:
            raise ValueError('Latest revision cannot be less than 1.')

        for revision in revision_modified:
            if revision < 1:
                raise ValueError('Modification revisions cannot be less than 1.')

        self.description = description
        self.cls = cls
        self.group = group
        self.version = version
        self.timestamp = timestamp
        self.revision_latest = revision_latest
        self.revision_current = revision_current
        self.revision_modified = revision_modified

    def __repr__(self):

        return (
            'node: leaf\n'
            'description: {}\n'
            'revision: {} (latest = {})\n'
            'object: {} ({}, v{})'
        ).format(self.description, self.revision_current, self.revision_latest, self.cls, self.group, self.version)

    def to_dict(self):
        """
        Serialises the object into a dictionary.

        :return: Serialised object.
        """

        return {
            'description': self.description,
            'object': {
                'class': self.cls,
                'group': self.group,
                'version': self.version
            },
            'timestamp': encode_timestamp(self.timestamp),
            'revision': {
                'latest': self.revision_latest,
                'current': self.revision_current,
                'modified': self.revision_modified
            }
        }

    @classmethod
    def from_dict(cls, d):

        return cls(
            description=d['description'],
            cls=d['object']['class'],
            group=d['object']['group'],
            version=d['object']['version'],
            timestamp=d['timestamp'],
            revision_latest=d['revision']['latest'],
            revision_current=d['revision']['current'],
            revision_modified=d['revision']['modified']
        )


class ObjectReport:
    """
    Describes the type of a data object.

    :param cls: Object class.
    :param group: Object group.
    :param version: Object version.
    """

    def __init__(self, cls, group, version):

        self.cls = str(cls)
        self.group = str(group)
        self.version = int(version)
