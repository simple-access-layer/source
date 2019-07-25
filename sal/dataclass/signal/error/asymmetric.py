import numpy as np
from .base import Error, ErrorSummary
from .. import subobject

# TODO: add tests


class AsymmetricArrayErrorSummary(ErrorSummary):
    """
    The asymmetric array error summary class.
    """

    CLASS = 'error_asymmetrical'
    VERSION = 1


# TODO: add validation to upper/lower -  must be positive, errors are magnitudes
@subobject.register
class AsymmetricArrayError(Error):
    """
    A signal error that varies over the data range.

    The error is asymmetrical with independent upper and lower uncertainties
    defined by separate data arrays.
    """

    CLASS = 'error_asymmetrical'
    VERSION = 1
    SUMMARY_CLASS = AsymmetricArrayErrorSummary

    def __init__(self, lower, upper, dtype=None, description=None):
        """
        The dimensions of the error are defined by the shape of the data
        supplied. The shape must match the shape of the object to which
        the error is being attached, otherwise an exception will be raised.
         
        The lower and upper errors are magnitudes and must contain only
        positive values. The errors values in the array must be correspond to 1
        standard deviation.
        
        The datatype of the arrays may be overridden using the dtype parameter.
        The permitted datatypes are identical to those listed for Signal.
                
        :param lower: Array holding the lower error values. 
        :param upper: Array holding the upper error values.
        :param dtype: The data type for the data array (default=np.float64).
        :param description: A text description of the error. 
        """

        supported_types = {
            np.int8, np.int16, np.int32, np.int64,
            np.uint8, np.uint16, np.uint32, np.uint64,
            np.float32, np.float64
        }

        description = description or 'Asymmetrical error per point.'

        # explicitly convert data to a numpy array, lets us use numpy to handle lists etc...
        lower = np.array(lower)
        upper = np.array(upper)

        # validate error shapes
        if lower.shape != upper.shape:
            raise ValueError('Lower and upper array shapes are inconsistent (lower: {}, upper: {}).'.format(lower.shape, upper.shape))

        # validate data type
        if dtype:

            # passing type through np.dtype allows user to specify type as a string or a numpy type
            dtype = np.dtype(dtype).type
            if dtype not in supported_types:
                raise TypeError('The specified data-type is not supported.')

        else:
            dtype = np.float64

        # ensure array data type is correct
        if dtype != lower.dtype.type or not lower.flags.c_contiguous:
            lower = lower.astype(dtype, order='C')

        if dtype != upper.dtype.type or not upper.flags.c_contiguous:
            upper = upper.astype(dtype, order='C')

        self.lower = lower
        self.upper = upper

        super().__init__(description)

    def shape_compatible(self, shape):
        return shape == self.lower.shape and shape == self.upper.shape

    def summary(self):
        return self.SUMMARY_CLASS(self.description)

    def to_dict(self):
        v = self._new_dict()

        # validate error shapes
        if self.lower.shape != self.upper.shape:
            raise ValueError('Cannot serialise, lower and upper array shapes are inconsistent (lower: {}, upper: {}).'.format(self.lower.shape, self.upper.shape))

        v['lower'] = self.lower
        v['upper'] = self.upper
        return v

    @classmethod
    def from_dict(cls, d):
        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised asymmetrical array error class.')

        if d['lower'].dtype != d['upper'].dtype:
            raise ValueError('Cannot deserialise, lower and upper data types are inconsistent (lower: {}, upper: {})'.format(d['lower'].dtype), dtype=d['upper'].dtype)

        c = cls(
            lower=d['lower'],
            upper=d['upper'],
            dtype=d['lower'].dtype.type,
            description=d['description']
        )

        return c
