################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7/platform/Device/SiliconLabs/EFR32MG21/Source/system_efr32mg21.c 

S_UPPER_SRCS += \
/Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7/platform/Device/SiliconLabs/EFR32MG21/Source/GCC/startup_efr32mg21.S 

OBJS += \
./CMSIS/EFR32MG21/startup_efr32mg21.o \
./CMSIS/EFR32MG21/system_efr32mg21.o 

C_DEPS += \
./CMSIS/EFR32MG21/system_efr32mg21.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFR32MG21/startup_efr32mg21.o: /Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7/platform/Device/SiliconLabs/EFR32MG21/Source/GCC/startup_efr32mg21.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -c -x assembler-with-cpp -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//hardware/kit/EFR32MG21_BRD4181A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//platform/Device/SiliconLabs/EFR32MG21/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//hardware/kit/common/bsp" '-DEFR32MG21A010F1024IM32=1' -mfpu=fpv5-sp-d16 -mfloat-abi=softfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFR32MG21/system_efr32mg21.o: /Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7/platform/Device/SiliconLabs/EFR32MG21/Source/system_efr32mg21.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32MG21A010F1024IM32=1' -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//hardware/kit/EFR32MG21_BRD4181A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//platform/Device/SiliconLabs/EFR32MG21/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v2.7//hardware/kit/common/bsp" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv5-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"CMSIS/EFR32MG21/system_efr32mg21.d" -MT"CMSIS/EFR32MG21/system_efr32mg21.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


