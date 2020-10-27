################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/src/subdir.mk
-include CMSIS/EFR32MG21/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: simple_touch_xg21.axf

# Tool invocations
simple_touch_xg21.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "simple_touch_xg21.ld" -Xlinker --gc-sections -Xlinker -Map="simple_touch_xg21.map" -mfpu=fpv5-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o simple_touch_xg21.axf "./BSP/bsp_stk_leds.o" "./CMSIS/EFR32MG21/startup_efr32mg21.o" "./CMSIS/EFR32MG21/system_efr32mg21.o" "./Drivers/src/capsense_xg21.o" "./emlib/em_acmp.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_prs.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./src/main.o" 
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: simple_touch_xg21.hex'
	arm-none-eabi-objcopy -O ihex "simple_touch_xg21.axf" "simple_touch_xg21.hex"
	@echo ' '

	@echo 'Building bin file: simple_touch_xg21.bin'
	arm-none-eabi-objcopy -O binary "simple_touch_xg21.axf" "simple_touch_xg21.bin"
	@echo ' '

	@echo 'Building s37 file: simple_touch_xg21.s37'
	arm-none-eabi-objcopy -O srec "simple_touch_xg21.axf" "simple_touch_xg21.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "simple_touch_xg21.axf" -A
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) simple_touch_xg21.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
