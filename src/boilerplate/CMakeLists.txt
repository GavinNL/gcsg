#
# Cmake file which compiles all cpp files within this folder
# and subfolders into a single library file which is
# named after the folder.
#
#
# All compiled modules are automatically linked into the test cases
# cases
#
cmake_minimum_required(VERSION 3.8)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include_directories( ${CPPBOILERPLATELIB_INCLUDE} )

FILE(GLOB_RECURSE  SRC_FILES  "*.cpp" )

add_library( ${ProjectId} ${SRC_FILES})
