option(BUILD_EXAMPLES  "Build EMBER examples"                                                    OFF)
option(BUILD_TESTS     "Generate build files for unit tests"                                     ON)
option(BUILD_VERILATOR "Compile verilator submodules (requires verilator version >= 4.0; < 5.0)" OFF)
option(BUILD_SABOTEURS "Compile default saboteurs library"                                       ON)

if((NOT DEFINED CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL ""))
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()