################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AuxStuff.cpp \
../src/AuxStuff_test.cpp \
../src/ConfigExporter.cpp \
../src/ConfigImporter.cpp \
../src/LogDumper.cpp \
../src/LogDumperTextFile.cpp \
../src/LogDumper_test.cpp \
../src/ProgramConfig.cpp \
../src/TimeTools.cpp \
../src/main.cpp \
../src/mainAuxFunctions.cpp 

CPP_DEPS += \
./src/AuxStuff.d \
./src/AuxStuff_test.d \
./src/ConfigExporter.d \
./src/ConfigImporter.d \
./src/LogDumper.d \
./src/LogDumperTextFile.d \
./src/LogDumper_test.d \
./src/ProgramConfig.d \
./src/TimeTools.d \
./src/main.d \
./src/mainAuxFunctions.d 

OBJS += \
./src/AuxStuff.o \
./src/AuxStuff_test.o \
./src/ConfigExporter.o \
./src/ConfigImporter.o \
./src/LogDumper.o \
./src/LogDumperTextFile.o \
./src/LogDumper_test.o \
./src/ProgramConfig.o \
./src/TimeTools.o \
./src/main.o \
./src/mainAuxFunctions.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/TimeTools.o: ../src/TimeTools.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -U_XOPEN_SOURCE -I../src/shared -I../src/ctable-master/src -I../src/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/AuxStuff.d ./src/AuxStuff.o ./src/AuxStuff_test.d ./src/AuxStuff_test.o ./src/ConfigExporter.d ./src/ConfigExporter.o ./src/ConfigImporter.d ./src/ConfigImporter.o ./src/LogDumper.d ./src/LogDumper.o ./src/LogDumperTextFile.d ./src/LogDumperTextFile.o ./src/LogDumper_test.d ./src/LogDumper_test.o ./src/ProgramConfig.d ./src/ProgramConfig.o ./src/TimeTools.d ./src/TimeTools.o ./src/main.d ./src/main.o ./src/mainAuxFunctions.d ./src/mainAuxFunctions.o

.PHONY: clean-src

