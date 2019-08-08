import numpy as np

from sal.core.object import DataObject, DataSummary, dataclass
from .dimension import Dimension, DimensionSummary
from .error import Error, ErrorSummary
from .mask import Mask, MaskSummary
from . import subobject

# TODO: add tests


class SignalSummary(DataSummary):
    """
    The summary object for Signal.  

    This object contains a subset of the data held by Signal. The SignalSummary
    is identical to Signal except that the data array is not present and the
    Dimension, Error and Mask objects are replaced by their equivalent summary
    objects.  
      
    See the Signal class for more information.
    """

    CLASS = 'signal'
    GROUP = 'signal'
    VERSION = 1

    def __init__(self, dimensions, error=None, mask=None, units=None, description=None):
        """
        :param dimensions: A list of DimensionSummary objects.
        :param error: An ErrorSummary object or None (default=None).
        :param mask: A MaskSummary object or None (default=None).
        :param units: A string describing the data units (default='au'). 
        :param description: A string describing the signal (default='A signal.').
        """

        # validate strings
        units = units or 'au'
        units = str(units)

        description = description or 'A signal.'
        description = str(description)

        # a signal requires at least one dimension
        dimensions = list(dimensions)
        if not dimensions:
            raise ValueError('At least one dimension must be defined.')

        # validate dimension objects
        for dimension in dimensions:
            if not isinstance(dimension, DimensionSummary):
                raise TypeError('Dimensions list contains an object that is not a DimensionSummary.')
        self.dimensions = dimensions

        # generate shape tuple
        shape = []
        for dimension in self.dimensions:
            shape.append(dimension.length)
        self.shape = tuple(shape)

        # validate error
        if error is not None:
            if not isinstance(error, ErrorSummary):
                raise TypeError('Error must be an object of class ErrorSummary.')
        self.error = error

        # validate mask
        if mask is not None:
            if not isinstance(mask, MaskSummary):
                raise TypeError('Mask must be an object of class MaskSummary.')
        self.mask = mask

        self.units = units
        super().__init__(description)

    def to_dict(self):
        """
        Returns a dictionary representation of the object.
         
        :return: A data object dictionary. 
        """

        v = self._new_dict()

        v.update({
            'units': str(self.units),
            'dimensions': {
                'count': np.uint64(len(self.dimensions))
            },
            'error': None,
            'mask': None
        })

        for index, dimension in enumerate(self.dimensions):
            v['dimensions'][str(index)] = dimension.to_dict()

        if self.error:
            v['error'] = self.error.to_dict()

        if self.mask:
            v['mask'] = self.mask.to_dict()

        return v

    @classmethod
    def from_dict(cls, d):
        """
        Instances the object from a dictionary representation.
        
        :param d: Dictionary containing a serialised object. 
        :return: An object instance.
        """

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised signal class.')

        # unpack dimensions
        dimension_count = d['dimensions']['count']
        if dimension_count <= 0:
            raise ValueError('The dictionary does not define any dimensions for the signal.')

        dimensions = []
        for index in range(dimension_count):
            dimension = subobject.build(d['dimensions'][str(index)])
            dimensions.append(dimension)

        # unpack error if present
        error = d['error']
        if error:
            error = subobject.build(error)

        # unpack mask if present
        mask = d['mask']
        if mask:
            mask = subobject.build(mask)

        # class init performs type validation
        c = cls(
            description=d['description'],
            units=d['units'],
            dimensions=dimensions,
            error=error,
            mask=mask
        )

        return c


