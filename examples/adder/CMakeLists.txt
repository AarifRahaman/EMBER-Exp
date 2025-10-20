
### Example Adder Project

# INTEGER
add_executable(ember-int-adder-validate ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(ember-int-adder-validate PRIVATE ember)
target_include_directories(ember-int-adder-validate PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-int-adder-validate PRIVATE VERBOSE=1 FAULTNUM=0)
add_test(NAME ember-int-adder-validate COMMAND ember-int-adder-validate)

add_executable(ember-int-adder-seu-example ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(ember-int-adder-seu-example PRIVATE ember)
target_include_directories(ember-int-adder-seu-example PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(ember-int-adder-seu-example PRIVATE VERBOSE=1 FAULTNUM=1 RUNONFAILS=1)
add_test(NAME ember-int-adder-seu-example COMMAND ember-int-adder-seu-example)

# Alias
add_custom_target(ember-adder-examples 
    DEPENDS
    ember-int-adder-validate
    ember-int-adder-seu-example
    )