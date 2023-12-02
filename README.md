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
