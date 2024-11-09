################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AuxStuff.cpp \
../src/AuxStuff_test.cpp \
../src/LogDumper.cpp \
../src/LogDumper_test.cpp \
../src/ProgramConfig.cpp \
../src/main.cpp 

CPP_DEPS += \
./src/AuxStuff.d \
./src/AuxStuff_test.d \
./src/LogDumper.d \
./src/LogDumper_test.d \
./src/ProgramConfig.d \
./src/main.d 

OBJS += \
./src/AuxStuff.o \
./src/AuxStuff_test.o \
./src/LogDumper.o \
./src/LogDumper_test.o \
./src/ProgramConfig.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/AuxStuff.d ./src/AuxStuff.o ./src/AuxStuff_test.d ./src/AuxStuff_test.o ./src/LogDumper.d ./src/LogDumper.o ./src/LogDumper_test.d ./src/LogDumper_test.o ./src/ProgramConfig.d ./src/ProgramConfig.o ./src/main.d ./src/main.o

.PHONY: clean-src

