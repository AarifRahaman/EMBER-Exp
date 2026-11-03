add_executable(math_module_test math_module.cpp)

target_include_directories(math_module_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(math_module_test
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME math_module_test COMMAND math_module_test)