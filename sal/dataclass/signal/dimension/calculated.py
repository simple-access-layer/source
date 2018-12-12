import numpy as np
from .base import Dimension, DimensionSummary
from .. import subobject

# TODO: add tests


class CalculatedDimensionSummary(DimensionSummary):
    """
    The calculated dimension summary class.
    """

    CLASS = 'coordinate_calc'
    VERSION = 1


@subobject.register
class CalculatedDimension(Dimension):
    """
    A dimension defined by a start point, length and step.
    """

    CLASS = 'coordinate_calc'
    VERSION = 1
    SUMMARY_CLASS = CalculatedDimensionSummary

    def __init__(self, length, start, step, units=None, error=None, temporal=False, description=None):
        """
        A calculated dimensions is defined by a length, start value and step
        value. The value of the dimension coordinate increases by the value
        step for each point along the dimension, starting at a defined start
        value.
        
        This class is ideal for representing timebases or coordinate dimensions
        where the dimension is derived from regularly sampled data e.g. an
        oscilloscope timebase.
        
        An error object may be provided to describe the uncertainty in the
        dimension values.

        If the dimension is a timebase, the temporal flag should be set to
        True.
        
        Units and description strings may be supplied.
        
        In addition to provided in the input parameters as class attributes,
        the class provides an 'end' attribute that is the calculated end value
        of the array. A 'data' attribute returns the value at each point along
        the dimensions as a numpy array.

        :param length: The length of the dimension.
        :param start: The start value.
        :param step: The increment value.
        :param units: A string describing the data units (default='au'). 
        :param error: An Error object or None (default=None).
        :param temporal: True is the dimension represent time, False otherwise (default=False).
        :param description: A string describing the signal (default='A signal.'). 
        """

        description = description or 'A calculated dimension.'

        # calculate limits and setup an array to simplify user interaction
        self.start = float(start)
        self.step = float(step)

        self.end = start + step * (length - 1)
        self.data = np.linspace(self.start, self.end, length, dtype=np.float64)

        super().__init__(length, units, error, temporal, description)

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
        v['start'] = np.float64(self.start)
        v['step'] = np.float64(self.step)
        return v

    @classmethod
    def from_dict(cls, d):

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised calculated dimension class.')

        # unpack error if present
        error = d['error']
        if error:
            error = subobject.build(error)

        c = cls(
            length=d['length'],
            start=d['start'],
            step=d['step'],
            units=d['units'],
            description=d['description'],
            error=error,
            temporal=d['temporal']
        )

        return c
