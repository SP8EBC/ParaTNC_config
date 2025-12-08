################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shared/config/ConfigVer0.cpp \
../shared/config/ConfigVer0_test.cpp \
../shared/config/IConfig.cpp \
../shared/config/ValidateVer0.cpp 

CPP_DEPS += \
./shared/config/ConfigVer0.d \
./shared/config/ConfigVer0_test.d \
./shared/config/IConfig.d \
./shared/config/ValidateVer0.d 

OBJS += \
./shared/config/ConfigVer0.o \
./shared/config/ConfigVer0_test.o \
./shared/config/IConfig.o \
./shared/config/ValidateVer0.o 


# Each subdirectory must supply rules for building sources it contributes
shared/config/%.o: ../shared/config/%.cpp shared/config/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared-2f-config

clean-shared-2f-config:
	-$(RM) ./shared/config/ConfigVer0.d ./shared/config/ConfigVer0.o ./shared/config/ConfigVer0_test.d ./shared/config/ConfigVer0_test.o ./shared/config/IConfig.d ./shared/config/IConfig.o ./shared/config/ValidateVer0.d ./shared/config/ValidateVer0.o

.PHONY: clean-shared-2f-config

