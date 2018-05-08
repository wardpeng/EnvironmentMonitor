################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\libraries\Wire\src\utility\twi.c 

C_DEPS += \
.\libraries\Wire\src\utility\twi.c.d 

LINK_OBJ += \
.\libraries\Wire\src\utility\twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries\Wire\src\utility\twi.c.o: C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\libraries\Wire\src\utility\twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_ADK -DARDUINO_ARCH_AVR   -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\cores\arduino" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\variants\mega" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\libraries\SD\1.2.2" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\libraries\SD\1.2.2\src" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\libraries\DS3231\1.0.2" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\libraries\RTClib\1.2.0" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\libraries\SPI" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\libraries\SPI\src" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\libraries\Wire" -I"C:\Users\I327500\MyWork_PXQ\software\java\eclipse_neon_4_SCP\arduinoPlugin\packages\arduino\hardware\avr\1.6.21\libraries\Wire\src" -I"C:\Users\I327500\MyWork_PXQ\other project\arduino20150615\libraries\DS18B20_S" -I"C:\Users\I327500\MyWork_PXQ\other project\arduino20150615\libraries\LiquidCrystal_I2C" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


