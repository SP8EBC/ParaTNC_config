################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shared/config/encode/EncodeVer0.cpp 

CPP_DEPS += \
./shared/config/encode/EncodeVer0.d 

OBJS += \
./shared/config/encode/EncodeVer0.o 


# Each subdirectory must supply rules for building sources it contributes
shared/config/encode/%.o: ../shared/config/encode/%.cpp shared/config/encode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared-2f-config-2f-encode

clean-shared-2f-config-2f-encode:
	-$(RM) ./shared/config/encode/EncodeVer0.d ./shared/config/encode/EncodeVer0.o

.PHONY: clean-shared-2f-config-2f-encode

