# File Structure

Files and folders located on `$PROJECT_ROOT`

## Folders

- `docs/` - contains documentation relevant to the project
- `include/` - include header files separating the implementation to interface
- `src/` - contains the implementing part of the project
- `res/` - additional resources like images and config files
- `lib/` - stores additional package modules developed as part of the project
- `test/` - test related files
- `build/` (ignored) - stores CMake generated file and the binary file

## Files

- `CMakeLists.txt` - CMake configuration file defining how the project is built
- `README.md` - information about the project
- `.gitignore` - ignored files by Git
- `.clang-format` - code formatting rules
- `.clang-tidy` - `clang-tidy` linter options
