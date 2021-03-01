"""
Tests that seralise and deserialise are symmetric operations when applied to
the objects which can represent SAL nodes  
"""

from datetime import datetime

import numpy as np
import pytest

from sal.core.serialise import serialise, deserialise
from sal.core.object import Branch, BranchReport, LeafReport
from sal.dataclass import Array, Dictionary, Scalar, String
                            

@pytest.fixture
def leaf_report():

    return LeafReport('This is a leaf report',
                      'string',
                      'core',
                      1,
                      datetime(2020, 4, 1, 13, 30, 15),
                      revision_current=3,
                      revision_latest=2)


@pytest.fixture
def branch_report():

    return BranchReport('This is a branch report',
                        ['sub_branch1', 'sub_branch2', 'sb3'],
                        [],
                        datetime(2020, 1, 15, 11, 30, 15),
                        revision_current=3,
                        revision_latest=2)

@pytest.fixture
def branch():

    return Branch('This is a branch')

@pytest.fixture
def string():

    return String('Test string', description='An example of a String')

@pytest.fixture
def scalar():

    return Scalar(1., description='An example of a Scalar')

@pytest.fixture
def array():

    shape = (10, 10)
    data = np.ones(shape=shape)
    return Array(shape=shape, data=data, dtype='float64',
                 description='An example of an Array')

@pytest.fixture
def dictionary():

    return Dictionary({'v1':1., 'v2': 2., 'v3':3.},
                      description='An example of a Dictionary')

@pytest.mark.parametrize('object_fixture',
                         ['branch_report',
                           'leaf_report',
                           'branch',
                           'string',
                           'scalar',
                           'array',
                           'dictionary'])
def test_serialise_deserialise_symmetry(object_fixture, request):

    """
    GIVEN
        A SAL object which can be returned from a SAL operation (e.g., list,
        get) on a branch or leaf node
    WHEN
        The object is serialised and then deserialised
    THEN
        The resulting object has the same number of attributes as the original
        AND the attributes of the resulting object are equal to the attributes
        of the original

    .. note::
        SAL classes do not implement __eq__, which means that equality is tested
        for based on location in memory i.e., for two objects to be equal they must
        be the same object. However serialising and deserialising an object
        results in a new object, so equality must be tested on a per-attribute
        basis. For this reason, neither a Signal or a BranchReport with leaves
        are tested, as these objects have SAL class instances as attributes
        (which also have to be tested for equality on a per-atttibute basis).
        If __eq__ is defined for the required SAL classes, it would be useful
        to add fixture for these examples.

    .. note::
        This test relies on __slots__ not being implemented for any SAL objects
    """

    obj = request.getfixturevalue(object_fixture)
    serialised = serialise(obj)
    
    assert isinstance(serialised, dict)
    deserialised = deserialise(serialised)

    # Unfortunately __eq__ is not defined for any of the test classes, so have
    # to assert attribute by attribute
    assert len(vars(obj)) == len(vars(deserialised))
    for attr in vars(obj):
        if not attr.startswith("__"):
            equality = getattr(obj, attr) == getattr(deserialised, attr) 
            if isinstance(equality, bool):
                assert equality
            else:
                assert equality.all()