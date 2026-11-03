add_executable(tick_core_tests core_func.cpp)

add_executable(tick_template_tests template_func.cpp)

target_include_directories(tick_core_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_include_directories(tick_template_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(tick_core_tests
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

target_link_libraries(tick_template_tests
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME tick_core_tests COMMAND tick_core_tests)

add_test(NAME tick_template_tests COMMAND tick_template_tests)
