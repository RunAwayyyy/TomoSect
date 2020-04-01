cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    celero
    GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
    GIT_TAG origin/master
)

FetchContent_GetProperties(celero)
if(NOT celero_POPULATED)
    FetchContent_Populate(celero)
    add_subdirectory(${celero_SOURCE_DIR} ${celero_BINARY_DIR})
endif()

# Setup output structure (I don't like all the nesting deep into subfolders)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/benchmarks)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/benchmarks)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/benchmarks)

add_executable(benchmark_rect_intersection bench_rect_intersection.cpp)
target_link_libraries(benchmark_rect_intersection PUBLIC tomosect celero enoki-cuda)

add_executable(benchmark_aabb_intersection bench_aabb_intersection.cpp)
target_link_libraries(benchmark_aabb_intersection PUBLIC tomosect celero enoki-cuda)

add_executable(benchmark_ray_gen bench_ray_generation.cpp)
target_link_libraries(benchmark_ray_gen PUBLIC tomosect celero enoki-cuda)
