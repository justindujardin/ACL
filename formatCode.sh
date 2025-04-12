
find ./tests/ -name "*.cpp" -o -name "*.h" -o -name "*.c" | xargs clang-format -i
find ./acl/ -name "*.cpp" -o -name "*.h" -o -name "*.c" | xargs clang-format -i