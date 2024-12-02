################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/config/decode/DecodeVer0.cpp \
../src/config/decode/ValidateVer0.cpp 

CPP_DEPS += \
./src/config/decode/DecodeVer0.d \
./src/config/decode/ValidateVer0.d 

OBJS += \
./src/config/decode/DecodeVer0.o \
./src/config/decode/ValidateVer0.o 


# Each subdirectory must supply rules for building sources it contributes
src/config/decode/%.o: ../src/config/decode/%.cpp src/config/decode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-config-2f-decode

clean-src-2f-config-2f-decode:
	-$(RM) ./src/config/decode/DecodeVer0.d ./src/config/decode/DecodeVer0.o ./src/config/decode/ValidateVer0.d ./src/config/decode/ValidateVer0.o

.PHONY: clean-src-2f-config-2f-decode

