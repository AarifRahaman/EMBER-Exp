
### Example Adder Array Project

add_executable(ember-adder-array-example ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(ember-adder-array-example PRIVATE ember)
target_include_directories(ember-adder-array-example PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-adder-array-example PRIVATE VERBOSE=1)
add_test(NAME ember-adder-array-example COMMAND ember-adder-array-example)