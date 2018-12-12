import numpy as np

from sal.core.object import DataObject, DataSummary, dataclass

# TODO: add tests


class StringSummary(DataSummary):
    """
    The summary object for String.

    See the String class for more information.
    """

    CLASS = 'string'
    GROUP = 'core'
    VERSION = 1

    def __init__(self, description=None):
        """
        :param description: A string describing the contents of the string (default='A string.').
        """

        #: A string describing the string.
        description = description or 'A string.'
        super().__init__(description)

    def to_dict(self):
        """
        Returns a dictionary representation of the object.

        :return: A data object dictionary.
        """

        return self._new_dict()

    @classmethod
    def from_dict(cls, d):
        """
        Instances the object from a dictionary representation.

        :param d: Dictionary containing a serialised object.
        :return: An object instance.
        """

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised scalar class.')
        return cls(d['description'])


@dataclass.register
class String(DataObject):
    """
    A SAL data class that contains a string.

    Holds a single string and an associated description. The string must be a
    python str object.
    """

    CLASS = 'string'
    GROUP = 'core'
    VERSION = 1
    SUMMARY_CLASS = StringSummary

    def __init__(self, value, description=None):
        """
        :param value: The string.
        :param description: A string describing the string contents (default='A string.').
        """

        #: A string describing the string.
        description = description or 'A string.'
        super().__init__(description)

        #: The string contents.
        self.value = value

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, value):
        if not isinstance(value, str):
            raise TypeError('The value must be a python string.')
        self._value = value

    def summary(self):
        """
        Returns a summary of the Scalar object.

        :return: A ScalarSummary object.
        """

        return self.SUMMARY_CLASS(self.description)

    def to_dict(self):
        """
        Returns a dictionary representation of the object.

        :return: A data object dictionary.
        """

        v = self._new_dict()

        v.update({
            'value': self._value,
            'description': str(self.description)
        })

        return v

    @classmethod
    def from_dict(cls, d):
        """
        Instances the object from a dictionary representation.

        :param d: Dictionary containing a serialised object.
        :return: An object instance.
        """

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised string class.')

        c = cls(
            value=d['value'],
            description=d['description']
        )

        return c
