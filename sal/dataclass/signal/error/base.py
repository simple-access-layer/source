from sal.core.object import DataObject, DataSummary

# TODO: add tests


class Error(DataObject):
    """
    Abstract base class representing a signal error.

    Provides a common interface for Error classes.
    """

    GROUP = 'signal_error'

    def shape_compatible(self, shape):
        """
        Checks the error data dimensions are compatible.
         
        :param shape: A tuple containing array dimensions. 
        :return: True if compatible, False otherwise.
        """
        raise NotImplementedError('This method must be implemented by sub-classes.')


class ErrorSummary(DataSummary):
    """
    The signal error summary base class.
    """

    GROUP = 'signal_error'

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
