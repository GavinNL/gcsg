#
# This CMake project is a boilerplate code for developing a C++ library
#
#
#
#
#
#
#
#
#
#


cmake_minimum_required(VERSION 3.8)

#CPPBOILERPLATELIB
set (CMAKE_CXX_STANDARD 11)


option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)

option(CPPBOILERPLATELIB_USE_CONAN "Use Conan to handle dependencies" OFF)
option(CPPBOILERPLATELIB_ENABLE_COVERAGE "Enable Code Coverage flags" OFF)
option(CPPBOILERPLATELIB_BUILD_TESTS "Build Tests" ON)

#########################################################
FILE(GLOB TEXT_FILES  "*.yml" )
add_custom_target(ci_files    SOURCES ${TEXT_FILES} .travis/install.sh .travis/build.sh conanfile.txt)

FILE(GLOB README_FILES  "*.md" )
add_custom_target(readme    SOURCES ${README_FILES})
#########################################################

# Link this 'library' to use the following warnings
add_library(project_warnings INTERFACE)

#########################################################
# Add the main include folder for our library
#########################################################
set(CPPBOILERPLATELIB_INCLUDE ${PROJECT_SOURCE_DIR}/include)
#########################################################



##
 MACRO(STRING_SPLIT var delimiter str)
     SET(_max_tokens "")
     FOREACH(_arg ${ARGN})
         IF(${_arg} STREQUAL "NOESCAPE_SEMICOLON")
             SET(_NOESCAPE_SEMICOLON "NOESCAPE_SEMICOLON")
         ELSE(${_arg} STREQUAL "NOESCAPE_SEMICOLON")
             SET(_max_tokens ${_arg})
         ENDIF(${_arg} STREQUAL "NOESCAPE_SEMICOLON")
     ENDFOREACH(_arg)

     IF(NOT _max_tokens)
         SET(_max_tokens -1)
     ENDIF(NOT _max_tokens)

     # ';' and '\' are tricky, need to be encoded.
     # '\' => '#B'
     # '#' => '#H'
     STRING(REGEX REPLACE "#" "#H" _str "${str}")
     STRING(REGEX REPLACE "#" "#H" _delimiter "${delimiter}")

     STRING(REGEX REPLACE "\\\\" "#B" _str "${_str}")

     IF(NOT _NOESCAPE_SEMICOLON STREQUAL "")
         # ';' => '#S'
         STRING(REGEX REPLACE ";" "#S" _str "${_str}")
         STRING(REGEX REPLACE ";" "#S" _delimiter "${_delimiter}")
     ENDIF(NOT _NOESCAPE_SEMICOLON STREQUAL "")

     SET(_str_list "")
     SET(_token_count 0)
     STRING(LENGTH "${_delimiter}" _de_len)

     WHILE(NOT _token_count EQUAL _max_tokens)
         #MESSAGE("_token_count=${_token_count} _max_tokens=${_max_tokens} _str=${_str}")
         MATH(EXPR _token_count ${_token_count}+1)
         IF(_token_count EQUAL _max_tokens)
             # Last token, no need splitting
             SET(_str_list ${_str_list} "${_str}")
         ELSE(_token_count EQUAL _max_tokens)
             # in case encoded characters are delimiters
             STRING(LENGTH "${_str}" _str_len)
             SET(_index 0)
             #MESSAGE("_str_len=${_str_len}")
             SET(_token "")
             SET(_str_remain "")
             MATH(EXPR _str_end ${_str_len}-${_de_len}+1)
             SET(_bound "k")
             WHILE(_index LESS _str_end)
                 STRING(SUBSTRING "${_str}" ${_index} ${_de_len} _str_cursor)
                 #MESSAGE("_index=${_index} _str_cursor=${_str_cursor} _de_len=${_de_len} _delimiter=|${_delimiter}|")
                 IF(_str_cursor STREQUAL _delimiter)
                     # Get the token
                     STRING(SUBSTRING "${_str}" 0 ${_index} _token)
                     # Get the rest
                     MATH(EXPR _rest_index ${_index}+${_de_len})
                     MATH(EXPR _rest_len ${_str_len}-${_index}-${_de_len})
                     STRING(SUBSTRING "${_str}" ${_rest_index} ${_rest_len} _str_remain)
                     SET(_index ${_str_end})
                 ELSE(_str_cursor STREQUAL _delimiter)
                     MATH(EXPR _index ${_index}+1)
                 ENDIF(_str_cursor STREQUAL _delimiter)
             ENDWHILE(_index LESS _str_end)

             #MESSAGE("_token=${_token} _str_remain=${_str_remain}")

             IF(_str_remain STREQUAL "")
                 # Meaning: end of string
                 SET(_str_list ${_str_list} "${_str}")
                 SET(_max_tokens ${_token_count})
             ELSE(_str_remain STREQUAL "")
                 SET(_str_list ${_str_list} "${_token}")
                 SET(_str "${_str_remain}")
             ENDIF(_str_remain STREQUAL "")
         ENDIF(_token_count EQUAL _max_tokens)
         #MESSAGE("_token_count=${_token_count} _max_tokens=${_max_tokens} _str=${_str}")
     ENDWHILE(NOT _token_count EQUAL _max_tokens)


     # Unencoding
     STRING(REGEX REPLACE "#B" "\\\\" _str_list "${_str_list}")
     STRING(REGEX REPLACE "#H" "#" _str_list "${_str_list}")

     IF(NOT _NOESCAPE_SEMICOLON STREQUAL "")
         # ';' => '#S'
         STRING(REGEX REPLACE "#S" "\\\\;" ${var} "${_str_list}")
     ELSE(NOT _NOESCAPE_SEMICOLON STREQUAL "")
         SET(${var} ${_str_list})
     ENDIF(NOT _NOESCAPE_SEMICOLON STREQUAL "")

 ENDMACRO(STRING_SPLIT var delimiter str)
