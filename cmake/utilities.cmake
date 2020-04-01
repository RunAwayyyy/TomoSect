include(FetchContent)

#
# This puts the FetchContent in the default build dirs and stuff For my cases this was always
# sufficient
#
# Download steps are only provided for git. Uses the same variables as given in
# https://cmake.org/cmake/help/v3.11/module/ExternalProject.html for the git documentation
function(fetch_content_git NAME GIT_REPOSITORY)
    set(OPTIONS GIT_SHALLOW GIT_PROGRESS)
    set(SINGLE_VALUE_KEYWORDS GIT_TAG GIT_REMOTE_NAME)
    set(MULTI_VALUE_KEYWORDS GIT_SUBMODULES GIT_CONFIG)
    cmake_parse_arguments(
        fetch_content_git
        "${OPTIONS}"
        "${SINGLE_VALUE_KEYWORDS}"
        "${MULTI_VALUE_KEYWORDS}"
        ${ARGN}
    )

    find_package(Git 1.6.5 REQUIRED)

    if(NOT DEFINED fetch_content_git_GIT_SHALLOW)
        set(GIT_SHALLOW TURE)
    else()
        set(GIT_SHALLOW ${fetch_content_git_GIT_SHALLOW})
    endif()

    if(NOT DEFINED fetch_content_git_GIT_PROGRESS)
        set(GIT_PROGRESS TURE)
    else()
        set(GIT_PROGRESS ${fetch_content_git_GIT_PROGRESS})
    endif()

    if(NOT DEFINED fetch_content_git_GIT_TAG)
        set(GIT_TAG "origin/master")
    else()
        set(GIT_TAG ${fetch_content_git_GIT_TAG})
    endif()

    if(NOT DEFINED fetch_content_git_GIT_TAG)
        set(GIT_REMOTE_NAME "origin")
    else()
        set(GIT_REMOTE_NAME ${fetch_content_git_GIT_REMOTE_NAME})
    endif()

    if(NOT DEFINED fetch_content_git_GIT_SUBMODULES)
        set(GIT_SUBMODULES "")
    else()
        set(GIT_SUBMODULES ${fetch_content_git_GIT_SUBMODULES})
    endif()

    if(NOT DEFINED fetch_content_git_GIT_CONFIG)
        set(GIT_CONFIG "")
    else()
        set(GIT_CONFIG ${fetch_content_git_GIT_CONFIG})
    endif()

    FetchContent_Declare(
        ${NAME}
        GIT_REPOSITORY ${GIT_REPOSITORY}
        GIT_TAG ${GIT_TAG}
        GIT_REMOTE_NAME ${GIT_REMOTE_NAME}
        GIT_SUBMODULES ${GIT_SUBMODULES}
        GIT_SHALLOW ${GIT_SHALLOW}
        GIT_PROGRESS ${GIT_PROGRESS}
        GIT_CONFIG ${GIT_CONFIG}
    )

    FetchContent_GetProperties(${NAME})
    if(NOT ${NAME}_POPULATED)
        FetchContent_Populate(${NAME})
        add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR})
    endif()
endfunction()
