#!/bin/sh

cd ./bin/debug/
echo "Switching to debug directory..."
if [ $# -gt 0 ]; then
  echo "Filtering tests by wildcard match: $@"
  exec gdb --args acltests --gtest_filter=$@
else
  echo "Debugging all tests"
  exec gdb acltests
fi
echo "Backing out of debug directory..."
cd ../../
