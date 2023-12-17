# Renaisscript

A perfect language to artistically entertain linguists whose hobby is learning
Shakespearean/Old English.

## Contribution

See [CONTRIBUTING.md](./CONTRIBUTING.md) for standard coding conventions used.
Consider it as the guide for contributing changes in this project.

## Setup

Required programs for development:

- [Git:](https://git-scm.com/downloads) versioning system
- [CMake:](https://cmake.org/) build system

### Windows

N/A

### Linux

1. Clone the repository

    ```console
    git clone https://github.com/steguiosaur/renaisscript.git
    cd renaisscript
    ```

2. Generate CMake build configuration of type Debug or Release

    > Generate build type Release

    ```console
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    ```

    > **OPTIONAL:** Generate build with Debug enabled option

    ```console
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    ```

3. Build executable to `build/` directory

    ```console
    cmake --build build
    ```

4. Run `renaisscript` in `$PROJECT_ROOT/` directory

    ```console
    ./build/renaisscript -o <binaryname> <filename>.rens
    ```

## Licenses

MIT License

Copyright (c) 2023 Barbin, Ciudad, Pabular, Pe, Quiray, Soriano, Victuelles

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
