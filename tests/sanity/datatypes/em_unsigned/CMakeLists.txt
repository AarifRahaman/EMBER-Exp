add_executable(unsigned_container_test unsigned_container.cpp)

target_include_directories(unsigned_container_test
    PRIVATE
    ${PROJECT_SOURCE_DIR})

target_link_libraries(unsigned_container_test
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME unsigned_container_test COMMAND unsigned_container_test)