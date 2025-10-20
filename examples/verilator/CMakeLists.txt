
project(ember-verilator-example LANGUAGES CXX)

find_package(verilator)

add_executable(ember-verilator-example)
target_include_directories(ember-verilator-example PRIVATE ember verilated-mac ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(ember-verilator-example PRIVATE ember verilated-mac)
target_sources(ember-verilator-example PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.cpp ${CMAKE_CURRENT_LIST_DIR}/mac_wrapper.cpp)
target_compile_definitions(ember-verilator-example PRIVATE VERBOSE=1)
add_test(NAME ember-verilator-example COMMAND ember-verilator-example)

add_subdirectory(vmod)
