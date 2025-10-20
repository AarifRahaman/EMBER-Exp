
add_subdirectory(adder)
add_subdirectory(adder_array)
add_subdirectory(dpsram)
add_subdirectory(netlist)

if(BUILD_VERILATOR)
    add_subdirectory(verilator)

    add_custom_target(ember-examples 
        DEPENDS
        ember-int-adder-example
        ember-float-adder-example
        ember-posit-adder-example
        ember-adder-array-example
        ember-dpsram-example
        ember-netlist-example
        ember-verilator-example
        )
else()
    add_custom_target(ember-examples 
        DEPENDS
        ember-int-adder-example
        ember-float-adder-example
        ember-posit-adder-example
        ember-adder-array-example
        ember-netlist-example
        ember-dpsram-example
        )
endif()
