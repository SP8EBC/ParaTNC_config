################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/did_decoder/DescriptionIniFileReader.cpp \
../src/did_decoder/DidDecoder.cpp 

CPP_DEPS += \
./src/did_decoder/DescriptionIniFileReader.d \
./src/did_decoder/DidDecoder.d 

OBJS += \
./src/did_decoder/DescriptionIniFileReader.o \
./src/did_decoder/DidDecoder.o 


# Each subdirectory must supply rules for building sources it contributes
src/did_decoder/%.o: ../src/did_decoder/%.cpp src/did_decoder/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../shared -I../lib/ctable-master/src -I../src/ -I../lib/wjwwood_serial/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-did_decoder

clean-src-2f-did_decoder:
	-$(RM) ./src/did_decoder/DescriptionIniFileReader.d ./src/did_decoder/DescriptionIniFileReader.o ./src/did_decoder/DidDecoder.d ./src/did_decoder/DidDecoder.o

.PHONY: clean-src-2f-did_decoder

