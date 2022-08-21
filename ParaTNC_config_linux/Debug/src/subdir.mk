################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ProgramConfig.cpp \
../src/main.cpp \
../src/serial.cpp 

OBJS += \
./src/ProgramConfig.o \
./src/main.o \
./src/serial.o 

CPP_DEPS += \
./src/ProgramConfig.d \
./src/main.d \
./src/serial.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++2a -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


