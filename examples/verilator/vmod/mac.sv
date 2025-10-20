////////////////////////////////////////////////////////////////////////////////
// @file     mac.sv
// @brief    MAC Array component of DLA
// @details  
// -----------------------------------------------------------------------------
// @par      Project: DLA
// @author   Jehaan Joseph
// @date     21#05#2024
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

module mac 
    #(
        parameter ATOMIC_C = 4,
        parameter BITWIDTH = 8,
        parameter BITWIDTH_B_MUL = BITWIDTH
    ) (
        NRST,
        CLK,
        VALID_IN,
        IN_A, 
        IN_B,
        RES,
        VALID_OUT
    );

    localparam MUL_OUT_WIDTH = BITWIDTH + BITWIDTH_B_MUL;
    localparam OUTPUT_WIDTH = MUL_OUT_WIDTH + $clog2(ATOMIC_C);

    input                                     NRST, CLK;
    input                                     VALID_IN;
    input        [ATOMIC_C-1:0][BITWIDTH-1:0] IN_A;
    input  [ATOMIC_C-1:0][BITWIDTH_B_MUL-1:0] IN_B;
    output                 [OUTPUT_WIDTH-1:0] RES;
    output                                    VALID_OUT;
    
    logic                                   VALID_IN_d1    /* verilator public */;
    logic                                   VALID_OUT_wire /* verilator public */;
    logic                                   VALID_OUT_reg  /* verilator public */;
    
    logic [ATOMIC_C-1:0][MUL_OUT_WIDTH-1:0] mul2add_reg    /* verilator public */;
    logic [ATOMIC_C-1:0][MUL_OUT_WIDTH-1:0] mul2add_wire   /* verilator public */;
    
    logic                [OUTPUT_WIDTH-1:0] tree_out_reg   /* verilator public */;
    logic                [OUTPUT_WIDTH-1:0] tree_out_wire  /* verilator public */;
    
    for (genvar i = 0; i < ATOMIC_C; i++) begin: gen_mult
        VLIB_multiplier #( .BITWIDTH_A(BITWIDTH), .BITWIDTH_B(BITWIDTH_B_MUL) )
            MAC_multiplier_array
                (
                    .IN_A(IN_A[i]),
                    .IN_B(IN_B[i]),
                    .RES(mul2add_wire[i])
                );
    end

    VLIB_adder_tree #( .ATOMIC_C(ATOMIC_C), .BITWIDTH(MUL_OUT_WIDTH) )
        MAC_adder_tree
            (
                .NRST(NRST),
                .CLK(CLK),
                .VALID_IN(VALID_IN_d1),
                .IN(mul2add_reg),
                .VALID_OUT(VALID_OUT_wire),
                .RES(tree_out_wire)
            ); 
    
    always_ff @( posedge CLK ) begin
        if ( !NRST ) begin
            VALID_IN_d1 <= 0;
            VALID_OUT_reg <= 0;
            mul2add_reg <= 0;
            tree_out_reg <= 0;
        end else begin
            VALID_IN_d1 <= VALID_IN;
            VALID_OUT_reg <= VALID_OUT_wire;
            mul2add_reg <= mul2add_wire;
            tree_out_reg <= tree_out_wire;
        end
    end
    
    assign RES = tree_out_reg;
    assign VALID_OUT = VALID_OUT_reg;
    
endmodule
