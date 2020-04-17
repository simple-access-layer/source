#!/bin/sh

# the current working directory is where bash call this script
echo "start build task at the folder: $(pwd)"

# include is the key folder in the project root
if [ ! -d include ]; then
    cd ..
    if [ ! -d include ]; then
        echo "this script must be started in project root folder"
    fi
fi

if [ ! -d build ]; then
    mkdir build
fi
cd build

# add this to support cpprestsdk -DCMAKE_PREFIX_PATH=/usr/local/lib/cmake
CXX=g++-8 cmake .. 
make -j 4

# test if build succeeded
if [[ $? == 0 ]]; then
    cp ../scripts/run_all_tests.sh ./ 
    bash ./run_all_tests.sh
fi