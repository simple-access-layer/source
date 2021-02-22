
import pytest
from hypothesis import given, strategies as st

from sal.core import path


def valid_path(request):

    pass


def test_path_segment_validation(valid_segment):

    """
    GIVEN
        A segment of any length consisting of alphanumeric characters, '-', '_'
        and '.'
    WHEN
        The segment is validated
    THEN
        The segment is determined to be valid
    """

    assert path.segment_valid(valid_segment)


def test_valid_path_validation():

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


def test_path_to_absolute():

    """
    GIVEN

    WHEN

    THEN
    """

