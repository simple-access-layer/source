import numpy as np
import pytest

from sal.dataclass import CalculatedDimension


@pytest.mark.parametrize('length, start, step, expected',
                         [(5, 1, 2, np.array([1., 3., 5., 7., 9.])),
                          (3, 10, 10, np.array([10., 20., 30.])),
                          (4, 6, -2, np.array([6., 4., 2., 0.])),
                          (1, 1, 1, np.array([1.])),
                          (1, 1, 10, np.array([1.]))])
def test_calculated_dimension_data(length, start, step, expected):

    calc_dim = CalculatedDimension(length, start, step)
    np.testing.assert_equal(expected, calc_dim.data)


def test_calculated_dimension_data_zero_length():

    """
    CalculatedDimension data must have a length >= 1, so this should raise an
    ValueError 
    """

    with pytest.raises(ValueError):
        CalculatedDimension(0, 1, 10)