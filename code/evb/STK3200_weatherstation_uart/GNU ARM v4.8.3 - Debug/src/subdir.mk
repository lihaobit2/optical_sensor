################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/background.c \
../src/graphics.c \
../src/weatherstation.c 

OBJS += \
./src/background.o \
./src/graphics.o \
./src/weatherstation.o 

C_DEPS += \
./src/background.d \
./src/graphics.d \
./src/weatherstation.d 


# Each subdirectory must supply rules for building sources it contributes
src/background.o: ../src/background.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DEFM32ZG222F32=1' '-DDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/weatherstation" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/common/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/config" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/gpiointerrupt/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/nvm/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/rtcdrv/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/sleep/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/spidrv/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/SiliconLabs/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -Os -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/background.d" -MT"src/background.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/graphics.o: ../src/graphics.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DEFM32ZG222F32=1' '-DDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/weatherstation" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/common/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/config" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/gpiointerrupt/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/nvm/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/rtcdrv/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/sleep/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/spidrv/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/SiliconLabs/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -Os -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/graphics.d" -MT"src/graphics.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/weatherstation.o: ../src/weatherstation.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DEFM32ZG222F32=1' '-DDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/weatherstation" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/common/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/config" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/gpiointerrupt/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/nvm/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/rtcdrv/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/sleep/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emdrv/spidrv/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/SiliconLabs/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/weatherstation.d" -MT"src/weatherstation.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


