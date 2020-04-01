#
# Heavily influenced by/Taken from https://github.com/StableCoder/cmake-script Thanks and credits to
# it!
#

set(USE_SANITIZER
    ""
    CACHE
        STRING
        "Compile with a sanitizer. Options are: Address, Memory, MemoryWithOrigins, Undefined, Thread, Leak, 'Address;Undefined'"
)

function(append value)
    foreach(variable ${ARGN})
        set(${variable}
            "${${variable}} ${value}"
            PARENT_SCOPE
        )
    endforeach(variable)
endfunction()

if(USE_SANITIZER)
    append("-fno-omit-frame-pointer" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)

    if(UNIX)

        set(USING_CLANG FALSE)
        if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang" OR CMAKE_CXX_COMPILER_ID MATCHES
                                                             "(Apple)?[Cc]lang"
        )
            set(USING_CLANG TRUE)
        endif()
        set(USING_GNU FALSE)
        if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
            set(USING_GNU TRUE)
        endif()

        if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
            append("-O1" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
        endif()

        # Address and UB sanitizer
        if(USE_SANITIZER MATCHES "([Aa]ddress);([Uu]ndefined)" OR USE_SANITIZER MATCHES
                                                                  "([Uu]ndefined);([Aa]ddress)"
        )
            message(STATUS "Building with Address, Undefined sanitizers")
            append("-fsanitize=address,undefined" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)

            # Address sanitizer
        elseif(USE_SANITIZER MATCHES "([Aa]ddress)")
            # Optional: -fno-optimize-sibling-calls -fsanitize-address-use-after-scope
            message(STATUS "Building with Address sanitizer")
            append("-fsanitize=address" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)

            # Memory sanitizer (only Clang)
        elseif(USE_SANITIZER MATCHES "([Mm]emory([Ww]ith[Oo]rigins)?)")
            if(USING_CLANG AND NOT USING_GNU)
                # Optional: -fno-optimize-sibling-calls -fsanitize-memory-track- origins=2
                append("-fsanitize=memory" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
                if(USE_SANITIZER MATCHES "([Mm]emory[Ww]ith[Oo]rigins)")
                    message(STATUS "Building with MemoryWithOrigins sanitizer")
                    append("-fsanitize-memory-track-origins" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
                else()
                    message(STATUS "Building with Memory sanitizer")
                endif()
            else()
                message(
                    STATUS "Can not use Memory sanitizer with GNU compiler. Use Address instead"
                )
            endif()

            # UB sanitizer
        elseif(USE_SANITIZER MATCHES "([Uu]ndefined)")
            message(STATUS "Building with Undefined sanitizer")
            append("-fsanitize=undefined" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
            if(EXISTS "${BLACKLIST_FILE}")
                append("-fsanitize-blacklist=${BLACKLIST_FILE}" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
            endif()

            # Thread sanitizer
        elseif(USE_SANITIZER MATCHES "([Tt]hread)")
            message(STATUS "Building with Thread sanitizer")
            append("-fsanitize=thread" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)

            # Leak sanitizer
        elseif(USE_SANITIZER MATCHES "([Ll]eak)")
            message(STATUS "Building with Leak sanitizer")
            append("-fsanitize=leak" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
        else()
            message(FATAL_ERROR "Unsupported value of USE_SANITIZER: ${USE_SANITIZER}")
        endif()
    elseif(MSVC)
        if(USE_SANITIZER MATCHES "([Aa]ddress)")
            message(STATUS "Building with Address sanitizer")
            append("-fsanitize=address" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
        else()
            message(
                FATAL_ERROR
                    "This sanitizer not yet supported in the MSVC environment: ${USE_SANITIZER}"
            )
        endif()
    else()
        message(FATAL_ERROR "USE_SANITIZER is not supported on this platform.")
    endif()

endif()
