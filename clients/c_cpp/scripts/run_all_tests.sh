#!/bin/sh

echo "C++ unit tests and client demo for SAL, PWD = ${PWD}"
ls ../build/bin
cd ../build/bin

# run all app with sal_*
./sal_test_attributes

if [[ $? == 0 ]]; then
    ./sal_demo
fi

if [[ $? -ne 0 ]]; then
    echo "demo exit with error, ignore for the moment"
fi