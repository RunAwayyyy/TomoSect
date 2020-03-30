cmake_minimum_required(VERSION 3.16)
project(include/TomoSect
        CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        enoki
        GIT_REPOSITORY https://github.com/mitsuba-renderer/enoki.git
        GIT_TAG        origin/master
)

set(ENOKI_CUDA ON)

FetchContent_GetProperties(enoki)
if(NOT enoki_POPULATED)
    FetchContent_Populate(enoki)
    add_subdirectory(${enoki_SOURCE_DIR} ${enoki_BINARY_DIR})
endif()

add_library(tomosect
        include/TomoSect/vector.hpp
        include/TomoSect/point.hpp
        include/TomoSect/geometry.hpp
        include/TomoSect/intersection.hpp
        )
target_include_directories(tomosect PUBLIC
        $<BUILD_INTERFACE:${enoki_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>  # <prefix>/include/mylib
        )

set_target_properties(tomosect PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
add_subdirectory(benchmarks)