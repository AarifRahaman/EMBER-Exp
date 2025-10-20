
### Example dpsram Project

add_executable(ember-dpsram-example ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(ember-dpsram-example PRIVATE ember)
target_include_directories(ember-dpsram-example PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_test(NAME ember-dpsram-example COMMAND ember-dpsram-example)