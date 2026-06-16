################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DAC7578.c \
../Core/Src/NTC.c \
../Core/Src/OLED.c \
../Core/Src/TCA9555.c \
../Core/Src/alldata.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/json-maker.c \
../Core/Src/jsonParser.c \
../Core/Src/main.c \
../Core/Src/mcp4728.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/tim.c \
../Core/Src/tiny-json.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/DAC7578.o \
./Core/Src/NTC.o \
./Core/Src/OLED.o \
./Core/Src/TCA9555.o \
./Core/Src/alldata.o \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/json-maker.o \
./Core/Src/jsonParser.o \
./Core/Src/main.o \
./Core/Src/mcp4728.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/tim.o \
./Core/Src/tiny-json.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/DAC7578.d \
./Core/Src/NTC.d \
./Core/Src/OLED.d \
./Core/Src/TCA9555.d \
./Core/Src/alldata.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/json-maker.d \
./Core/Src/jsonParser.d \
./Core/Src/main.d \
./Core/Src/mcp4728.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/tim.d \
./Core/Src/tiny-json.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_32 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DAC7578.cyclo ./Core/Src/DAC7578.d ./Core/Src/DAC7578.o ./Core/Src/DAC7578.su ./Core/Src/NTC.cyclo ./Core/Src/NTC.d ./Core/Src/NTC.o ./Core/Src/NTC.su ./Core/Src/OLED.cyclo ./Core/Src/OLED.d ./Core/Src/OLED.o ./Core/Src/OLED.su ./Core/Src/TCA9555.cyclo ./Core/Src/TCA9555.d ./Core/Src/TCA9555.o ./Core/Src/TCA9555.su ./Core/Src/alldata.cyclo ./Core/Src/alldata.d ./Core/Src/alldata.o ./Core/Src/alldata.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/json-maker.cyclo ./Core/Src/json-maker.d ./Core/Src/json-maker.o ./Core/Src/json-maker.su ./Core/Src/jsonParser.cyclo ./Core/Src/jsonParser.d ./Core/Src/jsonParser.o ./Core/Src/jsonParser.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mcp4728.cyclo ./Core/Src/mcp4728.d ./Core/Src/mcp4728.o ./Core/Src/mcp4728.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/tiny-json.cyclo ./Core/Src/tiny-json.d ./Core/Src/tiny-json.o ./Core/Src/tiny-json.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

