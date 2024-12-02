################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/shared/config/encode/EncodeVer0.cpp 

CPP_DEPS += \
./src/shared/config/encode/EncodeVer0.d 

OBJS += \
./src/shared/config/encode/EncodeVer0.o 


# Each subdirectory must supply rules for building sources it contributes
src/shared/config/encode/%.o: ../src/shared/config/encode/%.cpp src/shared/config/encode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I../src/shared -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-shared-2f-config-2f-encode

clean-src-2f-shared-2f-config-2f-encode:
	-$(RM) ./src/shared/config/encode/EncodeVer0.d ./src/shared/config/encode/EncodeVer0.o

.PHONY: clean-src-2f-shared-2f-config-2f-encode

