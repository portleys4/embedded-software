################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/steve/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/home/steve/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/steve/embedded-software/examples/MSP430F5529/GameExample" --include_path="/home/steve/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --include_path="/C:/Users/Michael/Software/embedded-software/include" --include_path="/C:/Users/Michael/Software/embedded-software/hal/MSP430/MSP430F5529" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

muh_game.obj: /C:/Users/Michael/Software/embedded-software/src/games/muh_game.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/steve/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/home/steve/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/steve/embedded-software/examples/MSP430F5529/GameExample" --include_path="/home/steve/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --include_path="/C:/Users/Michael/Software/embedded-software/include" --include_path="/C:/Users/Michael/Software/embedded-software/hal/MSP430/MSP430F5529" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="muh_game.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


