import numpy as np
from sal.core.object import build, DataObject, DataSummary
from sal.dataclass.signal.error import Error, ErrorSummary

"""
Adding Signal Dimensions Classes
--------------------------------

A new signal dimension is defined by extending Dimension and defining an associated
DimensionSummary class. The dimension class is then registered with the Signal class
using the register decorator found in the sal.dataclass.signal.subobject module:

    @register
    class MyNewDimension(Dimension):
        etc...

Concrete classes derived from Dimension must populate three class attributes:
 
    CLASS: A string that uniquely identifies the class to the Signal class.
    VERSION: An integer specifying the version of the dimension class.
    SUMMARY_CLASS: The summary class associated with the dimension class.  

The string attribute CLASS must be lower-case and contain only alphanumeric characters,
'_', '-' and no whitespace. e.g. CLASS = 'signal'

VERSION must be an integer greater than zero. The version of a data class should be
incremented if the storage representation of the class changes. This ensures it is simple
for developers to identify different representations of a data class stored in the data
system.

The SUMMARY_CLASS attribute must be a reference to the DimensionSummary class returned by the
Dimension when summary() is called. e.g. SUMMARY_CLASS = MyNewDimensionSummary

The from_dict() and to_dict() methods must be implemented. To simplify development of
to_dict() a helper method _new_dict() is provided. The _new_dict() method will return a
dictionary pre-populated with the common dimension class metadata keys.
"""

# TODO: add tests


class Dimension(DataObject):
    """
    Abstract base class representing a signal dimension.

    Provides a common interface for Dimension classes.
    """

    GROUP = 'signal_dimension'

    def __init__(self, length, units=None, error=None, temporal=False, description=None):
        """
        :param length: The length of the dimension.
        :param error: An ErrorSummary object or None (default=None)
        :param units: A string describing the data units (default='au'). 
        :param temporal: True if the dimension is a timebase, False otherwise.
        :param description: A string describing the signal (default='A signal.').
        """

        # validate inputs
        units = units or 'au'
        units = str(units)

        description = description or 'A coordinate dimension.'
        description = str(description)

        length = int(length)
        if length < 1:
            raise ValueError('A dimension must have a length of at least one element.')
        self.length = length

        if error:

            if not isinstance(error, Error):
                raise TypeError('Error must be an object of class Error.')

            if not error.shape_compatible((length, )):
                raise ValueError('The supplied error is not consistent with the length of the dimension data.')

        self.error = error

        self.temporal = bool(temporal)
        self.units = units
        super().__init__(description)

    def __len__(self):
        return self.length

    def _new_dict(self):

        v = super()._new_dict()

        v.update({
            'description': self.description,
            'units': self.units,
            'length': np.uint64(self.length),
            'temporal': self.temporal,
            'error': None
        })

        if self.error:
            v['error'] = self.error.to_dict()

        return v


class DimensionSummary(DataSummary):
    """
    The summary class base class for Dimension classes.  

    See the Dimension class for more information.
    """

    GROUP = 'signal_dimension'

    def __init__(self, length, units=None, error=None, temporal=False, description=None):
        """
        :param length: The length of the dimension.
        :param error: An ErrorSummary object or None (default=None)
        :param units: A string describing the data units (default='au'). 
        :param temporal: True if the dimension is a timebase, False otherwise.
        :param description: A string describing the signal (default='A signal.').
        """

        # validate inputs
        units = units or 'au'
        units = str(units)

        description = description or 'A coordinate dimension.'
        description = str(description)

        length = int(length)
        if length < 1:
            raise ValueError('A dimension must have a length of at least one element.')
        self.length = length

        if error:
            if not isinstance(error, ErrorSummary):
                raise TypeError('Error must be an object of class ErrorSummary.')
        self.error = error

        self.temporal = bool(temporal)
        self.units = units
        super().__init__(description)

    def to_dict(self):
        return self._new_dict()

    @classmethod
    def from_dict(cls, d):

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised array dimension class.')

        # unpack error if present
        error = d['error']
        if error:
            error = build(error)

        c = cls(
            length=d['length'],
            description=d['description'],
            units=d['units'],
            error=error,
            temporal=d['temporal']
        )

        return c

    def _new_dict(self):

        v = super()._new_dict()

        v.update({
            'units': self.units,
            'length': np.uint64(self.length),
            'temporal': self.temporal,
            'error': None
        })

        if self.error:
            v['error'] = self.error.to_dict()

        return v
