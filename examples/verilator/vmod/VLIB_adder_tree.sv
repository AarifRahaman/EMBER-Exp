////////////////////////////////////////////////////////////////////////////////
// @file     VLIB_adder_tree.sv
// @brief    Parameterized Adder Tree Subcomponent of DLA
// @details  
// -----------------------------------------------------------------------------
// @par      Project: DLA
// @author   Jehaan Joseph
// @date     24#06#2024
// @par          Language: SystemVerilog
// @revision:    0.1
//------------------------------------------------------------------------------
// @copyright IHP\n
//     Im Technologiepark 25,\n
//     15236 Frankfurt Oder,\n
//     Germany,\n
//     All rights reserved.
////////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ps

/* verilator lint_off UNUSED */
/* verilator lint_off WIDTH */
/* verilator lint_off UNOPTFLAT */

//`define PIPELINE_MAC

module VLIB_adder_tree #(parameter ATOMIC_C = 8, parameter BITWIDTH = 8)
    (
        NRST,
        CLK,
        VALID_IN,
        IN,
        VALID_OUT,
        RES
    );

    // Calculate output width and number of levels of tree
    localparam INPUT_WIDTH = ATOMIC_C*BITWIDTH;
    localparam NUM_LEVELS = $clog2(ATOMIC_C);
    localparam OUTPUT_WIDTH = BITWIDTH + NUM_LEVELS;
    localparam LINE_STRIDE = ATOMIC_C*OUTPUT_WIDTH;

    input                            NRST, CLK;
    input                            VALID_IN;
    input  signed [INPUT_WIDTH-1:0]  IN /* verilated public */;
    output signed                    VALID_OUT;
    output signed [OUTPUT_WIDTH-1:0] RES /* verilator public */;
    
    
    logic [NUM_LEVELS*LINE_STRIDE-1:0] bus /* verilator public */;
    logic [LINE_STRIDE-1:0]            bus_pipeline_w /* verilator public */;
    logic [LINE_STRIDE-1:0]            bus_pipeline /* verilator public */;
    logic                              VALID_IN_d1, VALID_OUT_reg;

    `ifdef PIPELINE_MAC
    localparam PIPELINE_STAGE = (NUM_LEVELS)/2 - 1;
    localparam NUM_ADDERS_PIPELINE = ATOMIC_C/(2**(PIPELINE_STAGE+1));
    localparam LVL_OUTWIDTH_PIPELINE = BITWIDTH + PIPELINE_STAGE;

    assign bus_pipeline_w = bus[PIPELINE_STAGE*LINE_STRIDE + (NUM_ADDERS_PIPELINE)*(LVL_OUTWIDTH_PIPELINE+1)-1 : PIPELINE_STAGE*LINE_STRIDE];

    always_ff @( posedge CLK ) begin : pipeline_registers
        if ( !NRST ) begin
            VALID_IN_d1 <= 0;
            bus_pipeline <= 0;
        end else begin
            VALID_IN_d1 <= VALID_IN;
            bus_pipeline <= bus_pipeline_w;
        end
    end : pipeline_registers

    assign VALID_OUT_reg = VALID_IN_d1;
    `else
    assign VALID_OUT_reg = VALID_IN;
    `endif

    for (genvar stage = 0; stage < NUM_LEVELS; stage++) begin : gen_1

        localparam NUM_ADDERS = ATOMIC_C/(2**(stage+1));
        localparam LVL_OUTWIDTH = BITWIDTH + stage;

        if ( stage == 0 ) begin : if_1 // first level is inputs to module
            for ( genvar adder = 0; adder < NUM_ADDERS; adder++ ) begin : gen_2
                VLIB_adder #( .BITWIDTH(LVL_OUTWIDTH) ) 
                    ADDER_TREE_add 
                        (
                            IN[(2*adder+1)*LVL_OUTWIDTH-1 : 2*adder*LVL_OUTWIDTH], 
                            IN[(2*adder+2)*LVL_OUTWIDTH-1 : (2*adder+1)*LVL_OUTWIDTH], 
                            bus[(adder+1)*(LVL_OUTWIDTH+1)-1 : adder*(LVL_OUTWIDTH+1)]
                        );
            end
        `ifdef PIPELINE_MAC
            end else if ( stage == (PIPELINE_STAGE + 1) ) begin
                for ( genvar adder = 0; adder < NUM_ADDERS; adder++ ) begin
                    VLIB_adder #( .BITWIDTH(LVL_OUTWIDTH) ) 
                        ADDER_TREE_add
                            (
                                bus_pipeline[(2*adder+1)*(LVL_OUTWIDTH)-1 : 2*adder*(LVL_OUTWIDTH)],
                                bus_pipeline[(2*adder+2)*(LVL_OUTWIDTH)-1 : (2*adder+1)*(LVL_OUTWIDTH)],
                                bus[stage*LINE_STRIDE + (adder+1)*(LVL_OUTWIDTH+1)-1 : stage*LINE_STRIDE + (adder)*(LVL_OUTWIDTH+1)]
                            );
                end
        `endif
        end else begin : else_1
            for ( genvar adder = 0; adder < NUM_ADDERS; adder++ ) begin : gen_3
                VLIB_adder #( .BITWIDTH(LVL_OUTWIDTH) )
                    ADDER_TREE_add
                        (
                            bus[(stage-1)*LINE_STRIDE + (2*adder+1)*(LVL_OUTWIDTH)-1 : (stage-1)*LINE_STRIDE + 2*adder*(LVL_OUTWIDTH)],
                            bus[(stage-1)*LINE_STRIDE + (2*adder+2)*(LVL_OUTWIDTH)-1 : (stage-1)*LINE_STRIDE + (2*adder+1)*(LVL_OUTWIDTH)],
                            bus[stage*LINE_STRIDE + (adder+1)*(LVL_OUTWIDTH+1)-1 : stage*LINE_STRIDE + (adder)*(LVL_OUTWIDTH+1)]
                        );
            end
        end
    end

    
    assign VALID_OUT = VALID_OUT_reg;
    assign RES = $signed( bus[(NUM_LEVELS-1)*LINE_STRIDE + OUTPUT_WIDTH-1 : (NUM_LEVELS-1)*LINE_STRIDE] );
    
endmodule

/* verilator lint_off UNUSED */
/* verilator lint_off WIDTH */
/* verilator lint_off UNOPTFLAT */
