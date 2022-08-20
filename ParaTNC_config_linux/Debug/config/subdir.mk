################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../config/ProgramConfig.cpp \
../config/UmbDevice.cpp 

OBJS += \
./config/ProgramConfig.o \
./config/UmbDevice.o 

CPP_DEPS += \
./config/ProgramConfig.d \
./config/UmbDevice.d 


# Each subdirectory must supply rules for building sources it contributes
config/%.o: ../config/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


