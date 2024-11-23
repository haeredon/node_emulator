cmake_minimum_required(VERSION 3.25)

# set the project name and version
project(NodeEmulator VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)


# add the executable
add_executable("${PROJECT_NAME}" 
    main.cpp
)

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}")

