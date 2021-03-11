from .node import Node
from ..time import validate_timestamp, encode_timestamp


class NodeReport(Node):

    def __init__(self, description, timestamp, revision_current=1,
                 revision_latest=1, revision_modified=None):
        
        super().__init__(description)
        self.timestamp = validate_timestamp(timestamp)

        revision_modified = (tuple(revision_modified) if revision_modified
                             else (1, ))

        # revisions numbers cannot be less than 1
        if revision_current < 1:
            raise ValueError('Current revision cannot be less than 1.')

        if revision_latest < 1:
            raise ValueError('Latest revision cannot be less than 1.')

        for revision in revision_modified:
            if revision < 1:
                raise ValueError('Modification revisions cannot be less than'
                                 ' 1.')
        self.revision_current = int(revision_current)
        self.revision_latest = int(revision_latest)
        self.revision_modified = revision_modified

    def __repr__(self):
        
        return(
            'description: {}\n'
            'revision: {} (latest = {})\n'
            ''.format(self.description,
                      self.revision_current,
                      self.revision_latest)
        )

    def to_dict(self):
        """
        Expresses the object as a dictionary.

        :rtype: dict.
        """

        return {
            'description': self.description,
            'timestamp': encode_timestamp(self.timestamp),
            'revision': {
                'latest': self.revision_latest,
                'current': self.revision_current,
                'modified': self.revision_modified
            }
        }
