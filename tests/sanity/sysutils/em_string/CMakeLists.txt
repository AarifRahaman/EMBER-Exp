add_executable(string_module_test string_module.cpp)

target_include_directories(string_module_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(string_module_test
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME string_module_test COMMAND string_module_test)