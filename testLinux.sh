#!/bin/sh
./bin/linux/premake4 --to=./build/test gmake 
cd build/test 
make 
cd ../../
rm -rf ./build/test/ 
./bin/debug/acltests

