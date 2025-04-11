################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shared/config/decode/DecodeVer0.cpp \
../shared/config/decode/ValidateVer0.cpp 

CPP_DEPS += \
./shared/config/decode/DecodeVer0.d \
./shared/config/decode/ValidateVer0.d 

OBJS += \
./shared/config/decode/DecodeVer0.o \
./shared/config/decode/ValidateVer0.o 


# Each subdirectory must supply rules for building sources it contributes
shared/config/decode/%.o: ../shared/config/decode/%.cpp shared/config/decode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared-2f-config-2f-decode

clean-shared-2f-config-2f-decode:
	-$(RM) ./shared/config/decode/DecodeVer0.d ./shared/config/decode/DecodeVer0.o ./shared/config/decode/ValidateVer0.d ./shared/config/decode/ValidateVer0.o

.PHONY: clean-shared-2f-config-2f-decode

