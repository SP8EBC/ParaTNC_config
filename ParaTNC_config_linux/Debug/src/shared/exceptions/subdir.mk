################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/shared/exceptions/NotSupportedEx.cpp \
../src/shared/exceptions/TimeoutE.cpp \
../src/shared/exceptions/TimeoutE_test.cpp \
../src/shared/exceptions/TransmissionFailedEx.cpp 

CPP_DEPS += \
./src/shared/exceptions/NotSupportedEx.d \
./src/shared/exceptions/TimeoutE.d \
./src/shared/exceptions/TimeoutE_test.d \
./src/shared/exceptions/TransmissionFailedEx.d 

OBJS += \
./src/shared/exceptions/NotSupportedEx.o \
./src/shared/exceptions/TimeoutE.o \
./src/shared/exceptions/TimeoutE_test.o \
./src/shared/exceptions/TransmissionFailedEx.o 


# Each subdirectory must supply rules for building sources it contributes
src/shared/exceptions/%.o: ../src/shared/exceptions/%.cpp src/shared/exceptions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I../src/shared -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-shared-2f-exceptions

clean-src-2f-shared-2f-exceptions:
	-$(RM) ./src/shared/exceptions/NotSupportedEx.d ./src/shared/exceptions/NotSupportedEx.o ./src/shared/exceptions/TimeoutE.d ./src/shared/exceptions/TimeoutE.o ./src/shared/exceptions/TimeoutE_test.d ./src/shared/exceptions/TimeoutE_test.o ./src/shared/exceptions/TransmissionFailedEx.d ./src/shared/exceptions/TransmissionFailedEx.o

.PHONY: clean-src-2f-shared-2f-exceptions

