add_executable(random_module_test random_module.cpp)

target_include_directories(random_module_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(random_module_test
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME random_module_test COMMAND random_module_test)