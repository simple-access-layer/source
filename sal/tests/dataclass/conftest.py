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
