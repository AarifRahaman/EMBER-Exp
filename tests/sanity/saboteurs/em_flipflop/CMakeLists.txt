add_executable(flipflop_saboteur_test flipflop.cpp)

target_include_directories(flipflop_saboteur_test
    PRIVATE
    ${PROJECT_SOURCE_DIR})

target_link_libraries(flipflop_saboteur_test
    PRIVATE
    ember
    Catch2::Catch2WithMain
)

add_test(NAME flipflop_saboteur_test COMMAND flipflop_saboteur_test)