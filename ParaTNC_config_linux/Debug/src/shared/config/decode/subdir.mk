################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/shared/config/decode/DecodeVer0.cpp \
../src/shared/config/decode/ValidateVer0.cpp 

CPP_DEPS += \
./src/shared/config/decode/DecodeVer0.d \
./src/shared/config/decode/ValidateVer0.d 

OBJS += \
./src/shared/config/decode/DecodeVer0.o \
./src/shared/config/decode/ValidateVer0.o 


# Each subdirectory must supply rules for building sources it contributes
src/shared/config/decode/%.o: ../src/shared/config/decode/%.cpp src/shared/config/decode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I../src/shared -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-shared-2f-config-2f-decode

clean-src-2f-shared-2f-config-2f-decode:
	-$(RM) ./src/shared/config/decode/DecodeVer0.d ./src/shared/config/decode/DecodeVer0.o ./src/shared/config/decode/ValidateVer0.d ./src/shared/config/decode/ValidateVer0.o

.PHONY: clean-src-2f-shared-2f-config-2f-decode

