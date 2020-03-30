cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/onqtam/doctest.git
    GIT_TAG        origin/master
)

FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
    add_subdirectory(${doctest_SOURCE_DIR} ${doctest_BINARY_DIR})
endif()

add_executable(tomosect_tests
        test_intersection.cpp
        test_main.cpp
        test_vector.cpp
        test_point.cpp
        test_geometry.cpp
        )
target_link_libraries(tomosect_tests PUBLIC tomosect enoki-cuda)
target_include_directories(tomosect_tests PUBLIC
        $<BUILD_INTERFACE:${doctest_SOURCE_DIR}/doctest>)