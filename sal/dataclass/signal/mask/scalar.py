import numpy as np
from .base import Mask, MaskSummary
from .. import subobject


# TODO: add docstrings
class ScalarStatusSummary(MaskSummary):

    CLASS = 'mask_scalar_status'
    VERSION = 1


# TODO: add tests
# TODO: add docstrings
@subobject.register
class ScalarStatus(Mask):

    CLASS = 'mask_scalar_status'
    VERSION = 1
    SUMMARY_CLASS = ScalarStatusSummary

    def __init__(self, status, key, description=None):

        description = description or'Scalar status mask.'

        super().__init__(description)

        # validate status
        if 0 > status >= len(key):
            raise ValueError('Status value lies outside range of listed keys (status={}, valid range=[0, {}]).'.format(status, len(key) - 1))

        self.status = status
        self.key = np.array(key, dtype=str)

    def shape_compatible(self, shape):
        return True

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

