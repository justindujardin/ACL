#!/bin/sh
./bin/premake4 --to=./build/test gmake 
cd build/test 
make clean && make 
cd ../../
rm -rf ./build/test/ 
./bin/debug/acltests

