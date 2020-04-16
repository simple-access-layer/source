# C and C++ client for Simple Access Layer (SAL)

License: European Union Public License 1.2, the same as the SAL project

by Qingfeng Xia(2020~) and Dr Alex Meakins  (2019) UKAEA

Based on Dr Alex Meakins unfinished C++ code base 2019 at 
<https://github.com/simple-access-layer/source/tree/feature/capi>

## Installation

[dependencies installation guide](./doc/Installation.md)

## Build examples and tests

CMake 3 is needed, Ubuntu 18.04 has cmake default to cmake 3.x
```
mkdir build && cd build && cmake .. && make
```
Centos 7.x 
```
mkdir build && cd build && cmake3 .. && make
```

## Deployment

header-only for C++:  just download the headers in the include folder and add them to your project.

git submodule

compiled deb and rpm package for C and C++.

## Funding acknowledge

