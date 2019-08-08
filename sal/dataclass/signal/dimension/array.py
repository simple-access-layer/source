import numpy as np
from .base import Dimension, DimensionSummary
from .. import subobject

# TODO: add tests


class ArrayDimensionSummary(DimensionSummary):
    """
    The array dimension summary class.
    """

    CLASS = 'coordinate_array'
    VERSION = 1


@subobject.register
class ArrayDimension(Dimension):
    """
    A dimension defined by an array of values.
    """

    CLASS = 'coordinate_array'
    VERSION = 1
    SUMMARY_CLASS = ArrayDimensionSummary

    def __init__(self, data, dtype=None, units=None, error=None, temporal=False, description=None):
        """
        An array dimension is defined by a 1 dimensional array containing the 
        axis values for each point along the dimension. The array length
        defines the length of the dimension.
        
        The datatype of the array may be overridden using the dtype parameter.
        The permitted datatypes are identical to those listed for Signal.        
        
        An error object may be provided to describe the uncertainty in the
        dimension values.

        If the dimension is a timebase, the temporal flag should be set to
        True.

        Units and description strings may be supplied.
        
        :param data: Array of axis values.
        :param dtype: The data type for the data array (default=np.float64).
        :param units: A string describing the data units (default='au'). 
        :param error: An Error object or None (default=None).
        :param temporal: True is the dimension represent time, False otherwise (default=False).
        :param description: A string describing the signal (default='A signal.').
        """

        supported_types = {
            np.int8, np.int16, np.int32, np.int64,
            np.uint8, np.uint16, np.uint32, np.uint64,
            np.float32, np.float64
        }

        description = description or 'An array dimension.'

        # validate data
        data = np.array(data)
        if data.ndim != 1:
            raise ValueError('The data array must be 1D.')

        # validate data type
        if dtype:

            # passing type through np.dtype allows user to specify type as a string or a numpy type
            dtype = np.dtype(dtype).type
            if dtype not in supported_types:
                raise TypeError('The specified data-type is not supported.')

        else:
            dtype = np.float64

        # ensure array datatype is correct
        if dtype != data.dtype.type or not data.flags.c_contiguous:
            data = data.astype(dtype, order='C')

        self.data = data
        super().__init__(len(data), units, error, temporal, description)

    def summary(self):

        # generate error summary?
        error = self.error
        if error:
            error = error.summary()

        return self.SUMMARY_CLASS(
            self.length,
            units=self.units,
            error=error,
            temporal=self.temporal,
            description=self.description
        )

    def to_dict(self):

        v = self._new_dict()
        v['data'] = self.data
        return v

    @classmethod
    def from_dict(cls, d):

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised array dimension class.')

        # unpack error if present
        error = d['error']
        if error:
            error = subobject.build(error)

        c = cls(
            data=d['data'],
            dtype=d['data'].dtype.type,
            description=d['description'],
            units=d['units'],
            error=error,
            temporal=d['temporal']
        )

        return c

