# Set the cuda compiler to C++98
macro(cuda_98)
    set(CMAKE_CUDA_STANDARD 98)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endmacro()

# Set the cuda compiler to C++11
macro(cuda_11)
    set(CMAKE_CUDA_STANDARD 11)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endmacro()

# Set the cuda compiler to C++14
macro(cuda_14)
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endmacro()
