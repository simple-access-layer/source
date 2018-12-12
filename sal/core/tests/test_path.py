import unittest

import jetserver.jetvfs.path as pth

VALID_SEGMENTS = [
    "abcdefghijklmnopqrstuvwxyz._-01234567890",
    "zeta", "18234", ".", "_", "-"
]

INVALID_SEGMENTS = [
    "", "ABC", ":", "\\", "\"", "abcDEF", "ABCdef"
]


VALID_PATHS = [
    "/abcdefghijklmnopqrstuzwxyz0123456789-_.:0",
    "/", "/:head", "/:0", "/:288",
    "/abc", "/abc:head", "/abc:0", "/abc:7362",
    "/abc/EFG/637", "/abc/EFG/637:head", "/abc/EFG/637:0", "/abc/EFG/637:145",
    ".", ".:head", ".:0", ".:5",
    "../abc", "../abc:head", "../abc:0", "../abc:912",
    "../a/b/../c", "../a/b/../c:head", "../a/b/../c:0", "../a/b/../c:912",
    "     /a/b/c:head   ", "     a/b/c:head   "
]

INVALID_PATHS = [
    "",
    "/abc/", "abc/" "12$34", "!\"£$%^&*()",
    "/:", ":", "/:cat", "/:-636", "/:636.3",
    ".:", ".:", ".:cat", ".:-636", ".:636.3",
    "/abc:", "/abc:cat", "/abc:-636", "/abc:636.3",
    "//", "abc//sdf", "//abc",
    "abc/      def:3"
]

ABSOLUTE_PATHS = [
    "/abcdefghijklmnopqrstuzwxyz0123456789-_.:0",
    "/", "/:head", "/:0", "/:288",
    "/abc", "/abc:head", "/abc:0", "/abc:7362",
    "/abc/EFG/637", "/abc/EFG/637:head", "/abc/EFG/637:0", "/abc/EFG/637:145",
    "     /a/b/c:head   "
]

RELATIVE_PATHS = [
    ".", ".:head", ".:0", ".:5",
    "../abc", "../abc:head", "../abc:0", "../abc:912",
    "../a/b/../c", "../a/b/../c:head", "../a/b/../c:0", "../a/b/../c:912",
    "     a/b/c:head   "
]


