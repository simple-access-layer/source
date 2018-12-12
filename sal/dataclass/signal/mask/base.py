from sal.core.object.dataclass import DataObject, DataSummary


# TODO: add tests
# TODO: add docstrings
class Mask(DataObject):

    GROUP = 'signal_mask'

    def shape_compatible(self, shape):
        raise NotImplementedError('This method must be implemented by sub-classes.')


# TODO: add tests
# TODO: add docstrings
class MaskSummary(DataSummary):

    GROUP = 'signal_mask'

    def to_dict(self):
        return self._new_dict()

    @classmethod
    def from_dict(cls, d):
        if not cls.is_compatible(d):
            raise ValueError('The dictionary does not contain a serialised error class.')

        c = cls(
            description=d['description'],
        )

        return c

