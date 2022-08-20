################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C++_SRCS += \
../libconfig/libconfigcpp.c++ 

C_SRCS += \
../libconfig/grammar.c \
../libconfig/libconfig.c \
../libconfig/scanctx.c \
../libconfig/scanner.c \
../libconfig/strbuf.c 

C++_DEPS += \
./libconfig/libconfigcpp.d 

OBJS += \
./libconfig/grammar.o \
./libconfig/libconfig.o \
./libconfig/libconfigcpp.o \
./libconfig/scanctx.o \
./libconfig/scanner.o \
./libconfig/strbuf.o 

C_DEPS += \
./libconfig/grammar.d \
./libconfig/libconfig.d \
./libconfig/scanctx.d \
./libconfig/scanner.d \
./libconfig/strbuf.d 


# Each subdirectory must supply rules for building sources it contributes
libconfig/%.o: ../libconfig/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libconfig/%.o: ../libconfig/%.c++
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


