#!/bin/sh
./bin/premake4 --to=./testProjects gmake 
cd test 
make clean && make 
cd ../
rm -rf ./testProjects/ 
./bin/debug/acltests

