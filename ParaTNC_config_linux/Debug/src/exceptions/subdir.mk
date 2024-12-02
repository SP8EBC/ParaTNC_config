################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/exceptions/NotSupportedEx.cpp \
../src/exceptions/TimeoutE.cpp \
../src/exceptions/TimeoutE_test.cpp \
../src/exceptions/TransmissionFailedEx.cpp 

CPP_DEPS += \
./src/exceptions/NotSupportedEx.d \
./src/exceptions/TimeoutE.d \
./src/exceptions/TimeoutE_test.d \
./src/exceptions/TransmissionFailedEx.d 

OBJS += \
./src/exceptions/NotSupportedEx.o \
./src/exceptions/TimeoutE.o \
./src/exceptions/TimeoutE_test.o \
./src/exceptions/TransmissionFailedEx.o 


# Each subdirectory must supply rules for building sources it contributes
src/exceptions/%.o: ../src/exceptions/%.cpp src/exceptions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-exceptions

clean-src-2f-exceptions:
	-$(RM) ./src/exceptions/NotSupportedEx.d ./src/exceptions/NotSupportedEx.o ./src/exceptions/TimeoutE.d ./src/exceptions/TimeoutE.o ./src/exceptions/TimeoutE_test.d ./src/exceptions/TimeoutE_test.o ./src/exceptions/TransmissionFailedEx.d ./src/exceptions/TransmissionFailedEx.o

.PHONY: clean-src-2f-exceptions

