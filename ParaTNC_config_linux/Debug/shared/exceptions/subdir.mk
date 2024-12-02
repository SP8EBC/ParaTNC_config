################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shared/exceptions/NotSupportedEx.cpp \
../shared/exceptions/TimeoutE.cpp \
../shared/exceptions/TransmissionFailedEx.cpp 

CPP_DEPS += \
./shared/exceptions/NotSupportedEx.d \
./shared/exceptions/TimeoutE.d \
./shared/exceptions/TransmissionFailedEx.d 

OBJS += \
./shared/exceptions/NotSupportedEx.o \
./shared/exceptions/TimeoutE.o \
./shared/exceptions/TransmissionFailedEx.o 


# Each subdirectory must supply rules for building sources it contributes
shared/exceptions/%.o: ../shared/exceptions/%.cpp shared/exceptions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared-2f-exceptions

clean-shared-2f-exceptions:
	-$(RM) ./shared/exceptions/NotSupportedEx.d ./shared/exceptions/NotSupportedEx.o ./shared/exceptions/TimeoutE.d ./shared/exceptions/TimeoutE.o ./shared/exceptions/TransmissionFailedEx.d ./shared/exceptions/TransmissionFailedEx.o

.PHONY: clean-shared-2f-exceptions

