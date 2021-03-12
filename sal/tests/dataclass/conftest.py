import numpy as np
import pytest

from sal import dataclass


@pytest.fixture
def scalar_value():

    return 1.


@pytest.fixture
def scalar_description():

    return 'A test scalar'


@pytest.fixture
def scalar_summary_dict(scalar_description):

    return {'_class': 'scalar',
            '_group': 'core',
            '_version': 1,
            'description': scalar_description,
            '_type': 'summary'}


@pytest.fixture
def scalar_dict(scalar_summary_dict, scalar_value):

    return {**scalar_summary_dict, 'value': scalar_value, '_type': 'object'}


@pytest.fixture
def scalar(scalar_value, scalar_description):

    return dataclass.Scalar(scalar_value, description=scalar_description)


@pytest.fixture
def scalar_summary(scalar_description):

    return dataclass.ScalarSummary(description=scalar_description)


@pytest.fixture
def string_value():

    return 'str'


@pytest.fixture
def string_description():

    return 'A test string'


@pytest.fixture
def string_summary_dict(string_description):

    return {'_class': 'string',
            '_group': 'core',
            '_version': 1,
            'description': string_description,
            '_type': 'summary'}


@pytest.fixture
def string_dict(string_summary_dict, string_value):

    return {**string_summary_dict, 'value': string_value, '_type': 'object'}


@pytest.fixture
def string(string_value, string_description):

    return dataclass.String(string_value, description=string_description)


@pytest.fixture
def string_summary(string_description):

    return dataclass.StringSummary(description=string_description)


@pytest.fixture
def array_status_status():

    return np.array([0, 1, 4, 2, 2, 3, 1, 2], dtype=np.uint8)


@pytest.fixture
def array_status_key():

    return np.array([0, 1, 2, 3, 4], dtype=str)

@pytest.fixture
def array_status_description():

    return 'arr_mask'


@pytest.fixture
def array_status_summary_dict(array_status_description):

    return {'_class': 'mask_array_status',
            '_group': 'signal_mask',
            '_version': 1,
            'description': array_status_description,
            '_type': 'summary'}


@pytest.fixture
def array_status_dict(array_status_summary_dict, array_status_status,
                      array_status_key):

    return {**array_status_summary_dict, '_type': 'object',
            'key': array_status_key, 'status': array_status_status}


@pytest.fixture
def array_status(array_status_status, array_status_key,
                 array_status_description):

    return dataclass.ArrayStatus(array_status_status, array_status_key,
                                 description=array_status_description)


@pytest.fixture
def array_status_summary(array_status_description):

    return dataclass.ArrayStatusSummary(description=array_status_description)


@pytest.fixture
def scalar_status_status():

    return 1


@pytest.fixture
def scalar_status_key():

    return np.array([0, 1, 2, 3, 4], dtype=str)

@pytest.fixture
def scalar_status_description():

    return 'scalar_mask'


@pytest.fixture
def scalar_status_summary_dict(scalar_status_description):

    return {'_class': 'mask_scalar_status',
            '_group': 'signal_mask',
            '_version': 1,
            'description': scalar_status_description,
            '_type': 'summary'}


@pytest.fixture
def scalar_status_dict(scalar_status_summary_dict, scalar_status_status,
                      scalar_status_key):

    return {**scalar_status_summary_dict, '_type': 'object',
            'key': scalar_status_key, 'status': scalar_status_status}


@pytest.fixture
def scalar_status(scalar_status_status, scalar_status_key,
                 scalar_status_description):

    return dataclass.ScalarStatus(scalar_status_status, scalar_status_key,
                                 description=scalar_status_description)


@pytest.fixture
def scalar_status_summary(scalar_status_description):

    return dataclass.ScalarStatusSummary(description=scalar_status_description)


@pytest.fixture
def asym_array_error_lower():

    return np.array([2] * 5)


@pytest.fixture
def asym_array_error_upper():

    return np.array([7] * 5)


@pytest.fixture
def asym_array_error_description():

    return 'asym_err'


@pytest.fixture
def asym_array_error_summary_dict(asym_array_error_description):

    return {'_class': 'error_asymmetrical',
            '_group': 'signal_error',
            '_version': 1,
            'description': asym_array_error_description,
            '_type': 'summary'}


@pytest.fixture
def asym_array_error_dict(asym_array_error_summary_dict,
                          asym_array_error_lower, asym_array_error_upper):

    return {**asym_array_error_summary_dict,
            '_type': 'object',
            'lower': asym_array_error_lower,
            'upper': asym_array_error_upper}


@pytest.fixture
def asym_array_error(asym_array_error_lower, asym_array_error_upper,
                     asym_array_error_description):
    
    return dataclass.AsymmetricArrayError(
        asym_array_error_lower,
        asym_array_error_upper,
        description=asym_array_error_description)


@pytest.fixture
def asym_array_error_summary(asym_array_error_description):

    return dataclass.AsymmetricArrayErrorSummary(
        description=asym_array_error_description)


@pytest.fixture
def constant_error_lower():

    return 1


@pytest.fixture
def constant_error_upper():

    return 7


@pytest.fixture
def constant_error_relative():

    return True


@pytest.fixture
def constant_error_description():

    return 'const_err'


@pytest.fixture
def constant_error_summary_dict(constant_error_description):

    return {'_class': 'error_constant',
            '_group': 'signal_error',
            '_version': 1,
            'description': constant_error_description,
            '_type': 'summary'}


