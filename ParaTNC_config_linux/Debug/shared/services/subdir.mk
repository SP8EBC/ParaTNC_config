################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shared/services/IService.cpp \
../shared/services/SrvEraseStartupConfig.cpp \
../shared/services/SrvGetRunningConfig.cpp \
../shared/services/SrvGetVersionAndId.cpp \
../shared/services/SrvReadDid.cpp \
../shared/services/SrvReadDid_test.cpp \
../shared/services/SrvReadMemory.cpp \
../shared/services/SrvReadMemory_test.cpp \
../shared/services/SrvReset.cpp \
../shared/services/SrvSendStartupConfig.cpp 

CPP_DEPS += \
./shared/services/IService.d \
./shared/services/SrvEraseStartupConfig.d \
./shared/services/SrvGetRunningConfig.d \
./shared/services/SrvGetVersionAndId.d \
./shared/services/SrvReadDid.d \
./shared/services/SrvReadDid_test.d \
./shared/services/SrvReadMemory.d \
./shared/services/SrvReadMemory_test.d \
./shared/services/SrvReset.d \
./shared/services/SrvSendStartupConfig.d 

OBJS += \
./shared/services/IService.o \
./shared/services/SrvEraseStartupConfig.o \
./shared/services/SrvGetRunningConfig.o \
./shared/services/SrvGetVersionAndId.o \
./shared/services/SrvReadDid.o \
./shared/services/SrvReadDid_test.o \
./shared/services/SrvReadMemory.o \
./shared/services/SrvReadMemory_test.o \
./shared/services/SrvReset.o \
./shared/services/SrvSendStartupConfig.o 


# Each subdirectory must supply rules for building sources it contributes
shared/services/%.o: ../shared/services/%.cpp shared/services/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared-2f-services

clean-shared-2f-services:
	-$(RM) ./shared/services/IService.d ./shared/services/IService.o ./shared/services/SrvEraseStartupConfig.d ./shared/services/SrvEraseStartupConfig.o ./shared/services/SrvGetRunningConfig.d ./shared/services/SrvGetRunningConfig.o ./shared/services/SrvGetVersionAndId.d ./shared/services/SrvGetVersionAndId.o ./shared/services/SrvReadDid.d ./shared/services/SrvReadDid.o ./shared/services/SrvReadDid_test.d ./shared/services/SrvReadDid_test.o ./shared/services/SrvReadMemory.d ./shared/services/SrvReadMemory.o ./shared/services/SrvReadMemory_test.d ./shared/services/SrvReadMemory_test.o ./shared/services/SrvReset.d ./shared/services/SrvReset.o ./shared/services/SrvSendStartupConfig.d ./shared/services/SrvSendStartupConfig.o

.PHONY: clean-shared-2f-services

