cmake_minimum_required(VERSION 3.25)
project(
  renaisscript
  VERSION 0.1.0
  LANGUAGES C)
set(CMAKE_C_STANDARD 17)

# Default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Speed up compilation process using ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "$(CCACHE_PROGRAM)")
endif()

# Generate compile_commands.json used for building process
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Add all source files for linking and compilation
file(GLOB SOURCES src/*.c)
add_executable(renaisscript ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")

