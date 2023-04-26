# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com

cmake_minimum_required(VERSION 3.9)

project(read-kb
        VERSION 0.1.0
        DESCRIPTION "Single-key keyboard reader for interactive program input"
        )

# Set platform dependent compiler/linker flags
if (MSVC)
  # Warning level 4
  add_compile_options(/W4)
  # All warnings as errors
  add_compile_options(/WX)
else()
  # Lots of warnings
  add_compile_options(-Wall -Wextra -pedantic)
  # All warnings as errors
  add_compile_options(-Werror)
endif()

install(PROGRAMS src/read-kb.sh
        DESTINATION bin
        RENAME "${PROJECT_NAME}"
        )

# C++ library to interact with the read-kb executable
add_library(libread-kb STATIC
            src/libread-kb.cpp
            )

target_include_directories(libread-kb PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/read-kb>
        $<INSTALL_INTERFACE:include/read-kb>  # <prefix>/include/read-kb
      )

add_subdirectory(examples)
