import numpy as np
import pytest

from sal.dataclass import Signal, SignalSummary


@pytest.mark.parametrize('class_',
                         [Signal,
                          SignalSummary])
def test_signal_empty_dimension(class_):
    
    with pytest.raises(ValueError):
        class_([])


def test_signal_incorrect_temporal_dimensions(array_dim, calc_dim):

    """
    GIVEN
        A temporal dimension
        AND a non-temporal dimension
    WHEN
        A Signal is created with the temporal dimension ordered after the
        non-temporal dimension
    THEN
        A ValueError is raised
    """

    array_dim.temporal = True
    calc_dim.temporal = False

    with pytest.raises(ValueError):
        Signal([calc_dim, array_dim])


def test_signal_incompatible_error(sym_array_error, calc_dim):

    """
    GIVEN
        An array based error
        AND a Dimension with a different shape to the error
    WHEN
        A Signal is created using the Dimension and the error
    THEN
        A ValueError is raised
    """

    sym_array_error.data = np.arange(100)
    calc_dim.data = np.arange(50)

    with pytest.raises(ValueError):
        Signal([calc_dim], error=sym_array_error)


def test_signal_incompatible_mask(array_status, calc_dim):

    """
    GIVEN
        An array based mask
        AND a Dimension with a different shape to the mask
    WHEN
        A Signal is created using the Dimension and the mask
    THEN
        A ValueError is raised
    """

    array_status.status = np.ones(100)
    calc_dim.data = np.arange(50)

    with pytest.raises(ValueError):
        Signal([calc_dim], mask=array_status)


def test_signal_incompatible_data(calc_dim):

    """
    GIVEN
        Some data
        AND a Dimension with a different shape to the data
    WHEN
        A Signal is created using the Dimension and the data
    THEN
        A ValueError is raised
    """

    data = np.arange(100)
    calc_dim.data = np.arange(50)

    with pytest.raises(ValueError):
        Signal([calc_dim], data=data)