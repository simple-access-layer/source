"""
Tests relating to SAL path operations and validation
"""


from random import randint

import pytest
from hypothesis import example, given, strategies as st

from sal.core import path

# Initializing commonly used hypothesis strategies so that this isn't
# duplicated

# A segment can contain any lowercase alphanumeric characters, and '-', '_',
# '.'
SEGMENT_CHARACTERS = st.characters(min_codepoint=48,
                                   max_codepoint=122,
                                   blacklist_categories=['Lu', 'P', 'S'],
                                   whitelist_characters=['-', '_', '.'])

# Almost all characters not in SEGMENT_CHARACTERS
INVALID_CHARACTERS = st.characters(blacklist_categories=['Ll', 'Lu', 'Nd'],
                                   blacklist_characters=['-', '_', '.', ':'])

@st.composite
def int_postfix(draw):
    
    """
    :return: A strategy with a str of the form ':*' where * is any non-negative
    int
    :rtype: LazyStrategy

    .. note::
        ints are restricted to <= 9999 for performance
    """
    
    return ':{}'.format((draw(st.integers(0, 9999))))


@st.composite
def valid_postfix(draw):

    """
    :return: A valid postfix for any path (either '', ':head' or int_postfix)
    :rtype: LazyStrategy
    """

    return draw(st.one_of(st.just(''),  st.just(':head'), int_postfix()))


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
    index = draw(st.integers(min_value=0, max_value=len(valid_segment)))
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
        valid_abs_path += draw(valid_postfix())
    return valid_abs_path


@st.composite
def valid_rel_path(draw):

    """
    :return: A valid relative path consisting of one or more segments, a valid
    relative path prefix and a valid path postfix
    :rtype: LazyStrategy
    """

    valid_rel_prefixes = st.one_of(st.just(''), st.just('../'))

    n_segments = draw(st.integers(min_value=1, max_value=4))
    valid_segment = st.text(SEGMENT_CHARACTERS,
                            min_size=4,
                            max_size=8)
    valid_rel_path = '/'.join([draw(valid_segment) for i in range(n_segments)])
    valid_rel_path = (draw(valid_rel_prefixes)
                      + valid_rel_path
                      + draw(valid_postfix()))
    return valid_rel_path


@st.composite
def invalid_path(draw):

    # Get a valid relative path to modify in different ways
    n_segments = draw(st.integers(min_value=1, max_value=3))
    valid_segment = st.text(SEGMENT_CHARACTERS,
                            min_size=2,
                            max_size=5)
    valid_path = '/'.join([draw(valid_segment) for i in range(n_segments)])

    # A path can be invalid because it contains invalid characters
    invalid_characters = draw(st.text(INVALID_CHARACTERS,
                                      min_size=1,
                                      max_size=4))
    index = randint(0, len(valid_path))
    invalid_char_path = (valid_path[:index]
                         + invalid_characters
                         + valid_path[index:])
    
    # A path can be invalid because it has an invalid prefix
    invalid_prefix_path = '//' + valid_path
    
    # A path can be invalid because it has an invalid postfix
    # First map is a negative revision
    # Second map is a float revision
    invalid_postfix = draw(st.one_of(st.just(':'),
                                     int_postfix().map(
                                         lambda x: x.replace(':', ':-')),
                                     int_postfix().map(lambda x: x + '.5')))
    invalid_postfix_path = valid_path + invalid_postfix

    return draw(st.sampled_from([invalid_char_path]))


@given(valid_segment=st.text(SEGMENT_CHARACTERS, min_size=1, max_size=15),
       invalid_segment=invalid_segment())
def test_path_segment_validation(valid_segment, invalid_segment):

    """
    GIVEN
        A segment (i.e. component) of a path made up of valid SAL characters
        OR a segment (i.e. component) of a path containing at least one invalid
        character
    WHEN
        The segment is validated
    THEN
        The segment's validity is correctly identified
    """

    assert path.segment_valid(valid_segment)
    assert not path.segment_valid(invalid_segment)


@given(valid_path=st.one_of(valid_abs_path(), valid_rel_path()),
       invalid_path=invalid_path())
def test_valid_path_validation(valid_path, invalid_path):

    """
    GIVEN
        A path consisting of valid SAL segment characters, separated by zero or
        more '/', optionally postfixed with ':' and either 'head' or one or
        more integers, and not ending in '/'
    WHEN
        The path is validated
    THEN
        The path is determined to be valid
    """

    assert path.is_valid(valid_path)
    assert not path.is_valid(invalid_path)


