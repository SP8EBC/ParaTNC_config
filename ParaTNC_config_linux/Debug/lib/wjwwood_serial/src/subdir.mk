################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../lib/wjwwood_serial/src/serial.cc 

O_SRCS += \
../lib/wjwwood_serial/src/serial.o 

CC_DEPS += \
./lib/wjwwood_serial/src/serial.d 

OBJS += \
./lib/wjwwood_serial/src/serial.o 


# Each subdirectory must supply rules for building sources it contributes
lib/wjwwood_serial/src/%.o: ../lib/wjwwood_serial/src/%.cc lib/wjwwood_serial/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../shared -I../lib/ctable-master/src -I../src/ -I../lib/wjwwood_serial/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-wjwwood_serial-2f-src

clean-lib-2f-wjwwood_serial-2f-src:
	-$(RM) ./lib/wjwwood_serial/src/serial.d ./lib/wjwwood_serial/src/serial.o

.PHONY: clean-lib-2f-wjwwood_serial-2f-src

