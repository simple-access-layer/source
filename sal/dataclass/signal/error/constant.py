import numpy as np
from .base import Error, ErrorSummary
from .. import subobject

# TODO: add tests


class ConstantErrorSummary(ErrorSummary):
    """
    The constant error summary class.
    """

    CLASS = 'error_constant'
    VERSION = 1


@subobject.register
class ConstantError(Error):
    """
    A signal error that is constant over the data range.
    
    The upper and lower uncertainties can be defined independently to support
    asymmetric uncertainties. Errors may be specified as absolute or relative.
    """

    CLASS = 'error_constant'
    VERSION = 1
    SUMMARY_CLASS = ConstantErrorSummary

    def __init__(self, lower, upper, relative=False, description=None):
        """
        The lower and upper error bars are defined independently and apply to
        the entire range of data to which this error class is applied. The
        errors values must be correspond to 1 standard deviation.
        
        By default the errors are absolute (relative=False). If relative is set
        to True, the lower and upper errors are treated as relative errors e.g.
        lower = 0.1 corresponds to a 10% relative error.
        
        :param lower: The lower uncertainty.
        :param upper: The upper uncertainty.
        :param relative: True if the error is relative, False if absolute (default=False).
        :param description: A text description of the error. 
        """

        description = description or 'Constant error.'

        self.lower = lower
        self.upper = upper
        self.relative = relative

        super().__init__(description)

    @property
    def lower(self):
        return self._lower

    @lower.setter
    def lower(self, value):
        if value < 0:
            raise ValueError('Lower error bound must be greater than zero.')
        self._lower = value

    @property
    def upper(self):
        return self._upper

    @upper.setter
    def upper(self, value):
        if value < 0:
            raise ValueError('Upper error bound must be greater than zero.')
        self._upper = value

    def shape_compatible(self, shape):
        return True

    def summary(self):
        return self.SUMMARY_CLASS(self.description)

    def to_dict(self):
        v = self._new_dict()
        v['lower'] = np.float64(self._lower)
        v['upper'] =np.float64(self._upper)
        v['relative'] = self.relative
        return v

    @classmethod
    def from_dict(cls, d):

        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised constant error class.')

        c = cls(
            lower=d['lower'],
            upper=d['upper'],
            relative=d['relative'],
            description=d['description']
        )

        return c
