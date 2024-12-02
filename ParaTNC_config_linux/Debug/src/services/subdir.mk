################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/services/IService.cpp \
../src/services/SrvEraseStartupConfig.cpp \
../src/services/SrvGetRunningConfig.cpp \
../src/services/SrvGetVersionAndId.cpp \
../src/services/SrvReadDid.cpp \
../src/services/SrvReadDid_test.cpp \
../src/services/SrvSendStartupConfig.cpp 

CPP_DEPS += \
./src/services/IService.d \
./src/services/SrvEraseStartupConfig.d \
./src/services/SrvGetRunningConfig.d \
./src/services/SrvGetVersionAndId.d \
./src/services/SrvReadDid.d \
./src/services/SrvReadDid_test.d \
./src/services/SrvSendStartupConfig.d 

OBJS += \
./src/services/IService.o \
./src/services/SrvEraseStartupConfig.o \
./src/services/SrvGetRunningConfig.o \
./src/services/SrvGetVersionAndId.o \
./src/services/SrvReadDid.o \
./src/services/SrvReadDid_test.o \
./src/services/SrvSendStartupConfig.o 


# Each subdirectory must supply rules for building sources it contributes
src/services/%.o: ../src/services/%.cpp src/services/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-services

clean-src-2f-services:
	-$(RM) ./src/services/IService.d ./src/services/IService.o ./src/services/SrvEraseStartupConfig.d ./src/services/SrvEraseStartupConfig.o ./src/services/SrvGetRunningConfig.d ./src/services/SrvGetRunningConfig.o ./src/services/SrvGetVersionAndId.d ./src/services/SrvGetVersionAndId.o ./src/services/SrvReadDid.d ./src/services/SrvReadDid.o ./src/services/SrvReadDid_test.d ./src/services/SrvReadDid_test.o ./src/services/SrvSendStartupConfig.d ./src/services/SrvSendStartupConfig.o

.PHONY: clean-src-2f-services

