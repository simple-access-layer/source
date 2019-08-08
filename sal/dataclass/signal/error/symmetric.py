import numpy as np
from .base import Error, ErrorSummary
from .. import subobject

# TODO: add tests


class SymmetricArrayErrorSummary(ErrorSummary):
    """
    The symmetric array error summary class.
    """

    CLASS = 'error_symmetrical'
    VERSION = 1


# todo: ensure error array data does not go negative, it is a magnitude
@subobject.register
class SymmetricArrayError(Error):
    """
    A signal error that varies over the data range.

    The error is symmetrical with both upper and lower uncertainties defined by
    a single value at each point.
    """

    CLASS = 'error_symmetrical'
    VERSION = 1
    SUMMARY_CLASS = SymmetricArrayErrorSummary

    def __init__(self, data, dtype=None, description=None):
        """
        The dimensions of the error are defined by the shape of the data
        supplied. The shape must match the shape of the object to which
        the error is being attached, otherwise an exception will be raised.
        
        The errors values in the array must be correspond to 1 standard
        deviation and, as error magnitudes, must be only positive values.
        
        The datatype of the array may be overridden using the dtype parameter.
        The permitted datatypes are identical to those listed for Signal.
                
        :param data: Array holding the error values.
        :param dtype: The data type for the data array (default=np.float64).
        :param description: A text description of the error. 
        """

        supported_types = {
            np.int8, np.int16, np.int32, np.int64,
            np.uint8, np.uint16, np.uint32, np.uint64,
            np.float32, np.float64
        }

        description = description or 'Symmetrical error per point.'

        # explicitly convert data to a numpy array, lets us use numpy to handle lists etc...
        data = np.array(data)

        # validate data type
        if dtype:

            # passing type through np.dtype allows user to specify type as a string or a numpy type
            dtype = np.dtype(dtype).type
            if dtype not in supported_types:
                raise TypeError('The specified data-type is not supported.')
        else:
            dtype = np.float64

        # ensure array data type/order is correct
        if dtype != data.dtype.type or not data.flags.c_contiguous:
            data = data.astype(dtype, order='C')

        self.data = data

        super().__init__(description)

    def shape_compatible(self, shape):
        return shape == self.data.shape

    def summary(self):
        return self.SUMMARY_CLASS(self.description)

    def to_dict(self):
        v = self._new_dict()
        v['data'] = self.data
        return v

    @classmethod
    def from_dict(cls, d):
        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised symmetrical array error class.')

        c = cls(
            data=d['data'],
            dtype=d['data'].dtype.type,
            description=d['description']
        )

        return c
