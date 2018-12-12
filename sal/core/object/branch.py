
class Branch:
    """
    An object representing a data tree branch node.

    A node description string must be provided.

    :param description: A description string.
    """

    def __init__(self, description):
        self.description = str(description)

    @property
    def description(self):
        """
        A string describing the branch contents.
        """
        return self._description

    @description.setter
    def description(self, value):
        if not isinstance(value, str):
            raise ValueError("Description must be a string.")
        if not value:
            raise ValueError("A description cannot be an empty string.")
        self._description = value

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
