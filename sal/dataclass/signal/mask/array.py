import numpy as np
from .base import Mask, MaskSummary
from .. import subobject


# TODO: add docstrings
class ArrayStatusSummary(MaskSummary):

    CLASS = 'mask_array_status'
    VERSION = 1


# TODO: add tests
# TODO: add docstrings
# TODO: consider renaming status attribute to data to be consistent with signal, and dimension classes?
@subobject.register
class ArrayStatus(Mask):

    CLASS = 'mask_array_status'
    VERSION = 1
    SUMMARY_CLASS = ArrayStatusSummary

    def __init__(self, status, key, description=None):

        description = description or 'Array status mask.'

        # explicitly convert data to a numpy array, lets us use numpy to handle lists etc...
        status = np.array(status, dtype=np.uint8, order='C')

        # validate status
        if (status >= len(key)).any():
            raise ValueError('Status array contains status values that lie outside the range of listed keys (valid range=[0, {}]).'.format(len(key) - 1))

        self.status = status
        self.key = np.array(key, dtype=str)

        super().__init__(description)

    def shape_compatible(self, shape):
        return shape == self.status.shape

    def summary(self):
        return self.SUMMARY_CLASS(self.description)

    def to_dict(self):
        v = self._new_dict()
        v['status'] = self.status
        v['key'] = self.key
        return v

    @classmethod
    def from_dict(cls, d):
        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised scalar status mask class.')

        c = cls(
            status=d['status'],
            key=d['key'],
            description=d['description']
        )

        return c