@pytest.fixture
def constant_error_dict(constant_error_summary_dict,
                        constant_error_lower,
                        constant_error_upper,
                        constant_error_relative):

    return {**constant_error_summary_dict,
            '_type': 'object',
            'lower': constant_error_lower,
            'upper': constant_error_upper,
            'relative': constant_error_relative}


@pytest.fixture
def constant_error(constant_error_lower, constant_error_upper,
                   constant_error_relative, constant_error_description):
    
    return dataclass.ConstantError(
        constant_error_lower,
        constant_error_upper,
        relative=constant_error_relative,
        description=constant_error_description)


@pytest.fixture
def constant_error_summary(constant_error_description):

    return dataclass.ConstantErrorSummary(
        description=constant_error_description)


@pytest.fixture
def sym_array_error_data():

    return np.array([1,4,2,5,3,2], dtype=np.float64)


@pytest.fixture
def sym_array_error_description():

    return 'sym_err'


@pytest.fixture
def sym_array_error_summary_dict(sym_array_error_description):

    return {'_class': 'error_symmetrical',
            '_group': 'signal_error',
            '_version': 1,
            'description': sym_array_error_description,
            '_type': 'summary'}


@pytest.fixture
def sym_array_error_dict(sym_array_error_summary_dict, sym_array_error_data):

    return {**sym_array_error_summary_dict,
            '_type': 'object',
            'data': sym_array_error_data}


@pytest.fixture
def sym_array_error(sym_array_error_data, sym_array_error_description):
    
    return dataclass.SymmetricArrayError(
        sym_array_error_data,
        description=sym_array_error_description)


@pytest.fixture
def sym_array_error_summary(sym_array_error_description):

    return dataclass.SymmetricArrayErrorSummary(
        description=sym_array_error_description)


@pytest.fixture
def array_dim_data():

    return np.array([1,24,5,4,3,3], dtype=np.float64)


@pytest.fixture
def array_dim_units():

    return 'ns'


@pytest.fixture
def array_dim_temporal():

    return True


@pytest.fixture
def array_dim_description():

    return 'array_dim'

@pytest.fixture
def array_dim_length():

    """
    .. note::
        This is a rare example of an attribute that is passed to Summary class
        but not the object class 
    """

    return 6


@pytest.fixture
def array_dim_summary_dict(array_dim_description, array_dim_length,
                           array_dim_units, array_dim_temporal):

    return {'_class': 'coordinate_array',
            '_group': 'signal_dimension',
            '_version': 1,
            'description': array_dim_description,
            'length': array_dim_length,
            'units': array_dim_units,
            'error': None,
            'temporal': array_dim_temporal,
            '_type': 'summary'}


@pytest.fixture
def array_dim_dict(array_dim_summary_dict, array_dim_data):

    # Remove length key
    return {**{k: v for k, v in array_dim_summary_dict.items()
               if k != 'length'},
            '_type': 'object',
            'data': array_dim_data}


@pytest.fixture
def array_dim(array_dim_data, array_dim_units, array_dim_temporal,
              array_dim_description):

    return dataclass.ArrayDimension(array_dim_data,
                                    units=array_dim_units,
                                    temporal=array_dim_temporal,
                                    description=array_dim_description)


@pytest.fixture
def array_dim_summary(array_dim_description, array_dim_length, array_dim_units,
                      array_dim_temporal):

    return dataclass.ArrayDimensionSummary(array_dim_length,
                                           units=array_dim_units,
                                           temporal=array_dim_temporal,
                                           description=array_dim_description)


@pytest.fixture
def calc_dim_length():

    return 10


@pytest.fixture
def calc_dim_start():

    return 1.


@pytest.fixture
def calc_dim_step():

    return 1.


@pytest.fixture
def calc_dim_units():

    return 'ns'


@pytest.fixture
def calc_dim_temporal():

    return True


@pytest.fixture
def calc_dim_description():

    return 'calc_dim'


@pytest.fixture
def calc_dim_summary_dict(calc_dim_description, calc_dim_length,
                          calc_dim_units, calc_dim_temporal):

    return {'_class': 'coordinate_calc',
            '_group': 'signal_dimension',
            '_version': 1,
            'description': calc_dim_description,
            'length': calc_dim_length,
            'units': calc_dim_units,
            'error': None,
            'temporal': calc_dim_temporal,
            '_type': 'summary'}


@pytest.fixture
def calc_dim_dict(calc_dim_summary_dict, calc_dim_start, calc_dim_step):

    return {**calc_dim_summary_dict,
            '_type': 'object',
            'start': calc_dim_start,
            'step': calc_dim_step}


@pytest.fixture
def calc_dim(calc_dim_length, calc_dim_start, calc_dim_step, calc_dim_units,
             calc_dim_temporal, calc_dim_description):

    return dataclass.CalculatedDimension(calc_dim_length,
                                         calc_dim_start,
                                         calc_dim_step,
                                         units=calc_dim_units,
                                         temporal=calc_dim_temporal,
                                         description=calc_dim_description)


@pytest.fixture
def calc_dim_summary(calc_dim_description, calc_dim_length, calc_dim_units,
                      calc_dim_temporal):

    return dataclass.CalculatedDimensionSummary(
        calc_dim_length,
        units=calc_dim_units,
        temporal=calc_dim_temporal,
        description=calc_dim_description)