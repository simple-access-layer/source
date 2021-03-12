import numpy as np
import pytest

from sal.dataclass import Scalar


@pytest.mark.parametrize('value, expected_type',
                         [(1, np.int64),
                          (1., np.float64),
                          (True, np.bool),
                          (np.int8(1), np.int8),
                          (np.int16(1), np.int16),
                          (np.int32(1), np.int32),
                          (np.int64(1), np.int64),
                          (np.uint8(1), np.uint8),
                          (np.uint16(1), np.uint16),
                          (np.uint32(1), np.uint32),
                          (np.uint64(1), np.uint64),
                          (np.float32(1.), np.float32),
                          (np.float32(1.), np.float32),
                          (np.bool(1), np.bool)])
def test_scalar_santise(scalar, value, expected_type):

    """
    GIVEN
        A Scalar
    WHEN
        The Scalar has a valid value set
    THEN
        The Scalar.value has the correct dtype 
    """

    scalar.value = value
    assert isinstance(scalar.value, expected_type)


@pytest.mark.parametrize('value',
                         [np.complex64(1.),
                          '1.',
                          [1., 2., 3.],
                          np.array([1., 2., 3.]),
                          (1., 2., 3.)])
def test_scalar_santise_invalid(scalar, value):

    """
    GIVEN
        A Scalar
    WHEN
        The Scalar has a value with an unsupported dtype set
    THEN
        A TypeError is raised
    """

    with pytest.raises(TypeError):
        scalar.value = value