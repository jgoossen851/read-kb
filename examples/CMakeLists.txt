# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com

cmake_minimum_required(VERSION 3.9)

set(TARGET_NAME "simple-ux-cpp")

add_executable("${TARGET_NAME}"
  main.cpp
)

# Specify include directories to use when compiling the given target
target_include_directories("${TARGET_NAME}"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../lib/read-kb/include/read-kb"
)

# Specify libraries or flags to use when linking a given target and/or its dependents
target_link_libraries("${TARGET_NAME}"
  PUBLIC read-kb
)

file(COPY_FILE
  ${CMAKE_CURRENT_SOURCE_DIR}/simple-ux-bash.sh
  ${CMAKE_CURRENT_BINARY_DIR}/simple-ux-bash
)
