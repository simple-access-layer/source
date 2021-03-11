import pytest

from sal import dataclass

@pytest.mark.parametrize('fixture, class_, group, version',
                         [('scalar', 'scalar', 'core', 1),
                          ('scalar_summary', 'scalar', 'core', 1),
                          ('string', 'string', 'core', 1),
                          ('string_summary', 'string', 'core', 1)])
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
                          ('string_summary', 'string_summary_dict')])
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
    assert dataclass.to_dict() == dict_


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
                           dataclass.StringSummary)])
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
                          ('string', 'string_summary')])
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
        assert getattr(test_object, attr) == val