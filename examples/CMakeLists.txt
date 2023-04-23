# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com

cmake_minimum_required(VERSION 3.9)

add_library(libread-kb STATIC
    libread-kb.cpp
    libread-kb.h)

add_executable(read-kb-example-daemon
    main.cpp libread-kb.cpp
)

# Specify libraries or flags to use when linking a given target and/or its dependents
target_link_libraries(read-kb-example-daemon
    INTERFACE libread-kb
)
