################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../umb/Routine23AnswerCbk.cpp \
../umb/Routine23Query.cpp \
../umb/Routine26AnswerCbk.cpp \
../umb/Routine26Query.cpp \
../umb/UmbHandling.cpp \
../umb/UmbThread.cpp 

OBJS += \
./umb/Routine23AnswerCbk.o \
./umb/Routine23Query.o \
./umb/Routine26AnswerCbk.o \
./umb/Routine26Query.o \
./umb/UmbHandling.o \
./umb/UmbThread.o 

CPP_DEPS += \
./umb/Routine23AnswerCbk.d \
./umb/Routine23Query.d \
./umb/Routine26AnswerCbk.d \
./umb/Routine26Query.d \
./umb/UmbHandling.d \
./umb/UmbThread.d 


# Each subdirectory must supply rules for building sources it contributes
umb/%.o: ../umb/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


