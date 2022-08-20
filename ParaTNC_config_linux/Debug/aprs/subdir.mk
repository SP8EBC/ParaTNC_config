################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../aprs/Call.cpp \
../aprs/Mappings.cpp \
../aprs/PacketFoundation.cpp \
../aprs/PacketWx.cpp 

OBJS += \
./aprs/Call.o \
./aprs/Mappings.o \
./aprs/PacketFoundation.o \
./aprs/PacketWx.o 

CPP_DEPS += \
./aprs/Call.d \
./aprs/Mappings.d \
./aprs/PacketFoundation.d \
./aprs/PacketWx.d 


# Each subdirectory must supply rules for building sources it contributes
aprs/%.o: ../aprs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../aprs -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


