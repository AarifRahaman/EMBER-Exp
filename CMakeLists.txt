cmake_minimum_required(VERSION 3.20)

project(ember
    VERSION 0.4.1
    DESCRIPTION "The EMBER Framework"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/ProjectOptions.cmake)
include(cmake/Catch2.cmake)

add_library(ember STATIC)

target_compile_features(ember PUBLIC cxx_std_20)

include(cmake/Warnings.cmake)

add_subdirectory(ember)




if(BUILD_EXAMPLES OR BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_SABOTEURS)
    add_subdirectory(saboteurs)
endif()

if(BUILD_VERILATOR)
    add_subdirectory(verilator)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


include(cmake/Packaging.cmake)
