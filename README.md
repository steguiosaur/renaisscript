# Renaisscript

A perfect language to artistically entertain linguists whose hobby is learning
Shakespearean/Old English.

## Contribution

See [CONTRIBUTING.md](./CONTRIBUTING.md) for standard coding conventions used.
Consider it as the guide for contributing changes in this project.

## Development Setup

Required programs for development:

- [Git:](https://git-scm.com/downloads) versioning system
- [CMake:](https://cmake.org/) build system
- [CTest:](https://cmake.org/cmake/help/latest/manual/ctest.1.html) testing
framework (integrated with CMake)

Optional programs:

- [clang-tidy:](https://clang.llvm.org/extra/clang-tidy/) linter
- [clang-format:](https://clang.llvm.org/docs/ClangFormat.html) formatter

### Windows

N/A

### Linux

1. Clone the repository

    ```console
    git clone https://github.com/steguiosaur/renaisscript.git
    cd renaisscript
    ```

2. Generate CMake build configuration folder of type `MinSizeRel`

    > **NOTE:** Execute for every repository update or new file creations

    ```console
    cmake -B build
    ```

3. Build executable to `build/` directory

    ```console
    cmake --build build
    ```

4. Run `renaisscript` in `$PROJECT_ROOT/` directory

    ```console
    ./build/renaisscript <filename>.rens
    ```

5. Test using `ctest` executable (integrated with CMake)

    ```console
    ctest --test-dir build --output-on-failure
    ```

6. **DEBUGGING:** Enable debug options (requires regeneration of build files)

    > Preferred for stepping through with a debugger

    ```console
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    ```

    > Type release with debug enabled options

    ```console
    cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
    cmake --build build
    ```

## Licenses

Renaisscript is available under the MIT license.
See the [LICENSE](./LICENSE) file for more info.