@given(valid_abs_path=valid_abs_path(), valid_rel_path=valid_rel_path())
def test_path_is_absolute(valid_abs_path, valid_rel_path):

    """
    GIVEN
        An absolute path (A path starting with '/' and consisting of one or
        more valid segments)
        OR a relative path (starting with '../' or a valid segment character and
        consisting of one or more valid segments)
    WHEN
        It is checked if the path is absolute
    THEN
        Whether or not the path is absolute is determined
    """

    assert path.is_absolute(valid_abs_path)
    assert not path.is_absolute(valid_rel_path)


@given(valid_rel_path=valid_rel_path(), valid_abs_path=valid_abs_path())
@example(valid_rel_path='.', valid_abs_path='/')
@example(valid_rel_path='.:0', valid_abs_path='/a')
@example(valid_rel_path='.:head', valid_abs_path='/:head')
def test_path_is_relative(valid_rel_path, valid_abs_path):

    """
    GIVEN
        A relative path (starting with '../' or a valid segment character and
        consisting of one or more valid segments)
        OR an absolute path (A path starting with '/' and consisting of one or
        more valid segments)
    WHEN
        If it is checked if the path is relative
    THEN
        Whether or not the path is relative is returned
    """

    assert path.is_relative(valid_rel_path)
    assert not path.is_relative(valid_abs_path)


@pytest.mark.parametrize('test_path, normalised, unnormalised',
   [("/",
     ([], 0, True),
     ([], 0, True)
    ),
    ("/:head",
     ([], 0, True),
     ([], 0, True)
    ),
    ("/:0",
     ([], 0, True),
     ([], 0, True)
    ),
    ("/:123",
     ([], 123, True),
     ([], 123, True)
    ),
    ("/abc",
     (["abc"], 0, True),
     (["abc"], 0, True)
    ),
    ("/abc/../EFG/637:145",
     (["efg", "637"],  145, True),
     (["abc", "..", "efg", "637"], 145, True)
    ),
    (".",
     ([], 0, False),
     (["."], 0, False)
    ),
    (".:head",
     ([], 0, False),
     (["."], 0, False)
    ),
    (".:0",
     ([], 0, False),
     (["."], 0, False)
    ),
    (".:76",
     ([], 76, False),
     (["."], 76, False)
    ),
    ("../abc/efg",
     (["..", "abc", "efg"], 0, False),
     (["..", "abc", "efg"], 0, False)
    ),
    ("../abc/efg:1",
     (["..", "abc", "efg"], 1, False),
     (["..", "abc", "efg"], 1, False)
    ),
    ("/../../a/../b",
     (["b"], 0, True),
     (["..", "..", "a", "..", "b"], 0, True)
    )])
def test_path_decomposition(test_path, normalised, unnormalised):

    """
    GIVEN
        A valid path
    WHEN
        The path is decomposed
    THEN
        The components, revision and whether the path is absolute is returned
    """

    assert path.decompose(test_path, normalise=True) == normalised
    assert path.decompose(test_path, normalise=False) == unnormalised


@pytest.mark.parametrize('test_path, expected',
    [("/abc/def/ghi", "/abc/def/ghi"),
     ("/abc/def/ghi:0", "/abc/def/ghi"),
     ("/abc/def/ghi:head", "/abc/def/ghi"),
     ("/abc/def/ghi:99", "/abc/def/ghi:99"),
     ("/./../..", "/"),
     ("/abc/../def:0", "/def"),
     ("/a/b/../../../c/d/.././e:head", "/c/e"),
     ("./../..", "../.."),
     ("a/../../../b/c/d/..", "../../b/c"),
     ("../abc/../efg/ijk:56", "../efg/ijk:56"),
     (".", "."),
     (".:98", ".:98")])
def test_normalise_path(test_path, expected):

    """
    GIVEN
        A valid path
    WHEN
        The path is normalised
    THEN
        The redundant pathing operators are removed from the path
    """

    assert path.normalise(test_path) == expected


@pytest.mark.parametrize('base, relative, expected',
    [("/", ".", "/"),
     ("/", "a", "/a"),
     ("/:76", "a", "/a"),
     ("/:76", "a:245", "/a:245"),
     ("/", "a:245", "/a:245"),
     ("/abc/def/ghi", "../../ijk", "/abc/ijk"),
     ("/abc/def/../ghi", "../../ijk", "/ijk"),
     ("/abc/def/ghi", "../../ijk/..", "/abc"),
     ("/abc/def/ghi", "../../../..:100", "/:100"),
     ("/abc/def/ghi:98", "../../../..:head", "/")])
def test_path_to_absolute(base, relative, expected):

    """
    GIVEN
        A valid base path
        AND a valid relative path
    WHEN
        The components are joined
    THEN
        The absolute path composed of the base and relative components is
        returned
    """

    assert path.to_absolute(base, relative) == expected