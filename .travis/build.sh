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

conan install .. --build

# compile and execute unit tests
if [ "${RUN_CODECOV}" = "TRUE" ]; then 
    echo "Building with Code Coverage"
    cmake ${CMAKE_OPTIONS} -GNinja -D CPPBOILERPLATELIB_USE_CONAN:BOOL=TRUE -D CPPBOILERPLATELIB_ENABLE_COVERAGE:BOOL=TRUE -D CPPBOILERPLATELIB_BUILD_TESTS:BOOL=TRUE ..
    cmake --build . -- -j2
else
    echo "Building without Code Coverage"
    cmake ${CMAKE_OPTIONS} -GNinja -D CPPBOILERPLATELIB_USE_CONAN:BOOL=TRUE -D CPPBOILERPLATELIB_BUILD_TESTS:BOOL=TRUE  ..
    cmake --build . -- -j2
fi

#   - CXX=/usr/bin/g++-7 CC=/usr/bin/gcc-7 cmake -D CPPBOILERPLATELIB_ENABLE_COVERAGE:BOOL=TRUE -D CPPBOILERPLATELIB_BUILD_TESTS:BOOL=TRUE ..
#   - cmake --build . -- -j2

#ctest -C Release -V -j
ctest -j2

#if [ ${RUN_CODECOV} = TRUE ]; then 
#    bash <(curl -s "https://codecov.io/bash")
#fi
