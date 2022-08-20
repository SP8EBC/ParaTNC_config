################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../types/ChannelValueFoundation.cpp \
../types/DeviceChannel.cpp \
../types/Float.cpp \
../types/Integer.cpp \
../types/OutputValues.cpp 

OBJS += \
./types/ChannelValueFoundation.o \
./types/DeviceChannel.o \
./types/Float.o \
./types/Integer.o \
./types/OutputValues.o 

CPP_DEPS += \
./types/ChannelValueFoundation.d \
./types/DeviceChannel.d \
./types/Float.d \
./types/Integer.d \
./types/OutputValues.d 


# Each subdirectory must supply rules for building sources it contributes
types/%.o: ../types/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


