#!/bin/sh

echo "C++ unit tests and client demo for SAL, PWD = ${PWD}"
ls ../build/bin
cd ../build/bin

# run all app with sal_*
./sal_test_attributes
./sal_demo