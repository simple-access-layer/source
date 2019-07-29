import numpy as np

from sal.core.object import DataObject, DataSummary, dataclass


class ArraySummary(DataSummary):
    """
    The summary object for Array.

    See the Array class for more information.
    """

    CLASS = 'array'
    GROUP = 'core'
    VERSION = 1

    def __init__(self, shape, description=None):
        """
        """

        self.shape = tuple(shape)

        description = description or 'An array.'
        description = str(description)

        super().__init__(description)

    def to_dict(self):
        """
        Returns a dictionary representation of the object.

        :return: A data object dictionary.
        """

        v = self._new_dict()
        v['shape'] = np.array(self.shape, np.uint64)
        return v

    @classmethod
    def from_dict(cls, d):
        """
        Instances the object from a dictionary representation.

        :param d: Dictionary containing a serialised object.
        :return: An object instance.
        """

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised array class.')

        return cls(
            shape=d['shape'],
            description=d['description'],
        )


@dataclass.register
class Array(DataObject):
    """
    A SAL data class representing an N-dimensional array.
    """

    CLASS = 'array'
    GROUP = 'core'
    VERSION = 1
    SUMMARY_CLASS = ArraySummary

    def __init__(self, shape, data=None, dtype=None, description=None):
        """
        The data array shape is is a tuple defining the length of each
        dimension. For example:

            Array(shape=(50,)) creates a 50 element 1D array

            Array(shape=(10, 10)) creates a 10x10 element 2D array

        The data array data type can be configured with the dtype parameter.
        The following data types are permitted:

            python:
                int, float

            numpy:
                int8, int16, int32, int64,
                uint8, uint16, uint32, uint64,
                float32, float64

        The dtype parameter is passed to the numpy array function and therefore
        supports all representations of type that numpy does, such as type
        strings ('float64') or type objects (np.float64).

        The array can be populated at initialisation using the data parameter.
        This should be an array, or sequence interpretable as a numpy array, of
        the same shape as the array dimensions. The data will be converted to
        the data type specified for the array.

        A array description may be provided. Though it is optional, it is
        highly recommended. The description string must never be used for
        storing additional metadata, it is intended for human consumption only.
        The description is a python string and therefore supports UTF8
        characters.

        :param shape: A tuple defining the dimensions of the array.
        :param data: Initial data to populate the data array with (default=None).
        :param dtype: The data type for the data array (default=np.float64).
        :param description: A string describing the array (default='An array.').
        """

        supported_types = {
            np.int8, np.int16, np.int32, np.int64,
            np.uint8, np.uint16, np.uint32, np.uint64,
            np.float32, np.float64
        }

        shape = tuple(shape)

        description = description or 'An array.'
        description = str(description)

        # validate data type
        if dtype:

            # passing type through np.dtype allows user to specify type as a string or a numpy type
            dtype = np.dtype(dtype).type
            if dtype not in supported_types:
                raise TypeError('The specified data-type is not supported by Array.')

        else:
            dtype = np.float64

        # validate data or initialise an empty data array
        if data is not None:

            data = np.array(data)

            # check dimensions
            if data.shape != shape:
                raise ValueError(
                    'The supplied data is inconsistent with the shape - data: {}, shape: {}'.format(
                        data.shape, shape))

            # convert data if required
            if dtype != data.dtype.type or not data.flags.c_contiguous:
                data = data.astype(dtype, order='C')

            self.data = data
        else:
            self.data = np.zeros(shape, dtype=dtype, order='C')

        super().__init__(description)

    @property
    def shape(self):
        return self.data.shape

    def summary(self):
        """
        Returns a summary of the Array object.

        :return: A ArraySummary object.
        """

        return self.SUMMARY_CLASS(self.shape, self.description)

    def to_dict(self):
        """
        Returns a dictionary representation of the object.

        :return: A data object dictionary.
        """

        v = self._new_dict()

        v.update({
            'description': str(self.description),
            'data': self.data,
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
            raise ValueError('The dictionary does not contain a serialised array class.')

        data = d['data']

        return cls(
            description=d['description'],
            shape=data.shape,
            data=data,
            dtype=data.dtype.type
        )
