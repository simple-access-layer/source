import numpy as np
import pytest

from sal.dataclass import ArrayDimension


@pytest.mark.xfail(reason='Known bug with array dimension data setting')
def test_array_dimension_multidimensional(array_dim):

    """
    GIVEN
        An ArrayDimension
    WHEN
        The data is set to be multidimensional
    THEN
        A ValueError is raised
    """

    with pytest.raises(ValueError):
        array_dim.data = np.array([[1., 2.],
                                [5., 6.]])


@pytest.mark.parametrize('dtype',
                         [np.int8, np.int16, np.int32, np.int64,
                          np.uint8, np.uint16, np.uint32, np.uint64,
                          np.float32, np.float64])
def test_array_dimension_set_dtype(array_dim_data, dtype):

    """
    GIVEN
        A string representation of a numpy dtype
    WHEN
        An ArrayDimension is created with the dtype
    THEN
        The ArrayDimension data will have that dtype 
    """

    array_dim = ArrayDimension(array_dim_data, dtype=str(dtype.__name__))
    assert array_dim.data.dtype == dtype


@pytest.mark.xfail(reason='Known bug with array dimension dtype validation')
@pytest.mark.parametrize('dtype',
                         [np.complex64,
                          str])
def test_array_dimension_unsupported_dtype(array_dim, dtype):

    """
    GIVEN
        An ArrayDimension
    WHEN
        Data with a unsupported type is set
    THEN
        A TypeError is raised
    """

    data = np.array([1.], dtype=dtype)
    with pytest.raises(TypeError):
        array_dim.data = data