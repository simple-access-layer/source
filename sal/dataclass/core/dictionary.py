import numpy as np
from re import match

from sal.core.object import DataObject, DataSummary, dataclass


class DictionarySummary(DataSummary):
    """
    The summary object for Dictionary.

    See the Dictionary class for more information.
    """

    CLASS = 'dictionary'
    GROUP = 'core'
    VERSION = 1

    def __init__(self, description=None):
        """
        :param description: A string describing the dictionary (default='A dictionary.').
        """

        #: A string describing the dictionary contents.
        description = description or 'A dictionary.'
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
            raise ValueError('The dictionary does not contain a serialised dictionary class.')
        return cls(d['description'])


@dataclass.register
class Dictionary(DataObject):
    """
    A SAL data class containing a collection of key-value pairs.

    A dictionary class contains a collection of data values indexed by a key.
    The key is a string (e.g. 'enabled', 'gain') that is unique in the
    dictionary. The key strings must consist of only the characters::

        [a-z] [0-9] . - _

    Associated with each key is a value. The following types are supported for
    the item values:

        Python types:
            bool, int, float, str

        Numpy:
            int8, int16, int32, int64,
            uint8, uint16, uint32, uint64,
            float32, float64, bool

    Python types are automatically promoted to the equivalent numpy types.

    This object behaves like a standard python dictionary and supports nearly
    all the basic dictionary operations.
    """

    CLASS = 'dictionary'
    GROUP = 'core'
    VERSION = 1
    SUMMARY_CLASS = DictionarySummary

    def __init__(self, items=None, description=None):
        """
        :param items: A dictionary containing item values.
        :param description: A string describing the dictionary (default='A dictionary.').
        """

        # default values
        items = items or {}
        #: A string describing the dictionary contents.
        description = description or 'A dictionary.'

        # initialise
        super().__init__(description)
        self._items = {}
        self.update(items)

    def __getitem__(self, key):
        return self._items[key]

    def __setitem__(self, key, value):
        self._validate_key(key)
        self._items[key] = self._sanitise_value(value)

    def __delitem__(self, key):
        del self._items[key]

    def __iter__(self):
        return iter(self._items)

    def __len__(self):
        return len(self._items)

    def __contains__(self, key):
        return key in self._items

    def keys(self):
        return self._items.keys()

    def values(self):
        return self._items.values()

    def items(self):
        return self._items.items()

    def clear(self):
        self._items = {}

    def get(self, key, default=None):
        return self._items.get(key, default)

    def update(self, m=None, **kwargs):
        m = dict(m or {})
        m.update(kwargs)
        for key, value in m.items():
            self[key] = value
        return None

    def _validate_key(self, key):

        if not isinstance(key, str):
            raise TypeError('Keys must be python string objects.')

        if match(r"^[a-z0-9.\-_]+$", key) is None:
            raise ValueError('Item keys must consist of only the characters [a-z], [0-9], \'.\', \'-\', and \'_\'.')

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
            np.bool,
            str
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
        Returns a summary of the Dictionary object.

        :return: A DictionarySummary object.
        """

        return self.SUMMARY_CLASS(self.description)

    def to_dict(self):
        """
        Returns a dictionary representation of the object.

        :return: A data object dictionary.
        """

        v = self._new_dict()

        v.update({
            'items': self._items.copy(),
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
            raise ValueError('The dictionary does not contain a serialised dictionary class.')

        c = cls(
            items=d['items'],
            description=d['description']
        )

        return c
