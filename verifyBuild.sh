#!/bin/bash

# Move to top-level project directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
pushd "${SCRIPT_DIR}"

# Build project
cmake -S . -B build/                        || exit
sudo cmake --build build/ --target install  || exit

popd
