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

install(PROGRAMS src/read-kb.sh
        DESTINATION bin
        RENAME "${PROJECT_NAME}"
        )