#
# Heavily influenced by/Taken from https://github.com/StableCoder/cmake-script Thanks and credits to it!
#

option(CLANG_TIDY "Turns on clang-tidy processing if it is found." OFF)
option(IWYU "Turns on include-what-you-use processing if it is found." OFF)
option(CPPCHECK "Turns on cppcheck processing if it is found." OFF)

# Adds clang-tidy checks to the compilation, with the given arguments being used as the options set.
macro(clang_tidy)
    if(CLANG_TIDY AND CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} ${ARGN})
    endif()
endmacro()

macro(target_fix_clang_tidy target)
    if(CLANG_TIDY AND CLANG_TIDY_EXE)
        set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_EXE} "-fix")
    endif()
endmacro()

# Adds include_what_you_use to the compilation, with the given arguments being used as the options set.
macro(include_what_you_use)
    if(IWYU AND IWYU_EXE)
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_EXE};${IWYU_STRING}")
    endif()
endmacro()

# Adds cppcheck to the compilation, with the given arguments being used as the options set.
macro(cppcheck)
    if(CPPCHECK AND CPPCHECK_EXE)
        set(CMAKE_CXX_CPPCHECK "${CPPCHECK_EXE};${CPPCHECK_STRING}")
    endif()
endmacro()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
mark_as_advanced(FORCE CLANG_TIDY_EXE)
if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    if(NOT CLANG_TIDY)
        message(STATUS "clang-tidy NOT ENABLED via 'CLANG_TIDY' variable!")
        set(CMAKE_CXX_CLANG_TIDY
            ""
            CACHE STRING "" FORCE
        ) # delete it
    endif()
elseif(CLANG_TIDY)
    message(SEND_ERROR "Cannot enable clang-tidy, as executable not found!")
    set(CMAKE_CXX_CLANG_TIDY
        ""
        CACHE STRING "" FORCE
    ) # delete it
else()
    message(STATUS "clang-tidy not found!")
    set(CMAKE_CXX_CLANG_TIDY
        ""
        CACHE STRING "" FORCE
    ) # delete it
endif()

find_program(IWYU_EXE NAMES "include-what-you-use")
mark_as_advanced(FORCE IWYU_EXE)
if(IWYU_EXE)
    message(STATUS "include-what-you-use found: ${IWYU_EXE}")
    if(NOT IWYU)
        message(STATUS "include-what-you-use NOT ENABLED via 'IWYU' variable!")
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
            ""
            CACHE STRING "" FORCE
        ) # delete it
    else()
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
            "${IWYU_EXE};-Xiwyu;any;-Xiwyu;iwyu;-Xiwyu;args"
            CACHE STRING "" FORCE
        ) # delete it
    endif()
elseif(IWYU)
    message(SEND_ERROR "Cannot enable include-what-you-use, as executable not found!")
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
        ""
        CACHE STRING "" FORCE
    ) # delete it
else()
    message(STATUS "include-what-you-use not found!")
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
        ""
        CACHE STRING "" FORCE
    ) # delete it
endif()

find_program(CPPCHECK_EXE NAMES "cppcheck")
mark_as_advanced(FORCE CPPCHECK_EXE)
if(CPPCHECK_EXE)
    message(STATUS "cppcheck found: ${CPPCHECK_EXE}")
    if(CPPECHECK)
        set(CMAKE_CXX_CPPCHECK
            "${CPPCHECK_EXE};--enable=warning,performance,portability,missingInclude;--template=\"[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)\";--suppress=missingIncludeSystem;--quiet;--verbose;--force"
        )
    endif()
    if(NOT CPPCHECK)
        message(STATUS "cppcheck NOT ENABLED via 'CPPCHECK' variable!")
        set(CMAKE_CXX_CPPCHECK
            ""
            CACHE STRING "" FORCE
        ) # delete it
    endif()
elseif(CPPCHECK)
    message(SEND_ERROR "Cannot enable cppcheck, as executable not found!")
    set(CMAKE_CXX_CPPCHECK
        ""
        CACHE STRING "" FORCE
    ) # delete it
else()
    message(STATUS "cppcheck not found!")
    set(CMAKE_CXX_CPPCHECK
        ""
        CACHE STRING "" FORCE
    ) # delete it
endif()
