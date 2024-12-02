################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/serial/Serial.cpp \
../src/serial/SerialRxBackgroundWorker.cpp 

CPP_DEPS += \
./src/serial/Serial.d \
./src/serial/SerialRxBackgroundWorker.d 

OBJS += \
./src/serial/Serial.o \
./src/serial/SerialRxBackgroundWorker.o 


# Each subdirectory must supply rules for building sources it contributes
src/serial/%.o: ../src/serial/%.cpp src/serial/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-serial

clean-src-2f-serial:
	-$(RM) ./src/serial/Serial.d ./src/serial/Serial.o ./src/serial/SerialRxBackgroundWorker.d ./src/serial/SerialRxBackgroundWorker.o

.PHONY: clean-src-2f-serial

