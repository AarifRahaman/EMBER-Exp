
#####################
### Verilate MAC

add_library(verilated-mac STATIC "")

verilate(verilated-mac
    VERILATOR_ARGS -cc -Wall --trace --trace-structs --clk CLK --timescale 1ns/1ps
    TOP_MODULE mac
    INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}
    SOURCES ${CMAKE_CURRENT_LIST_DIR}/mac.sv
    )

