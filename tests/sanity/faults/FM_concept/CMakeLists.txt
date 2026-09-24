add_executable(concept_module_test concept_module.cpp)

target_include_directories(concept_module_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(concept_module_test
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME concept_module_test COMMAND concept_module_test)