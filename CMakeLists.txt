cmake_minimum_required(VERSION 3.1)
enable_testing()

## Project
project(ember
	VERSION 0.4
	LANGUAGES CXX)

option(DEBUG           "Add debug build options to all the targets"                              OFF)
option(BUILD_EXAMPLES  "Build EMBER examples"                                                    OFF)
option(BUILD_TESTS     "Generate build files for unit tests"                                     OFF)
option(BUILD_VERILATOR "Compile verilator submodules (requires verilator version >= 4.0; < 5.0)" OFF)
option(BUILD_SABOTEURS "Compile default saboteurs library"                                        ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_library(ember STATIC)

## Subdirectories
add_subdirectory(ember)

## Build Extras

# Build Saboteurs Library
if(BUILD_SABOTEURS)
	add_subdirectory(saboteurs)
endif()

# Build Verilator Extension
if(BUILD_VERILATOR)
	add_subdirectory(verilator)
endif()

# Build Examples
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
