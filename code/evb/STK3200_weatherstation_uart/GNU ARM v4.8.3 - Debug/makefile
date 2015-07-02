################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Graphics/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/efm32zg/subdir.mk
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
all: STK3200_weatherstation_uart.axf

# Tool invocations
STK3200_weatherstation_uart.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -T "STK3200_weatherstation_uart.ld" -Xlinker --gc-sections -Xlinker -Map="STK3200_weatherstation_uart.map" --specs=nano.specs -o STK3200_weatherstation_uart.axf "./src/background.o" "./src/graphics.o" "./src/weatherstation.o" "./emlib/em_adc.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_int.o" "./emlib/em_leuart.o" "./emlib/em_rtc.o" "./emlib/em_usart.o" "./Graphics/dmd_display.o" "./Graphics/glib.o" "./Graphics/glib_bitmap.o" "./Graphics/glib_circle.o" "./Graphics/glib_font_normal_8x8.o" "./Graphics/glib_line.o" "./Graphics/glib_rectangle.o" "./Graphics/glib_string.o" "./Drivers/display.o" "./Drivers/displayls013b7dh03.o" "./Drivers/displaypalemlib.o" "./Drivers/i2cdrv.o" "./Drivers/rtcdriver.o" "./Drivers/si1147_i2c.o" "./Drivers/si114x_algorithm.o" "./Drivers/si114x_functions.o" "./Drivers/si7013.o" "./Drivers/udelay.o" "./CMSIS/efm32zg/startup_efm32zg.o" "./CMSIS/efm32zg/system_efm32zg.o" "./BSP/bsp_bcc.o" "./BSP/bsp_stk.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: STK3200_weatherstation_uart.hex'
	arm-none-eabi-objcopy -O ihex "STK3200_weatherstation_uart.axf" "STK3200_weatherstation_uart.hex"
	@echo ' '

	@echo 'Building bin file: STK3200_weatherstation_uart.bin'
	arm-none-eabi-objcopy -O binary "STK3200_weatherstation_uart.axf" "STK3200_weatherstation_uart.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "STK3200_weatherstation_uart.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) STK3200_weatherstation_uart.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
