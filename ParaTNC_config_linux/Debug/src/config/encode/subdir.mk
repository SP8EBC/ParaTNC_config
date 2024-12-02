################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/config/encode/EncodeVer0.cpp 

CPP_DEPS += \
./src/config/encode/EncodeVer0.d 

OBJS += \
./src/config/encode/EncodeVer0.o 


# Each subdirectory must supply rules for building sources it contributes
src/config/encode/%.o: ../src/config/encode/%.cpp src/config/encode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-config-2f-encode

clean-src-2f-config-2f-encode:
	-$(RM) ./src/config/encode/EncodeVer0.d ./src/config/encode/EncodeVer0.o

.PHONY: clean-src-2f-config-2f-encode

