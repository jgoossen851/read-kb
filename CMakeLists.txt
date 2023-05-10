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

# Set platform independent compiler/linker flags
#   C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#   PIC does not depend on absolute address.
#   True by default for SHARED and MODULE library targets, otherwise False
set(CMAKE_POSITION_INDEPENDENT_CODE True)

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

# Enable tests to be added in subdirectories
include(CTest)

# Main Executable
add_subdirectory(app)

# Libraries
add_subdirectory(lib/read-kb)

# Examples
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(examples)
endif()
