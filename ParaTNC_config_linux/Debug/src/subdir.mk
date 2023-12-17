################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AuxStuff.cpp \
../src/AuxStuff_test.cpp \
../src/ConfigEncodeDecode_test.cpp \
../src/ProgramConfig.cpp 

CPP_DEPS += \
./src/AuxStuff.d \
./src/AuxStuff_test.d \
./src/ConfigEncodeDecode_test.d \
./src/ProgramConfig.d 

OBJS += \
./src/AuxStuff.o \
./src/AuxStuff_test.o \
./src/ConfigEncodeDecode_test.o \
./src/ProgramConfig.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I../src/shared -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/AuxStuff.d ./src/AuxStuff.o ./src/AuxStuff_test.d ./src/AuxStuff_test.o ./src/ConfigEncodeDecode_test.d ./src/ConfigEncodeDecode_test.o ./src/ProgramConfig.d ./src/ProgramConfig.o

.PHONY: clean-src

