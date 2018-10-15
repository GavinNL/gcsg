cmake_minimum_required(VERSION 3.10)

project(gcgs)
add_executable(${PROJECT_NAME} "main.cpp")


add_executable(2d "examples/2d.cpp")

add_executable(tests "tests/tests.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(.)

add_definitions(-std=c++17)
add_definitions(-DGCGS_USE_SPDLOG)
