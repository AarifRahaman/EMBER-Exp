

target_include_directories(ember
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    )

add_subdirectory(ems_ff)
add_subdirectory(ems_dpsram)
add_subdirectory(ems_stdcells)