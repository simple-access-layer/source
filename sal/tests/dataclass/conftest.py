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