cmake_minimum_required(VERSION 3.25)

# set the project name and version
project(NodeEmulator VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)



add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME} 
    PUBLIC
        IpAddress.cpp
        Config.cpp
        NodeFactory.cpp
        Requester.cpp
        Receiver.cpp
        Peer.cpp
    PUBLIC FILE_SET HEADERS FILES 
        NodeFactory.h
        Hook.h
        IpAddress.h
        Peer.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME} FILE_SET HEADERS)