##

#########################################################
# If we have asked to use CONAN, we should execute the
# conan process and gather all the include paths
# and libraries required
#########################################################
if(CPPBOILERPLATELIB_USE_CONAN)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      # using Clang
      set(CONAN_COMPILER clang)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      # using GCC
      set(CONAN_COMPILER gcc)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      # using Intel C++
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
      # using Visual Studio C++
      set(CONAN_COMPILER "Visual Studio")
    endif()


    STRING_SPLIT( XX "." "${CMAKE_CXX_COMPILER_VERSION}" )
    set (MY_LIST a b c d)
    list (GET XX 0 MAJOR)
    list (GET XX 1 MINOR)
    #message ("HEAD = ${HEAD}")
    set(CXX_COMPILER_VER ${MAJOR}.${MINOR} )
    message("COMPILER VERSION: ${MAJOR}.${MINOR}" )


    message(" COMPILER DETECTED: ${CONAN_COMPILER} ${MAJOR}.${MINOR}")

    #execute_process (
    #    COMMAND bash -c "conan install ${CMAKE_SOURCE_DIR} --build"
    #    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    #)

    #execute_process (
    #    COMMAND bash -c "conan install ${CMAKE_SOURCE_DIR} --build=missing -s compiler=${CONAN_COMPILER} -s compiler.version=${CXX_COMPILER_VER}"
    #    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    #)

   include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
   conan_basic_setup()

   link_libraries(${CONAN_LIBS})
endif()
#########################################################


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


