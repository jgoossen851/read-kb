# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com

cmake_minimum_required(VERSION 3.9)

set(TARGET_NAME "read-kb-app")

add_executable("${TARGET_NAME}"
  main.cpp
)

target_include_directories("${TARGET_NAME}"
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../lib/read-kb/include/read-kb>
          $<INSTALL_INTERFACE:include/read-kb>  # <prefix>/include/read-kb
)

# Specify libraries or flags to use when linking a given target and/or its dependents
target_link_libraries("${TARGET_NAME}"
  PRIVATE read-kb
)

set_target_properties("${TARGET_NAME}"
  PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
)
install(TARGETS "${TARGET_NAME}"
  DESTINATION bin
)
