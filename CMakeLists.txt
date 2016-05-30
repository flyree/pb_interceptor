cmake_minimum_required(VERSION 3.3)
project(pb_interceptor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    faultinjection.cpp
    faultinjection.h
    fi_cjmp_map.h
    instcategory.cpp
    instcount.cpp
    instselector.cpp
    instselector.h
    memtrack.cpp
    memtrack.h
    utils.cpp
    utils.h 
    libload.cpp libload.h randomInst.cpp determineInst.cpp)

add_executable(pb_interceptor ${SOURCE_FILES})
