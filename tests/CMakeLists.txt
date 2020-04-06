cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.3.7
)

FetchContent_GetProperties(eigen3)
if(NOT eigen3_POPULATED)
    FetchContent_Populate(eigen3)

    add_library(Eigen3 INTERFACE)
    target_include_directories(Eigen3 INTERFACE ${eigen3_SOURCE_DIR})

endif()

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/onqtam/doctest.git
        GIT_TAG origin/master
)

FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
    add_subdirectory(${doctest_SOURCE_DIR} ${doctest_BINARY_DIR})
endif()

# Setup output structure (I don't like all the nesting deep into subfolders)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/tests)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/tests)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/tests)

add_executable(
    tomosect_tests
    test_geometry.cpp
    test_intersection.cpp
    test_main.cpp
    test_point.cpp
    test_vector.cpp
        test_custom_point.cpp
)
target_link_libraries(tomosect_tests PUBLIC tomosect enoki-cuda Eigen3)
target_include_directories(tomosect_tests PUBLIC $<BUILD_INTERFACE:${doctest_SOURCE_DIR}/doctest>)
