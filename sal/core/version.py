"""
Parses the package version file and exports it to constant.

The package version string is stored in the sal/VERSION text file.
"""

from os import path

# parse the package version number
with open(path.join(path.dirname(__file__), 'VERSION')) as version_file:
    VERSION = version_file.read().strip()


