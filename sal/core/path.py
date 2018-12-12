from re import match
from string import whitespace
from .exception import InvalidPath


def is_valid(path):
    """
    Tests if a path string conforms to the path specification.

    :param path: A path string.
    :return: True if the path is valid, False if not.
    """

    path = _sanitise(path)
    return _validate(path)


def segment_valid(path_segment):
    """
    Tests if a path segment conforms to the path specification.

    :param path_segment: A path segment string.
    :return: True if the path segment is valid, False if not.
    """

    return match(r"^(/?[a-z0-9.\-_]+)$", path_segment) is not None


def is_absolute(path):
    """
    Tests if the path is an absolute path.

    :param path: A path string.
    :return: True if the path is an absolute path, False otherwise
    """

    path = _sanitise_and_check(path)
    return path[0] == "/"


def is_relative(path):
    """
    Tests if the path is an relative path.

    :param path: A path string.
    :return: True if the path is an relative path, False otherwise.
    """

    path = _sanitise_and_check(path)
    return path[0] != "/"


def decompose(path, normalise=True):
    """
    Splits the path into its component parts.

    Returns a tuple containing (in order):

    * list of path segments
    * revision number
    * absolute path (True/False)

    By default the path will be normalised by collapsing redundant pathing
    operators before decomposing. If this behaviour is not desired it can be
    disabled by setting the normalise keyword argument to False.

    Examples:

    >> decompose("/")
    ([], 0, True)

    >> decompose("/a/b/../c:57")
    (["a", "c"], 57, True)

    >> decompose("../n/../m/:head", normalise=False)
    (["..", "n", "..", "m"], 0, False)

    :param path: A path string.
    :param normalise: Enable/disable path normalisation.
    :return: A tuple containing the decomposed path.
    """

    path = _sanitise_and_check(path)

    # split off and parse revision
    subpath, _, revision = path.partition(":")

    if revision == "" or revision == "head":
        revision = 0
    else:
        revision = int(revision)

    # is this an absolute root path?
    if subpath == "/":
        return [], revision, True
    segments = subpath.split("/")

    # if there is an initial blank segment this is an absolute path
    absolute = segments[0] == ""

    # remove initial blank string if absolute
    if absolute:
        segments = segments[1:]

    if normalise:
        segments = _normalise_path_segments(segments, absolute)

    return segments, revision, absolute


def normalise(path):
    """
    Collapses redundant pathing operators to produce a minimal path.

    For example:

        "/../../../g" becomes "/g"
        "/d/e/f/..:76" becomes "/d/e:76"
        "./a/b/../c:head" becomes "a/c"

    :param path: A path string.
    :return: A normalised path string.
    """

    path = _sanitise_and_check(path)

    # decompose normalises the segments by default
    segments, revision, absolute = decompose(path)
    return _compose(segments, revision, absolute)


def to_absolute(base_path, relative_path):
    """
    Starting from the base path, this routine returns the absolute path
    specified by following the relative path.

    Both the base path and the path returned are absolute paths. The
    returned path will be normalised (see normalise()).

    The revision number of the relative path is used for the returned path.

    :param base_path: An absolute path string.
    :param relative_path: A relative path string.
    :return: A normalised absolute path string.
    """

    base_path = _sanitise_and_check(base_path)
    relative_path = _sanitise_and_check(relative_path)

    if not is_absolute(base_path):
        raise InvalidPath("The base path must be an absolute path.")

    if not is_relative(relative_path):
        raise InvalidPath("The relative path must be a relative path.")

    base_segments, _, _ = decompose(base_path)
    relative_segments, revision, _ = decompose(relative_path)

    new_path = "/" + "/".join(base_segments + relative_segments)

    # attach revision number
    if revision:
        new_path += ":" + str(revision)

    return normalise(new_path)


def _sanitise(path):
    """
    Sanitises strings for path handling.

    This function strips any extraneous whitespace from the path string and
    converts it to lower case.

    :param path: A path string.
    :return: A sanitised path string.
    """

    return path.strip(whitespace).lower()


def _validate(path):
    """
    Tests if a path string conforms to the path specification.

    This methods expects a sanitised path string (see _sanitise())

    :param path: A sanitised path string.
    :return: True if the path is valid, False if not.
    """

    return match(r"^(/|(/?[a-z0-9.\-_]+(/[a-z0-9.\-_]+)*))(:(([0-9]+)|(head)))?$", path) is not None


def _sanitise_and_check(path):
    """
    Sanitises and checks a path is valid.

    Raises a ValueError exception if the path does not conform to the data
    system path specification.

    :param path: A path string.
    :return: A sanitised path string.
    """

    path = _sanitise(path)
    if not _validate(path):
        raise InvalidPath("The supplied path ({}) does not conform to the data system path specification.".format(path))
    return path


def _normalise_path_segments(segments, absolute):
    """
    Normalises the supplied path segments.

    Collapses redundant pathing operators to produce a minimal set of path segments.

    This method expects a valid set of path segments (as exported by decompose()).

    :param segments: Path segments.
    :param absolute: True if the path is absolute, False if path is relative.
    :return: Normalised path segments.
    """

    # filter path segments
    new_segments = []
    for segment in segments:

        if segment == ".":
            continue

        if segment == "..":
            if absolute:
                # absolute paths should not contain any backward steps
                if new_segments:
                    # discard last segment
                    new_segments.pop()
            else:
                # relative paths may start with a chain of backwards steps
                if not new_segments or new_segments[-1] == "..":
                    new_segments.append("..")
                else:
                    # discard last segment
                    new_segments.pop()
        else:
            new_segments.append(segment)

    return new_segments


def _compose(segments, revision, absolute):
    """
    Recombines a decomposed path into a path string.

    This method expects a valid set of path segments (as exported by decompose()).

    :param segments: Path segments.
    :param revision: Revision number.
    :param absolute: True if the path is absolute, False if path is relative.
    :return: A valid path string.
    """

    new_path = "/".join(segments)

    if absolute:
        new_path = "/" + new_path
    else:
        if not new_path:
            new_path = "."

    if revision:
        new_path += ":" + str(revision)

    return new_path

