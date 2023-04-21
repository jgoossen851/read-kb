# read-kb

Single-key keyboard reader for interactive program input

## Installation

```bash
cmake -S . -B build/                   # Generate build directory and configure project
cmake --build build/ --targets install # Install program
```

Making the `install` target installs the following:

* Program `read-kb` to `/usr/local/bin/`

## Usage

First make the `install` target.

### read-kb

```bash
read-kb
# Press any key to print it's name - either a letter or a special key like "Shft-Ctrl-Alt-F8"
```
