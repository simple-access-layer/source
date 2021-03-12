import numpy as np
import pytest

from sal.dataclass import CalculatedDimension


@pytest.mark.parametrize('length, start, step, expected',
                         [(5, 1, 2, np.array([1., 3., 5., 7., 9.])),
                          (3, 10, 10, np.array([10., 20., 30.])),
                          (4, 6, -2, np.array([6., 4., 2., 0.]))])
def test_calculated_dimension_data(length, start, step, expected):

    calc_dim = CalculatedDimension(length, start, step)
    np.testing.assert_equal(expected, calc_dim.data)