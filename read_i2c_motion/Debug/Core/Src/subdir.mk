################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/OLED.c \
../Core/Src/lis3dh_free_fall.c \
../Core/Src/lis3dh_multi_read_fifo.c \
../Core/Src/lis3dh_orientation.c \
../Core/Src/lis3dh_read_data_polling.c \
../Core/Src/lis3dh_reg.c \
../Core/Src/lis3dh_self_test.c \
../Core/Src/lis3dh_tap_double.c \
../Core/Src/lis3dh_tap_single.c \
../Core/Src/lis3dh_wake_up.c \
../Core/Src/main.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c 

OBJS += \
./Core/Src/OLED.o \
./Core/Src/lis3dh_free_fall.o \
./Core/Src/lis3dh_multi_read_fifo.o \
./Core/Src/lis3dh_orientation.o \
./Core/Src/lis3dh_read_data_polling.o \
./Core/Src/lis3dh_reg.o \
./Core/Src/lis3dh_self_test.o \
./Core/Src/lis3dh_tap_double.o \
./Core/Src/lis3dh_tap_single.o \
./Core/Src/lis3dh_wake_up.o \
./Core/Src/main.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o 

C_DEPS += \
./Core/Src/OLED.d \
./Core/Src/lis3dh_free_fall.d \
./Core/Src/lis3dh_multi_read_fifo.d \
./Core/Src/lis3dh_orientation.d \
./Core/Src/lis3dh_read_data_polling.d \
./Core/Src/lis3dh_reg.d \
./Core/Src/lis3dh_self_test.d \
./Core/Src/lis3dh_tap_double.d \
./Core/Src/lis3dh_tap_single.d \
./Core/Src/lis3dh_wake_up.d \
./Core/Src/main.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_32 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/OLED.cyclo ./Core/Src/OLED.d ./Core/Src/OLED.o ./Core/Src/OLED.su ./Core/Src/lis3dh_free_fall.cyclo ./Core/Src/lis3dh_free_fall.d ./Core/Src/lis3dh_free_fall.o ./Core/Src/lis3dh_free_fall.su ./Core/Src/lis3dh_multi_read_fifo.cyclo ./Core/Src/lis3dh_multi_read_fifo.d ./Core/Src/lis3dh_multi_read_fifo.o ./Core/Src/lis3dh_multi_read_fifo.su ./Core/Src/lis3dh_orientation.cyclo ./Core/Src/lis3dh_orientation.d ./Core/Src/lis3dh_orientation.o ./Core/Src/lis3dh_orientation.su ./Core/Src/lis3dh_read_data_polling.cyclo ./Core/Src/lis3dh_read_data_polling.d ./Core/Src/lis3dh_read_data_polling.o ./Core/Src/lis3dh_read_data_polling.su ./Core/Src/lis3dh_reg.cyclo ./Core/Src/lis3dh_reg.d ./Core/Src/lis3dh_reg.o ./Core/Src/lis3dh_reg.su ./Core/Src/lis3dh_self_test.cyclo ./Core/Src/lis3dh_self_test.d ./Core/Src/lis3dh_self_test.o ./Core/Src/lis3dh_self_test.su ./Core/Src/lis3dh_tap_double.cyclo ./Core/Src/lis3dh_tap_double.d ./Core/Src/lis3dh_tap_double.o ./Core/Src/lis3dh_tap_double.su ./Core/Src/lis3dh_tap_single.cyclo ./Core/Src/lis3dh_tap_single.d ./Core/Src/lis3dh_tap_single.o ./Core/Src/lis3dh_tap_single.su ./Core/Src/lis3dh_wake_up.cyclo ./Core/Src/lis3dh_wake_up.d ./Core/Src/lis3dh_wake_up.o ./Core/Src/lis3dh_wake_up.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su

.PHONY: clean-Core-2f-Src

