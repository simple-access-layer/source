from setuptools import setup, find_packages
from os import path

# parse the package version number
with open(path.join(path.dirname(__file__), 'sal/core/VERSION')) as version_file:
    VERSION = version_file.read().strip()

setup(
    name='sal',
    version=VERSION,
    packages=find_packages(),
    include_package_data=True,
    install_requires=['requests', 'numpy', 'flask'],
    url='https://github.com/simpleaccesslayer',
    license='EUPL v1.2',
    author='Dr Alex Meakins',
    author_email='alex.meakins@ukaea.uk',
    description='Simple Access Layer: A general purpose data system framework.'

)
