from .node import Node


class Branch(Node):
    """
    An object representing a data tree branch node.

    A node description string must be provided.

    :param description: A description string.
    """

    def __init__(self, description):
        super().__init__(description)

    def to_dict(self):
        """
        Serialises the object into a dictionary.

        :return: Serialised object.
        """

        return {'description': self._description}

    @classmethod
    def from_dict(cls, d):
        """
        Instances the class from a dictionary representation.

        :param d: Dictionary containing a serialised object.
        :return: An object instance.
        """

        return cls(d['description'])

    
