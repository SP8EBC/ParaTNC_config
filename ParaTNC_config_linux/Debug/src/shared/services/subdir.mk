################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/shared/services/IService.cpp \
../src/shared/services/SrvEraseStartupConfig.cpp \
../src/shared/services/SrvGetRunningConfig.cpp \
../src/shared/services/SrvGetVersionAndId.cpp \
../src/shared/services/SrvReadDid.cpp \
../src/shared/services/SrvReadDid_test.cpp \
../src/shared/services/SrvSendStartupConfig.cpp 

CPP_DEPS += \
./src/shared/services/IService.d \
./src/shared/services/SrvEraseStartupConfig.d \
./src/shared/services/SrvGetRunningConfig.d \
./src/shared/services/SrvGetVersionAndId.d \
./src/shared/services/SrvReadDid.d \
./src/shared/services/SrvReadDid_test.d \
./src/shared/services/SrvSendStartupConfig.d 

OBJS += \
./src/shared/services/IService.o \
./src/shared/services/SrvEraseStartupConfig.o \
./src/shared/services/SrvGetRunningConfig.o \
./src/shared/services/SrvGetVersionAndId.o \
./src/shared/services/SrvReadDid.o \
./src/shared/services/SrvReadDid_test.o \
./src/shared/services/SrvSendStartupConfig.o 


# Each subdirectory must supply rules for building sources it contributes
src/shared/services/%.o: ../src/shared/services/%.cpp src/shared/services/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I../src/shared -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-shared-2f-services

clean-src-2f-shared-2f-services:
	-$(RM) ./src/shared/services/IService.d ./src/shared/services/IService.o ./src/shared/services/SrvEraseStartupConfig.d ./src/shared/services/SrvEraseStartupConfig.o ./src/shared/services/SrvGetRunningConfig.d ./src/shared/services/SrvGetRunningConfig.o ./src/shared/services/SrvGetVersionAndId.d ./src/shared/services/SrvGetVersionAndId.o ./src/shared/services/SrvReadDid.d ./src/shared/services/SrvReadDid.o ./src/shared/services/SrvReadDid_test.d ./src/shared/services/SrvReadDid_test.o ./src/shared/services/SrvSendStartupConfig.d ./src/shared/services/SrvSendStartupConfig.o

.PHONY: clean-src-2f-shared-2f-services