@dataclass.register
class Signal(DataObject):
    """
    A SAL data class representing an N-dimensional signal.
     
    This data class represents an N-dimensional signal such as a time series or
    profile. A Signal object holds an N-dimensional array of data, the shape of
    which is defined by a collection of Dimension objects. The data array and
    dimensions may each be given Error objects to describe the uncertainty in
    their data. An additional Mask may be supplied to provide additional
    metadata for each point in the data array.
    
    Dimension objects describe a single axis of the data array. There are a 
    number of different types of dimension classes that may be used to describe
    an axis. Dimension objects provide a method to store data for each position
    along the axis, typically an array or function for calculating the values.
    For more information see the documentation for the Dimension classes.
    
    Similar to Dimension classes there are a range of different Error classes
    that may be used to describe the uncertainty in the Signal data and
    Dimension data. For more information see the documentation for the Error
    classes.
    
    A data Mask object may be provided for a signal. These classes allow users
    to add additional data associated with each data point in the Signal data
    array. For example a Mask object may be used to describe which data points
    contain a feature of interest or are untrusted due to a diagnostic being
    saturated.
    """

    CLASS = 'signal'
    GROUP = 'signal'
    VERSION = 1
    SUMMARY_CLASS = SignalSummary

    def __init__(self, dimensions, data=None, dtype=None, error=None, mask=None, units=None, description=None):
        """
        The minimal requirements to create a signal object is a list of
        Dimension objects. This will create an Signal object with an empty
        data array using the default data type. For example::
        
            s = Signal([
                CalculatedDimension(length=1000, start=0.0, step=0.1, units='s', temporal=True, description='Timebase.')
                CalculatedDimension(length=25, start=2.8, step=0.1, units='m', temporal=False, description='Radius.')
            ])
            
        This will create a Signal with an empty data array of shape (1000, 25).
        The rest of the signal will assume default values.
        
        The data array shape must not be changed once the Signal is created as
        this will make the data array inconsistent with the stated dimensions.
        This will lead to the object being rejected by the SAL server.
        
        Dimensions flagged as temporal (e.g. time-bases) must occur first in
        the list of dimensions. Lists that do not conform to this requirement
        will result in an exception being raised.
        
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
        
        The signal data array can be populated at initialisation using the data
        parameter. This should be an array, or sequence interpretable as a
        numpy array, of the same shape as the signal dimensions. The data will
        be converted to the data type specified for the signal.
        
        If an error and/or Mask object are required, these may be passed via
        the error and mask parameters respectively. Both objects must have
        shapes compatible with the signal data array shape.
        
        The data units for the signal may be specified via the units parameter.
        This must be a python string and should contain the shortened form of
        the relevant units system (e.g. SI units: m, mT, us). By default, the
        units string is 'au' meaning 'arbitrary units'. Symbols are supported
        via UTF-8 support in Python, for example '\u03A9' is the UTF8 code for
        the Greek capital omega symbol.
           
        A signal description may be provided. Though it is optional, it is
        highly recommended. A good description of the signal will place the
        signal in context and identify any nuances of the data that are not
        obvious by looking at the data alone. The description string must never
        be used for storing additional metadata, it is intended for human
        consumption only. The description is a python string and therefore 
        supports UTF8 characters, as per the units string.

        The following example code will create a Signal object with full
        uncertainties on the signal data and dimension data. Note that this
        example is written with full verbosity, in practice real code will be 
        more compact::
        
            s = Signal(
                dimensions=[
                    CalculatedDimension(
                        length=5,
                        start=0.0,
                        step=0.1,
                        error=ConstantError(
                            lower=1e-6,
                            upper=1e-6,
                            relative=False,
                            description='Timebase uncertainty.'
                        ),
                        units='s',
                        temporal=True,
                        description='Timebase.'
                    ),
                    ArrayDimension(
                        data=np.array([1.5, 2.5]),
                        dtype=np.float32,
                        error=ConstantError(
                            lower=0.01,
                            upper=0.01,
                            relative=False,
                            description='Radius uncertainty.'
                        ),
                        units='m',
                        temporal=False,
                        description='Radius.')
                ],
                data=np.array([[-2.0, -1.3], [0.1, 0.7], [3.5, -1.0], [-0.8, 0.4], [-5.6, -8.9]]),
                dtype=np.float32,
                error=ConstantError(
                    lower=0.1,
                    upper=0.1,
                    relative=True,
                    description='Field magnitude uncertainty.'
                ),
                mask=None,
                units='mT',
                description='Magnetic field magnitude measured at two radial positions vs time.'
            )

        :param dimensions: A list of Dimension objects.
        :param data: Initial data to populate the data array with (default=None).
        :param dtype: The data type for the data array (default=np.float64). 
        :param error: An Error object or None (default=None)
        :param mask: A Mask object or None (default=None)
        :param units: A string describing the data units (default='au'). 
        :param description: A string describing the signal (default='A signal.').
        """

        supported_types = {
            np.int8, np.int16, np.int32, np.int64,
            np.uint8, np.uint16, np.uint32, np.uint64,
            np.float32, np.float64
        }

        # validate strings
        units = units or 'au'
        units = str(units)

        description = description or 'A signal.'
        description = str(description)

        # a signal requires at least one dimension
        dimensions = list(dimensions)
        if not dimensions:
            raise ValueError('At least one dimension must be defined.')

        # validate dimension object types
        for dimension in dimensions:
            if not isinstance(dimension, Dimension):
                raise TypeError('Dimensions list contains an object that does not define a dimension.')

        # validate dimension object ordering (temporal dimensions must always be first in the list)
        temporal_forbidden = False
        for dimension in dimensions:
            if not dimension.temporal:
                temporal_forbidden = True
            elif temporal_forbidden:
                raise ValueError('Temporal dimensions must precede all other dimensions in the dimension list.')

        self.dimensions = dimensions

        # generate shape tuple
        shape = tuple(dimension.length for dimension in dimensions)

        # validate data type
        if dtype:

            # passing type through np.dtype allows user to specify type as a string or a numpy type
            dtype = np.dtype(dtype).type
            if dtype not in supported_types:
                raise TypeError('The specified data-type is not supported by Signal.')

        else:
            dtype = np.float64

        # validate data or initialise an empty data array
        if data is not None:

            data = np.array(data)

            # check dimensions
            if data.shape != shape:
                raise ValueError('The supplied data is inconsistent with the dimensions - data: {}, dimensions: {}'.format(data.shape, shape))

            # convert data if required
            if dtype != data.dtype.type or not data.flags.c_contiguous:
                data = data.astype(dtype, order='C')

            self.data = data
        else:
            self.data = np.zeros(shape, dtype=dtype, order='C')

        # validate error
        if error is not None:

            if not isinstance(error, Error):
                raise TypeError('Error must be an object of class SignalError.')

            # error must be able to describe the error for the whole data array
            if not error.shape_compatible(self.data.shape):
                raise ValueError('The supplied error is not consistent with the data dimensions.')

        self.error = error

        # validate mask
        if mask is not None:

            if not isinstance(mask, Mask):
                raise TypeError('Mask must be an object of class SignalMask.')

            # mask must be able to describe the state for the whole data array
            if not mask.shape_compatible(self.data.shape):
                raise ValueError('The supplied mask is not consistent with the data dimensions.')

        self.mask = mask

        self.units = units
        super().__init__(description)

    @property
    def shape(self):
        return self.data.shape

    def summary(self):
        """
        Returns a summary of the Signal object.

        :return: A SignalSummary object.
        """

        # obtain summary of all dimensions
        dimensions = [dimension.summary() for dimension in self.dimensions]

        # generate error summary?
        error = self.error
        if error:
            error = error.summary()

        # generate mask summary?
        mask = self.mask
        if mask:
            mask = mask.summary()

        return self.SUMMARY_CLASS(
            dimensions=dimensions,
            error=error,
            mask=mask,
            units=self.units,
            description=self.description
        )

    def to_dict(self):
        """
        Returns a dictionary representation of the object.
         
        :return: A data object dictionary. 
        """

        v = self._new_dict()

        v.update({
            'description': str(self.description),
            'units': str(self.units),
            'dimensions': {
                'count': np.uint64(len(self.dimensions))
            },
            'data': self.data,
            'error': None,
            'mask': None
        })

        for index, dimension in enumerate(self.dimensions):
            v['dimensions'][str(index)] = dimension.to_dict()

        if self.error:
            v['error'] = self.error.to_dict()

        if self.mask:
            v['mask'] = self.mask.to_dict()

        return v

    @classmethod
    def from_dict(cls, d):
        """
        Instances the object from a dictionary representation.
        
        :param d: Dictionary containing a serialised object. 
        :return: An object instance.
        """

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised signal class.')

        # unpack dimensions
        dimension_count = d['dimensions']['count']
        if dimension_count <= 0:
            raise ValueError('The dictionary does not define any dimensions for the signal.')

        dimensions = []
        for index in range(dimension_count):
            dimension = subobject.build(d['dimensions'][str(index)])
            if isinstance(dimension, Dimension):
                dimensions.append(dimension)
            else:
                raise ValueError('Dimension defined in dictionary is not a valid signal dimension class.')

        # unpack error if present
        error = d['error']
        if error:
            error = subobject.build(error)

        # unpack mask if present
        mask = d['mask']
        if mask:
            mask = subobject.build(mask)

        c = cls(
            description=d['description'],
            units=d['units'],
            dimensions=dimensions,
            data=d['data'],
            dtype=d['data'].dtype.type,
            error=error,
            mask=mask
        )

        return c
