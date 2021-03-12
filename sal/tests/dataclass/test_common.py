import numpy as np
import pytest

from sal import dataclass

@pytest.mark.parametrize('fixture, class_, group, version',
    [('scalar', 'scalar', 'core', 1),
     ('scalar_summary', 'scalar', 'core', 1),
     ('string', 'string', 'core', 1),
     ('string_summary', 'string', 'core', 1),
     ('array_status', 'mask_array_status', 'signal_mask', 1),
     ('array_status_summary', 'mask_array_status', 'signal_mask', 1),
     ('scalar_status', 'mask_scalar_status', 'signal_mask', 1),
     ('scalar_status_summary', 'mask_scalar_status', 'signal_mask', 1),
     ('asym_array_error', 'error_asymmetrical', 'signal_error', 1),
     ('asym_array_error_summary', 'error_asymmetrical', 'signal_error', 1),
     ('constant_error', 'error_constant', 'signal_error', 1),
     ('constant_error_summary', 'error_constant', 'signal_error', 1),
     ('sym_array_error', 'error_symmetrical', 'signal_error', 1),
     ('sym_array_error_summary', 'error_symmetrical', 'signal_error', 1),
     ('array_dim', 'coordinate_array', 'signal_dimension', 1),
     ('array_dim_summary', 'coordinate_array', 'signal_dimension', 1),
     ('calc_dim', 'coordinate_calc', 'signal_dimension', 1),
     ('calc_dim_summary', 'coordinate_calc', 'signal_dimension', 1)])
def test_class_attributes(fixture, class_, group, version, request):

    """
    GIVEN
        A DataClass
    WHEN
        The DataClass is initialized
    THEN
        The DataClass object has the expected class attributes
    """

    dataclass = request.getfixturevalue(fixture)
    assert dataclass.CLASS == class_
    assert dataclass.GROUP == group
    assert dataclass.VERSION == version


@pytest.mark.parametrize('dataclass_fixture, dict_fixture',
    [('scalar', 'scalar_dict'),
     ('scalar_summary', 'scalar_summary_dict'),
     ('string', 'string_dict'),
     ('string_summary', 'string_summary_dict'),
     ('array_status', 'array_status_dict'),
     ('array_status_summary', 'array_status_summary_dict'),
     ('scalar_status', 'scalar_status_dict'),
     ('scalar_status_summary', 'scalar_status_summary_dict'),
     ('asym_array_error', 'asym_array_error_dict'),
     ('asym_array_error_summary', 'asym_array_error_summary_dict'),
     ('constant_error', 'constant_error_dict'),
     ('constant_error_summary', 'constant_error_summary_dict'),
     ('sym_array_error', 'sym_array_error_dict'),
     ('sym_array_error_summary', 'sym_array_error_summary_dict'),
     ('array_dim', 'array_dim_dict'),
     ('array_dim_summary', 'array_dim_summary_dict'),
     ('calc_dim', 'calc_dim_dict'),
     ('calc_dim_summary', 'calc_dim_summary_dict')])
def test_to_dict(dataclass_fixture, dict_fixture, request):

    """
    GIVEN
        A DataClass (Summary or Object) instance
    WHEN
        The DataClass instance is converted to a dict
    THEN
        The dict correctly describes the DataClass instance
    """

    dataclass = request.getfixturevalue(dataclass_fixture)
    dict_ = request.getfixturevalue(dict_fixture)
    test_dict = dataclass.to_dict()

    for k, v in dict_.items():
        np.testing.assert_equal(test_dict[k], v)


@pytest.mark.parametrize('dataclass_fixture, dict_fixture, class_',
                         [('scalar', 
                           'scalar_dict',
                           dataclass.Scalar),
                          ('scalar_summary',
                           'scalar_summary_dict',
                           dataclass.ScalarSummary),
                          ('string',
                           'string_dict',
                           dataclass.String),
                          ('string_summary',
                           'string_summary_dict',
                           dataclass.StringSummary),
                          ('array_status',
                           'array_status_dict',
                           dataclass.ArrayStatus),
                          ('array_status_summary',
                           'array_status_summary_dict',
                           dataclass.ArrayStatusSummary),
                          ('scalar_status',
                           'scalar_status_dict',
                           dataclass.ScalarStatus),
                          ('scalar_status_summary',
                           'scalar_status_summary_dict',
                           dataclass.ScalarStatusSummary),
                          ('asym_array_error',
                           'asym_array_error_dict',
                           dataclass.AsymmetricArrayError),
                          ('asym_array_error_summary',
                           'asym_array_error_summary_dict',
                           dataclass.AsymmetricArrayErrorSummary),
                          ('constant_error',
                           'constant_error_dict',
                           dataclass.ConstantError),
                          ('constant_error_summary',
                           'constant_error_summary_dict',
                           dataclass.ConstantErrorSummary),
                          ('sym_array_error',
                           'sym_array_error_dict',
                           dataclass.SymmetricArrayError),
                          ('sym_array_error_summary',
                           'sym_array_error_summary_dict',
                           dataclass.SymmetricArrayErrorSummary),
                          ('array_dim',
                           'array_dim_dict',
                           dataclass.ArrayDimension),
                          ('array_dim_summary',
                           'array_dim_summary_dict',
                           dataclass.ArrayDimensionSummary),
                          ('calc_dim',
                           'calc_dim_dict',
                           dataclass.CalculatedDimension),
                          ('calc_dim_summary',
                           'calc_dim_summary_dict',
                           dataclass.CalculatedDimensionSummary)])
def test_from_dict(dataclass_fixture, dict_fixture, class_, request):

    """
    GIVEN
        A dict describing a DataClass (Summary or Object)
    WHEN
        The dict is converted to a DataClass instance
    THEN
        The DataClass instance has attributes corresponding to the dict
    """

    dataclass = request.getfixturevalue(dataclass_fixture)
    dict_ = request.getfixturevalue(dict_fixture)
    test_class = class_.from_dict(dict_)

    _per_attribute_comparison(test_class, dataclass)


@pytest.mark.parametrize('object_fixture, summary_fixture',
                         [('scalar', 'scalar_summary'),
                          ('string', 'string_summary'),
                          ('array_status', 'array_status_summary'),
                          ('scalar_status', 'scalar_status_summary'),
                          ('asym_array_error', 'asym_array_error_summary'),
                          ('constant_error', 'constant_error_summary'),
                          ('sym_array_error', 'sym_array_error_summary'),
                          ('array_dim', 'array_dim_summary'),
                          ('calc_dim', 'calc_dim_summary')])
def test_summary(object_fixture, summary_fixture, request):

    """
    GIVEN
        A DataObject instance
    WHEN
        A DataSummary is generated from the DataObject instance 
    THEN
        The DataSummary is the equivalent for the Dataobject instance 
    """

    object_ = request.getfixturevalue(object_fixture)
    summary = request.getfixturevalue(summary_fixture)
    test_summary = object_.summary()

    _per_attribute_comparison(test_summary, summary)


def _per_attribute_comparison(test_object, expected):

    """
    As DataClass subclasses do not define __eq__, this attribute-wise
    comparison is the best way to test for equality.

    .. note::
        This requires that no class has __slots__ defined
    """

    for attr, val in vars(expected).items():
        np.testing.assert_equal(getattr(test_object, attr), val)