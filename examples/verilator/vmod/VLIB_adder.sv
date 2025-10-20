////////////////////////////////////////////////////////////////////////////////
// @file     VLIB_adder.sv
// @brief    Basic Two-Way Adder Subcomponent of DLA
// @details  
// -----------------------------------------------------------------------------
// @par      Project: DLA
// @author   Jehaan Joseph
// @date     08#05#2024
// @par          Language: SystemVerilog
// @revision:    0.1
//------------------------------------------------------------------------------
// @copyright IHP\n
//     Im Technologiepark 25,\n
//     15236 Frankfurt Oder,\n
//     Germany,\n
//     All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/* verilator lint_off UNOPTFLAT */

module VLIB_adder #(parameter BITWIDTH = 16)
    (
        IN_A,
        IN_B,
        RES 
    );

    input signed [BITWIDTH-1:0] IN_A, IN_B;
    output signed [BITWIDTH:0] RES;
    
    assign RES = $signed(IN_A) + $signed(IN_B);
    
endmodule

/* verilator lint_off UNOPTFLAT */
