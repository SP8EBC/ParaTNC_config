################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../exceptions/NullPointerE.cpp \
../exceptions/NullPointerE_test.cpp \
../exceptions/StartOfHeaderTimeoutEx.cpp \
../exceptions/StartOfHeaderTimeoutEx_test.cpp \
../exceptions/TimeoutE.cpp \
../exceptions/TimeoutE_test.cpp \
../exceptions/UsageParseError.cpp 

OBJS += \
./exceptions/NullPointerE.o \
./exceptions/NullPointerE_test.o \
./exceptions/StartOfHeaderTimeoutEx.o \
./exceptions/StartOfHeaderTimeoutEx_test.o \
./exceptions/TimeoutE.o \
./exceptions/TimeoutE_test.o \
./exceptions/UsageParseError.o 

CPP_DEPS += \
./exceptions/NullPointerE.d \
./exceptions/NullPointerE_test.d \
./exceptions/StartOfHeaderTimeoutEx.d \
./exceptions/StartOfHeaderTimeoutEx_test.d \
./exceptions/TimeoutE.d \
./exceptions/TimeoutE_test.d \
./exceptions/UsageParseError.d 


# Each subdirectory must supply rules for building sources it contributes
exceptions/%.o: ../exceptions/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


