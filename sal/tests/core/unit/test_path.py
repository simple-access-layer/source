
from random import randint

import pytest
from hypothesis import example, given, strategies as st

from sal.core import path


# A segment can contain any lowercase alphanumeric characters, and '-', '_',
# '.'
SEGMENT_CHARACTERS = st.characters(min_codepoint=48,
                                   max_codepoint=122,
                                   blacklist_categories=['Lu', 'P', 'S'],
                                   whitelist_characters=['-', '_', '.'])

# Almost all characters not in SEGMENT_CHARACTERS
INVALID_CHARACTERS = st.characters(blacklist_categories=['Ll', 'Nd'],
                                   blacklist_characters=['-', '_', '.'])

@st.composite
def int_postfix(draw):
    
    """
    :return: A strategy with a str of the form ':*' where * is any non-negative
    int
    :rtype: LazyStrategy

    .. note::
        ints are restricted to <= 9999 for the sake of performance
    """
    
    return ':{}'.format((draw(st.integers(0, 9999))))

# A valid postfix for any path
# This is either '', ':head' or ':*' where * is any non-negative int
VALID_POSTFIX = st.one_of(st.just(''),  st.just(':head'), int_postfix())

# A valid prefix for a relative path
VALID_REL_PREFIX = st.one_of(st.just(''), st.just('../'))


@st.composite
def invalid_segment(draw):

    """
    :return: An invalid segment of a path, comprised of some valid segment
    characters and at least 1 invalid character inserted at a random position
    :rtype: LazyStrategy
    """

    valid_segment = draw(st.text(SEGMENT_CHARACTERS,
                                 min_size=4,
                                 max_size=8))
    invalid_characters = draw(st.text(INVALID_CHARACTERS,
                                      min_size=1,
                                      max_size=4))

    # Insert invalid characters randomly into valid segment
    index = randint(0, len(valid_segment))
    return valid_segment[:index] + invalid_characters + valid_segment[index:]


@st.composite
def valid_abs_path(draw):

    """
    :return: A valid absolute path consisting of one or more segments and a
    valid path postfix
    :rtype: LazyStrategy
    """

    n_segments = draw(st.integers(min_value=1, max_value=4))
    valid_segment = st.text(SEGMENT_CHARACTERS,
                            min_size=4,
                            max_size=8)
    valid_abs_path = '/' + '/'.join([draw(valid_segment) for i
                                     in range(n_segments)])
    if randint(0, 1):
        valid_abs_path += draw(VALID_POSTFIX)
    return valid_abs_path


@st.composite
def valid_rel_path(draw):

    """
    :return: A valid relative path consisting of one or more segments, a valid
    relative path prefix and a valid path postfix
    :rtype: LazyStrategy
    """

    n_segments = draw(st.integers(min_value=1, max_value=4))
    valid_segment = st.text(SEGMENT_CHARACTERS,
                            min_size=4,
                            max_size=8)
    valid_rel_path = '/'.join([draw(valid_segment) for i in range(n_segments)])
    valid_rel_path = (draw(VALID_REL_PREFIX)
                      + valid_rel_path
                      + draw(VALID_POSTFIX))
    return valid_rel_path


@given(valid_segment=st.text(SEGMENT_CHARACTERS, min_size=1, max_size=15))
def test_path_segment_validation(valid_segment):

    """
    GIVEN
        
    WHEN
        The segment is validated
    THEN
        The segment is determined to be valid
    """

    assert path.segment_valid(valid_segment)


@given(invalid_segment=invalid_segment())
def test_path_invalid_segment_validation(invalid_segment):

    """
    GIVEN
        
    WHEN
        The segment is validated
    THEN
        The segment is determined to be valid
    """

    assert not path.segment_valid(invalid_segment)


@given(valid_path=st.one_of(valid_abs_path(), valid_rel_path()))
def test_valid_path_validation(valid_path):

    """
    GIVEN
        A path consisting of valid SAL segment characters, separated by zero or
        more '/', optionally postfixed with ':' and either 'head' or one or
        more numerics, and not ending in '/'
    WHEN
        The path is validated
    THEN
        The path is determined to be valid
    """

    assert path.is_valid(valid_path)


def test_path_decomposition():

    """
    GIVEN

    WHEN

    THEN
    """


def test_path_segment_normalisation():

    """
    GIVEN

    WHEN

    THEN
    """


@given(valid_abs_path=valid_abs_path())
def test_path_is_absolute(valid_abs_path):

    """
    GIVEN

    WHEN

    THEN
    """

    assert path.is_absolute(valid_abs_path)


@given(valid_rel_path=valid_rel_path())
@example('.')
@example('.:0')
@example('.:head')
def test_path_to_relative(valid_rel_path):

    """
    GIVEN

    WHEN

    THEN
    """

    assert path.is_relative(valid_rel_path)