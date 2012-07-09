# The name of our project is "ACL". CMakeLists files in this project can 
# refer to the root source directory of the project as ${ACL_SOURCE_DIR} and 
# to the root binary directory of the project as ${ACL_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.6) 
project (ACL) 

set(CMAKE_OSX_ARCHITECTURES i386 CACHE STRING "do not build for 64-bit" FORCE)
set(ARCH i386)


file(GLOB_RECURSE ACL_SOURCES "acl/*.cpp")
file(GLOB_RECURSE ACL_HEADERS "acl/*.h")

set (ACL_INCLUDE_DIRS "")
foreach (_headerFile ${ACL_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND ACL_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES ACL_INCLUDE_DIRS)

include_directories(${ACL_INCLUDE_DIRS})
add_library (ACL STATIC ${ACL_SOURCES})