class TestPath(unittest.TestCase):

    def setUp(self):

        self.valid_segments = VALID_SEGMENTS
        self.invalid_segments = INVALID_SEGMENTS
        self.valid_paths = VALID_PATHS
        self.invalid_paths = INVALID_PATHS
        self.absolute_paths = ABSOLUTE_PATHS
        self.relative_paths = RELATIVE_PATHS

        # path, result tuple
        self.decomposed_paths_normalised = [
            ("/", ([], 0, True)),
            ("/:head", ([], 0, True)),
            ("/:0", ([], 0, True)),
            ("/:123", ([], 123, True)),
            ("/abc", (["abc"], 0, True)),
            ("/abc/../EFG/637:145", (["efg", "637"], 145, True)),
            (".", ([], 0, False)),
            (".:head", ([], 0, False)),
            (".:0", ([], 0, False)),
            (".:76", ([], 76, False)),
            ("../abc/efg", (["..", "abc", "efg"], 0, False)),
            ("../abc/efg:1", (["..", "abc", "efg"], 1, False)),
            ("/../../a/../b", (["b"], 0, True))
        ]

        # path, result tuple
        self.decomposed_paths_unnormalised = [
            ("/", ([], 0, True)),
            ("/:head", ([], 0, True)),
            ("/:0", ([], 0, True)),
            ("/:123", ([], 123, True)),
            ("/abc", (["abc"], 0, True)),
            ("/abc/../EFG/637:145", (["abc", "..", "efg", "637"], 145, True)),
            (".", (["."], 0, False)),
            (".:head", (["."], 0, False)),
            (".:0", (["."], 0, False)),
            (".:76", (["."], 76, False)),
            ("../abc/efg", (["..", "abc", "efg"], 0, False)),
            ("../abc/efg:1", (["..", "abc", "efg"], 1, False)),
            ("/../../a/../b", (["..", "..", "a", "..", "b"], 0, True))
        ]

        # path, result
        self.normalise_paths = [
            ("/abc/def/ghi", "/abc/def/ghi"),
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
            (".:98", ".:98")
        ]

        # base_path, relative_path, result
        self.to_absolute_paths = [
            ("/", ".", "/"),
            ("/", "a", "/a"),
            ("/:76", "a", "/a"),
            ("/:76", "a:245", "/a:245"),
            ("/", "a:245", "/a:245"),
            ("/abc/def/ghi", "../../ijk", "/abc/ijk"),
            ("/abc/def/../ghi", "../../ijk", "/ijk"),
            ("/abc/def/ghi", "../../ijk/..", "/abc"),
            ("/abc/def/ghi", "../../../..:100", "/:100"),
            ("/abc/def/ghi:98", "../../../..:head", "/")
        ]

    def test_is_valid(self):

        # test valid paths
        for path in self.valid_paths:
            self.assertTrue(pth.is_valid(path), "Valid path " + path + " incorrectly reported as False (invalid).")

        # test invalid paths
        for path in self.invalid_paths:
            self.assertFalse(pth.is_valid(path), "Invalid path " + path + " incorrectly reported as True (valid).")

    def test_segment_valid(self):

        # test valid segments
        for segment in self.valid_segments:
            self.assertTrue(pth.segment_valid(segment), "Valid path segment " + segment + " incorrectly reported as False (invalid).")

        # test invalid segments
        for segment in self.invalid_segments:
            self.assertFalse(pth.segment_valid(segment), "Invalid path segment" + segment + " incorrectly reported as True (valid).")

    def test_is_absolute(self):

        # test invalid paths
        for path in self.invalid_paths:
            with self.assertRaises(ValueError, msg="Invalid path " + path + " did not raise a ValueError Exception"):
                pth.is_absolute(path)

        # test absolute paths
        for path in self.absolute_paths:
            self.assertTrue(pth.is_absolute(path), "Absolute path " + path + " incorrectly reported as False (relative).")

        # test relative paths
        for path in self.relative_paths:
            self.assertFalse(pth.is_absolute(path), "Relative path " + path + " incorrectly reported as True (absolute).")

    def test_is_relative(self):

        # test invalid paths
        for path in self.invalid_paths:
            with self.assertRaises(ValueError, msg="Invalid path " + path + " did not raise a ValueError Exception"):
                pth.is_relative(path)

        # test relative paths
        for path in self.relative_paths:
            self.assertTrue(pth.is_relative(path), "Relative path " + path + " incorrectly reported as False (absolute).")

        # test absolute paths
        for path in self.absolute_paths:
            self.assertFalse(pth.is_relative(path), "Absolute path " + path + " incorrectly reported as True (relative).")

    def test_decompose(self):

        # test invalid paths
        for path in self.invalid_paths:
            with self.assertRaises(ValueError, msg="Invalid path " + path + " did not raise a ValueError Exception"):
                pth.decompose(path)

        # test path decomposition, default (normalised)
        for path, parts in self.decomposed_paths_normalised:
            self.assertEqual(parts, pth.decompose(path), "Path " + path + " incorrectly decomposed.")

        # test path decomposition, normalised
        for path, parts in self.decomposed_paths_normalised:
            self.assertEqual(parts, pth.decompose(path, normalise=True), "Path " + path + " incorrectly decomposed.")

        # test path decomposition, unnormalised
        for path, parts in self.decomposed_paths_unnormalised:
            self.assertEqual(parts, pth.decompose(path, normalise=False), "Path " + path + " incorrectly decomposed.")

    def test_normalise(self):

        # test invalid paths
        for path in self.invalid_paths:
            with self.assertRaises(ValueError, msg="Invalid path " + path + " did not raise a ValueError Exception"):
                pth.normalise(path)

        # test path normalisation
        for path, normalised in self.normalise_paths:
            self.assertEqual(normalised, pth.normalise(path), "Path " + path + " incorrectly decomposed.")

    def test_to_absolute(self):

        # test invalid base paths
        for path in self.invalid_paths + self.relative_paths:
            with self.assertRaises(ValueError, msg="Invalid base path " + path + " did not raise a ValueError Exception"):
                pth.to_absolute(path, ".")

        # test invalid relative paths
        for path in self.invalid_paths + self.absolute_paths:
            with self.assertRaises(ValueError, msg="Invalid relative path " + path + " did not raise a ValueError Exception"):
                pth.to_absolute("/", path)

        # test to_absolute produces valid results
        for basepath, relpath, result in self.to_absolute_paths:
            self.assertEqual(result, pth.to_absolute(basepath, relpath), "Relative path " + relpath + " incorrectly combined with base path " + basepath + ".")
