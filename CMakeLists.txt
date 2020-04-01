cmake_minimum_required(VERSION 3.16)
project(include/TomoSect CXX CUDA)

# Add cmake/ folder to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set C++17 without any extensions or anything
include(cxx-standard)
cxx_17()

# Set Cuda standard to 14
include(cuda-standard)
cuda_14()

# Include Sanitizers
include(sanitizers)

# Setup code coverage
include(code-coverage)

# Setup tool like clang-tidy, cppcheck
include(tools)

cppcheck()
clang_tidy()
include_what_you_use()

# Include formatting tools (clang-format, cmake-format)
include(formatting)

# Recursively search in the different paths for CMake files
file(
    GLOB_RECURSE
    CMAKE_FILES
    ${PROJECT_SOURCE_DIR}/cmake/*.cmake
    ${PROJECT_SOURCE_DIR}/include/CMakeLists.txt
    ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt
    ${PROJECT_SOURCE_DIR}/benchmarks/CMakeLists.txt
)

# Add the top level CMakeList file, so we don't include CMake files from dependencies
list(APPEND CMAKE_FILES "${PROJECT_SOURCE_DIR}/CMakeLists.txt")
cmake_format(format-cmake ${CMAKE_FILES})

# Recursively search in the different paths for C++ source and header files
file(
    GLOB_RECURSE
    ALL_CODE_FILES
    ${PROJECT_SOURCE_DIR}/include/*.[ch]pp
    ${PROJECT_SOURCE_DIR}/tests/*.[ch]pp
    ${PROJECT_SOURCE_DIR}/benchmarks/*.[ch]pp
)

clang_format(format-cxx ${ALL_CODE_FILES})

# Fetch dependency
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(
    enoki
    GIT_REPOSITORY https://github.com/mitsuba-renderer/enoki.git
    GIT_TAG origin/master
)

set(ENOKI_CUDA
    ON
    CACHE INTERNAL "Turn on enokis CUDA support"
)
set(ENOKI_PYTHON
    ON
    CACHE INTERNAL "Turn on enokis python bindings"
)
set(ENOKI_AUTODIFF
    OFF
    CACHE INTERNAL "Turn on enokis autodiff support"
)

FetchContent_GetProperties(enoki)
if(NOT enoki_POPULATED)
    FetchContent_Populate(enoki)
    add_subdirectory(${enoki_SOURCE_DIR} ${enoki_BINARY_DIR})
endif()

# Setup output structure (I don't like all the nesting deep into subfolders)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_library(
    tomosect include/TomoSect/geometry.hpp include/TomoSect/intersection.hpp
             include/TomoSect/point.hpp include/TomoSect/vector.hpp
)
target_include_directories(
    tomosect
    PUBLIC $<BUILD_INTERFACE:${enoki_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
           $<INSTALL_INTERFACE:include/> # <prefix>/include/mylib
)

set_target_properties(tomosect PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
add_subdirectory(benchmarks)
