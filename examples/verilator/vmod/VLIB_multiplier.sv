////////////////////////////////////////////////////////////////////////////////
// @file     VLIB_multiplier.sv
// @brief    Basic Two-Way multiplier Subcomponent of DLA
// @details  Two-Way Multiplier with asymmetric input sizes
// -----------------------------------------------------------------------------
// @par      Project: DLA
// @author   Jehaan Joseph
// @date     10#05#2024
// @par          Language: SystemVerilog
// @revision:    0.1
//------------------------------------------------------------------------------
// @copyright IHP\n
//     Im Technologiepark 25,\n
//     15236 Frankfurt Oder,\n
//     Germany,\n
//     All rights reserved.
////////////////////////////////////////////////////////////////////////////////

module VLIB_multiplier #(parameter BITWIDTH_A = 8, parameter BITWIDTH_B = BITWIDTH_A)
    (
        IN_A,
        IN_B,
        RES 
    );

    localparam OUTPUT_WIDTH = BITWIDTH_A + BITWIDTH_B;

    input  signed   [BITWIDTH_A-1:0] IN_A;
    input  signed   [BITWIDTH_B-1:0] IN_B;
    output signed [OUTPUT_WIDTH-1:0] RES;
    
    assign RES = $signed(IN_A) * $signed(IN_B);
    
endmodule
