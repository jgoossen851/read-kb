# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com

cmake_minimum_required(VERSION 3.9)

add_executable(read-kb-daemon-cpp
  read-kb-daemon-cpp.cpp
)

target_include_directories(read-kb-daemon-cpp
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/read-kb>
          $<INSTALL_INTERFACE:include/read-kb>  # <prefix>/include/read-kb
)

# Specify libraries or flags to use when linking a given target and/or its dependents
target_link_libraries(read-kb-daemon-cpp
  PRIVATE libread-kb
)
