# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com

cmake_minimum_required(VERSION 3.5.1)

# Set the CMake target name and files for this subdirectory
set(TARGET_NAME "test-lib-read-kb")

set(SOURCE_FILES
  read-kb.test.cpp
)

set(INCLUDE_FILES
)

# Create a new target with the source and header files as dependencies
add_executable("${TARGET_NAME}" ${SOURCE_FILES} ${INCLUDE_FILES})

# The keywords PUBLIC, PRIVATE, and INTERFACE are used for both Include
# Directories and Link Libraries.
# * PUBLIC means that other libraries including or linking thistarget should
#   also include or link the listed directory.
# * PRIVATE means that this target must include or link the directory while
#   other libraries including or linking this target do not.
# * INTERFACE means that other libraries including or linking this target
#   should include or link the listed directory while this target does not.

# Specify include directories to use when compiling the given target
target_include_directories("${TARGET_NAME}"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/read-kb"
)

# Specify libraries or flags to use when linking a given target and/or its dependents
target_link_libraries("${TARGET_NAME}"
  PUBLIC read-kb
)

# Add build-in self test to test list
file(COPY_FILE
  "${CMAKE_CURRENT_SOURCE_DIR}/res/input.txt"
  "${CMAKE_CURRENT_BINARY_DIR}/input.txt"
)
add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
