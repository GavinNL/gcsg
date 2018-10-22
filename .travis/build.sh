#!/bin/bash

set -e
set -x


# INSTALLATION
if [[ "$(uname -s)" == 'Darwin' ]]; then

    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

conan user

mkdir build
cd build

echo ################################################
echo $CPPFLAGS
echo ################################################
echo $LIBS
echo ################################################

## # script for Clang - detects compiler version and passes to conan
## export CC=clang
## export CXX=clang++
## COMPILER_MAJOR=$(touch test.c && $CC -E -dM test.c  | grep -o '__clang_major__ [[:digit:]]' | cut -d' ' -f2)
## COMPILER_MINOR=$(touch test.c && $CC -E -dM test.c  | grep -o '__clang_minor__ [[:digit:]]' | cut -d' ' -f2)
## conan install -s compiler=clang -s compiler.version=$COMPILER_MAJOR.$COMPILER_MINOR ...
##
##
##
## # script for GCC - detects compiler version and passes to conan
## export CC=gcc
## export CXX=g++
## COMPILER_MAJOR=$(touch test.c && $CC -E -dM test.c  | grep -o '__GNUC__ [[:digit:]]' | cut -d' ' -f2)
## COMPILER_MINOR=$(touch test.c && $CC -E -dM test.c  | grep -o '__GNUC_MINOR__ [[:digit:]]' | cut -d' ' -f2)
## conan install -s compiler=gcc -s compiler.version=$COMPILER_MAJOR.$COMPILER_MINOR ...


conan install .. --build

# compile and execute unit tests
if [ "${RUN_CODECOV}" = "TRUE" ]; then 
    echo "Building with Code Coverage"
    cmake ${CMAKE_OPTIONS} -GNinja -D GCSG_USE_CONAN:BOOL=TRUE -D GCSG_ENABLE_COVERAGE:BOOL=TRUE -D GCSG_BUILD_TESTS:BOOL=TRUE ..
    cmake --build . -- -j2
else
    echo "Building without Code Coverage"
    cmake ${CMAKE_OPTIONS} -GNinja -D GCSG_USE_CONAN:BOOL=TRUE -D GCSG_BUILD_TESTS:BOOL=TRUE  ..
    cmake --build . -- -j2
fi

#   - CXX=/usr/bin/g++-7 CC=/usr/bin/gcc-7 cmake -D GCSG_ENABLE_COVERAGE:BOOL=TRUE -D GCSG_BUILD_TESTS:BOOL=TRUE ..
#   - cmake --build . -- -j2

#ctest -C Release -V -j
ctest -j2

#if [ ${RUN_CODECOV} = TRUE ]; then 
#    bash <(curl -s "https://codecov.io/bash")
#fi