#########################################################
# If we hhave enabled coverage, make sure to add the
# specific flags required to calculate the coverage
#########################################################
if(CPPBOILERPLATELIB_ENABLE_COVERAGE)

  if(CMAKE_COMPILER_IS_GNUCC)
    option(CPPBOILERPLATELIB_ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

    add_compile_options(--coverage -g -O0 -fprofile-arcs -ftest-coverage)
  endif()

endif()
#########################################################

if(MSVC)
  target_compile_options(project_warnings INTERFACE /W4)
else()
    if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.1)
        add_compile_options(
                               -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                                                  # non-virtual destructor. This helps catch hard to
                                                  # track down memory errors
                               -Wold-style-cast # warn for c-style casts
                               -Wcast-align # warn for potential performance problem casts
                               -Wunused # warn on anything being unused
                               -Woverloaded-virtual # warn if you overload (not override) a virtual
                                                    # function
                               -Wpedantic # warn if non-standard C++ is used
                               -Wconversion # warn on type conversions that may lose data
                               -Wsign-conversion # warn on sign conversions
                               -Wmisleading-indentation # warn if identation implies blocks where blocks
                                                        # do not exist
                               -Wduplicated-cond # warn if if / else chain has duplicated conditions
                               -Wduplicated-branches # warn if if / else branches have duplicated code
                               -Wlogical-op # warn about logical operations being used where bitwise were
                                            # probably wanted
                               -Wnull-dereference # warn if a null dereference is detected
                               -Wuseless-cast # warn if you perform a cast to the same type
                               -Wdouble-promotion # warn if float is implicit promoted to double
                               -Wformat=2 # warn on security issues around functions that format output
                                          # (ie printf)
                               )
    else()
        add_compile_options(
                               -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                                                  # non-virtual destructor. This helps catch hard to
                                                  # track down memory errors
                               -Wold-style-cast # warn for c-style casts
                               -Wcast-align # warn for potential performance problem casts
                               -Wunused # warn on anything being unused
                               -Woverloaded-virtual # warn if you overload (not override) a virtual
                                                    # function
                               -Wpedantic # warn if non-standard C++ is used
                               -Wconversion # warn on type conversions that may lose data
                               -Wsign-conversion # warn on sign conversions
                               -Wformat=2 # warn on security issues around functions that format output
                                          # (ie printf)
                               )
    endif()
  #target_compile_options(project_warnings
  #                       INTERFACE
  #                       -Werror
  #                       -Wall
  #                       -Wextra # reasonable and standard
  #                       -Wshadow # warn the user if a variable declaration shadows one from a
  #                                # parent context
  #                       -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
  #                                          # non-virtual destructor. This helps catch hard to
  #                                          # track down memory errors
  #                       -Wold-style-cast # warn for c-style casts
  #                       -Wcast-align # warn for potential performance problem casts
  #                       -Wunused # warn on anything being unused
  #                       -Woverloaded-virtual # warn if you overload (not override) a virtual
  #                                            # function
  #                       -Wpedantic # warn if non-standard C++ is used
  #                       -Wconversion # warn on type conversions that may lose data
  #                       -Wsign-conversion # warn on sign conversions
  #                       -Wmisleading-indentation # warn if identation implies blocks where blocks
  #                                                # do not exist
  #                       -Wduplicated-cond # warn if if / else chain has duplicated conditions
  #                       -Wduplicated-branches # warn if if / else branches have duplicated code
  #                       -Wlogical-op # warn about logical operations being used where bitwise were
  #                                    # probably wanted
  #                       -Wnull-dereference # warn if a null dereference is detected
  #                       -Wuseless-cast # warn if you perform a cast to the same type
  #                       -Wdouble-promotion # warn if float is implicit promoted to double
  #                       -Wformat=2 # warn on security issues around functions that format output
  #                                  # (ie printf)
  #                       )
endif()






# Maros

#########################################################
# Macro to find all sub directories within a folder
#########################################################
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
#########################################################



######################
# Get a list of all the subdirectories in the src folder. Each of
# these subdirectories is compiled into its own library
# which will be linked with all unit tests
SUBDIRLIST(CPPBOILERPLATE_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/src)


if(CPPBOILERPLATELIB_ENABLE_COVERAGE)
    set(CPPBOILERPLATELIB_COVERAGE_FLAGS "--coverage -g -O0 -fprofile-arcs -ftest-coverage")
    MESSAGE("Setting Coverage flags: ${CPPBOILERPLATELIB_COVERAGE_FLAGS} ")
endif()



enable_testing()


#########################################################
# Add the src sub directory. Each subdirectory within
# the src folder will be compiled into its own library
#########################################################
add_subdirectory(src)
#########################################################


#########################################################
# Add the test sub directory. This is where all unit tests
# will be stored. Each unit test will be compiled and
# linked with all the libraryes in the src/ folder
#########################################################
if( ${CPPBOILERPLATELIB_BUILD_TESTS})
add_subdirectory(test)
endif()
#########################################################


#########################################################
# Add the examples directory. Each sub directory within
# this folder will be compiled into an executable and
# linked with all the libraries compiled in the src/ foler
# plus all the dependent libraries defined in the conanfile
#########################################################
add_subdirectory(examples)
#########################################################

