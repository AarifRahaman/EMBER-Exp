
### Example Netlist Project

# Validation
add_executable(ember-netlist-example ${CMAKE_CURRENT_LIST_DIR}/validate.cpp)
target_link_libraries(ember-netlist-example PRIVATE ember)
target_include_directories(ember-netlist-example PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-netlist-example PRIVATE VERBOSE=1)
add_test(NAME ember-netlist-example COMMAND ember-netlist-example)

# SEU Injection
add_executable(ember-netlist-seu-injection-example ${CMAKE_CURRENT_LIST_DIR}/inject.cpp)
target_link_libraries(ember-netlist-seu-injection-example PRIVATE ember)
target_include_directories(ember-netlist-seu-injection-example PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-netlist-seu-injection-example PRIVATE VERBOSE=1 SEU=1)
add_test(NAME ember-netlist-seu-injection-example COMMAND ember-netlist-seu-injection-example)

# SA0 Injection
add_executable(ember-netlist-sa0-injection-example ${CMAKE_CURRENT_LIST_DIR}/inject.cpp)
target_link_libraries(ember-netlist-sa0-injection-example PRIVATE ember)
target_include_directories(ember-netlist-sa0-injection-example PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-netlist-sa0-injection-example PRIVATE VERBOSE=1 SA0=1)
add_test(NAME ember-netlist-sa0-injection-example COMMAND ember-netlist-sa0-injection-example)

# SA1 Injection
add_executable(ember-netlist-sa1-injection-example ${CMAKE_CURRENT_LIST_DIR}/inject.cpp)
target_link_libraries(ember-netlist-sa1-injection-example PRIVATE ember)
target_include_directories(ember-netlist-sa1-injection-example PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-netlist-sa1-injection-example PRIVATE VERBOSE=1 SA1=1)
add_test(NAME ember-netlist-sa1-injection-example COMMAND ember-netlist-sa1-injection-example)
