import numpy as np

from sal.core.object import DataObject, DataSummary, dataclass

# TODO: add tests


class ScalarSummary(DataSummary):
    """
    The summary object for Scalar.

    See the Scalar class for more information.
    """

    CLASS = 'scalar'
    GROUP = 'core'
    VERSION = 1

    def __init__(self, description=None):
        """
        :param description: A string describing the scalar (default='A scalar value.').
        """

        #: A string describing the scalar value.
        description = description or 'A scalar value.'
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
class Scalar(DataObject):
    """
    A SAL data class that contains a single value.

    Holds a single scalar value and an associated description.

    Value may be one of the following types:

        Python types:
            bool, int, float, str

        Numpy:
            int8, int16, int32, int64,
            uint8, uint16, uint32, uint64,
            float32, float64, bool

    Python types are automatically promoted to the equivalent numpy types.
    """

    CLASS = 'scalar'
    GROUP = 'core'
    VERSION = 1
    SUMMARY_CLASS = ScalarSummary

    def __init__(self, value, description=None):
        """
        :param value: The scalar value.
        :param description: A string describing the scalar (default='A scalar value.').
        """

        #: A string describing the scalar value.
        description = description or 'A scalar value.'
        super().__init__(description)

        #: The scalar value.
        self.value = value

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, value):
        self._value = self._sanitise_value(value)

    def _sanitise_value(self, value):

        promotion_table = {
            int: np.int64,
            float: np.float64,
            bool: np.bool
        }

        supported_types = [
            np.int8, np.int16, np.int32, np.int64,
            np.uint8, np.uint16, np.uint32, np.uint64,
            np.float32, np.float64,
            np.bool
        ]

        # promote python types to more specific numpy types
        if type(value) in promotion_table:
            value = promotion_table[type(value)](value)

        # reject unsupported types
        if type(value) not in supported_types:
            raise TypeError('The type of the supplied value is not supported.')

        return value

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
            raise ValueError('The dictionary does not contain a serialised scalar class.')

        c = cls(
            value=d['value'],
            description=d['description']
        )

        return c
