#
# Heavily influenced by/Taken from https://github.com/StableCoder/cmake-script Thanks and credits to it!
#

find_package(Doxygen)
find_package(Sphinx)

option(BUILD_DOCUMENTATION "Build API documentation using Doxygen. (make doc)" ${DOXYGEN_FOUND})

# Builds doxygen documentation with a default 'Doxyfile.in' or with a specified one, and can make the results installable (under the `doc`
# install target)
#
# This can only be used once per project, as each target generated is as `doc-${PROJECT_NAME}` unless TARGET_NAME is specified.
# ~~~
# Optional Arguments:
#
# ADD_TO_DOC
#   If specified, adds this generated target to be a dependency of the more general
#   `doc` target.
#
# INSTALLABLE
#   Adds the generated documentation to the generic `install` target, under the
#   `documentation` installation group.
#
# PROCESS_DOXYFILE
#   If set, then will process the found Doxyfile through the CMAKE `configure_file`
#   function for macro replacements before using it. (@ONLY)
#
# TARGET_NAME <str>
#   The name to give the doc target. (Default: doc-${PROJECT_NAME})
#
# OUTPUT_DIR <str>
#   The directory to place the generated output. (Default: ${CMAKE_CURRENT_BINARY_DIR}/doc)
#
# INSTALL_PATH <str>
#   The path to install the documentation under. (if not specified, defaults to
#  'share/${PROJECT_NAME})
#
# DOXYFILE_PATH <str>
#   The given doxygen file to use/process. (Defaults to'${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile')
#
# SPHINX_PATH <str>
#   The given sphinx config file to use/process. (Defaults to'${CMAKE_CURRENT_SOURCE_DIR}/config.py')
# ~~~
function(build_docs)
    set(OPTIONS
        ADD_TO_DOC
        INSTALLABLE
        PROCESS_DOXYFILE
        USE_SPHINX
        PROCESS_SPHINX_CONFIG
    )
    set(SINGLE_VALUE_KEYWORDS
        TARGET_NAME
        INSTALL_PATH
        DOXYFILE_PATH
        SPHINX_PATH
        OUTPUT_DIR
    )
    set(MULTI_VALUE_KEYWORDS)
    cmake_parse_arguments(
        build_docs
        "${OPTIONS}"
        "${SINGLE_VALUE_KEYWORDS}"
        "${MULTI_VALUE_KEYWORDS}"
        ${ARGN}
    )

    if(BUILD_DOCUMENTATION)
        if(NOT DOXYGEN_FOUND)
            message(FATAL_ERROR "Doxygen is needed to build the documentation.")
        endif()

        if(USE_SPHINX AND NOT SPHINX_EXECUTABLE)
            message(FATAL_ERROR "Sphinx is needed to build the documentation.")
        endif()

        if(NOT build_docs_DOXYFILE_PATH)
            set(DOXYFILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
        elseif(EXISTS ${build_docs_DOXYFILE_PATH})
            set(DOXYFILE_PATH ${build_docs_DOXYFILE_PATH})
        else()
            set(DOXYFILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${build_docs_DOXYFILE_PATH})
        endif()

        if(NOT EXISTS ${DOXYFILE_PATH})
            message(SEND_ERROR "Could not find Doxyfile to use for processing documentation at: ${DOXYFILE_PATH}")
            return()
        endif()

        if(build_docs_PROCESS_DOXYFILE)
            set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
            configure_file(${DOXYFILE_PATH} ${DOXYFILE} @ONLY)
        else()
            set(DOXYFILE ${DOXYFILE_PATH})
        endif()

        if(build_docs_OUTPUT_DIR)
            set(OUT_DIR ${build_docs_OUTPUT_DIR})
        else()
            set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc)
        endif()

        file(MAKE_DIRECTORY ${OUT_DIR})

        if(build_docs_TARGET_NAME)
            set(TARGET_NAME ${build_docs_TARGET_NAME})
        else()
            set(TARGET_NAME doc-${PROJECT_NAME})
        endif()

        add_custom_target(
            ${TARGET_NAME}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
            WORKING_DIRECTORY ${OUT_DIR}
            VERBATIM
        )

        if(build_docs_USE_SPHINX AND SPHINX_EXECUTABLE)
            if(NOT build_docs_SPHINX_PATH)
                set(SPHINX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config.py)
            elseif(EXISTS ${build_docs_SPHINX_PATH})
                set(SPHINX_PATH ${build_docs_SPHINX_PATH})
            else()
                set(SPHINX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${build_docs_SPHINX_PATH})
            endif()

            if(NOT EXISTS ${SPHINX_PATH})
                message(SEND_ERROR "Could not find config.py to use for processing documentation at: ${SPHINX_PATH}")
                return()
            endif()

            if(build_docs_PROCESS_SPHINX_CONFIG)
                message(STATUS "Processing sphinx config file")
                configure_file(${SPHINX_PATH}/conf.py.in ${SPHINX_PATH}/conf.py @ONLY)
            endif()

            add_custom_target(
                ${TARGET_NAME}-sphinx
                COMMAND
                    ${SPHINX_EXECUTABLE} -b html -Dbreathe_projects.test=${OUT_DIR}/xml # breathe config
                    ${SPHINX_PATH} # input dir for sphinx
                    ${OUT_DIR} # output dir for sphinx
                WORKING_DIRECTORY ${OUT_DIR}
                VERBATIM
            )
            add_dependencies(${TARGET_NAME}-sphinx ${TARGET_NAME})
        endif()

        if(build_docs_ADD_TO_DOC)
            if(NOT TARGET doc)
                add_custom_target(doc)
            endif()

            if(build_docs_USE_SPHINX AND SPHINX_EXECUTABLE)
                add_dependencies(doc ${TARGET_NAME}-sphinx)
            else()
                add_dependencies(doc ${TARGET_NAME})
            endif()
        endif()

        if(build_docs_INSTALLABLE)
            if(NOT build_docs_INSTALL_PATH)
                set(build_docs_INSTALL_PATH share/${PROJECT_NAME})
            endif()
            install(
                DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/
                COMPONENT documentation
                DESTINATION ${build_docs_INSTALL_PATH}
            )
        endif()
    endif()
endfunction()
