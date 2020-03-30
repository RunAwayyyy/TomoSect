# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.17.0)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(celero-populate NONE)

include(ExternalProject)
ExternalProject_Add(celero-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://github.com/DigitalInBlue/Celero.git" "GIT_TAG" "origin/master"
                    SOURCE_DIR          "/home/ner0/CLionProjects/TomoSect/build/_deps/celero-src"
                    BINARY_DIR          "/home/ner0/CLionProjects/TomoSect/build/_deps/celero-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)
