################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../lib/wjwwood_serial/src/impl/list_ports/list_ports_linux.cc \
../lib/wjwwood_serial/src/impl/list_ports/list_ports_osx.cc \
../lib/wjwwood_serial/src/impl/list_ports/list_ports_win.cc 

O_SRCS += \
../lib/wjwwood_serial/src/impl/list_ports/list_ports_linux.o 

CC_DEPS += \
./lib/wjwwood_serial/src/impl/list_ports/list_ports_linux.d \
./lib/wjwwood_serial/src/impl/list_ports/list_ports_osx.d \
./lib/wjwwood_serial/src/impl/list_ports/list_ports_win.d 

OBJS += \
./lib/wjwwood_serial/src/impl/list_ports/list_ports_linux.o \
./lib/wjwwood_serial/src/impl/list_ports/list_ports_osx.o \
./lib/wjwwood_serial/src/impl/list_ports/list_ports_win.o 


# Each subdirectory must supply rules for building sources it contributes
lib/wjwwood_serial/src/impl/list_ports/%.o: ../lib/wjwwood_serial/src/impl/list_ports/%.cc lib/wjwwood_serial/src/impl/list_ports/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -D_XOPEN_SOURCE=600 -I../shared -I../lib/ctable-master/src -I../src/ -I../lib/wjwwood_serial/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-wjwwood_serial-2f-src-2f-impl-2f-list_ports

clean-lib-2f-wjwwood_serial-2f-src-2f-impl-2f-list_ports:
	-$(RM) ./lib/wjwwood_serial/src/impl/list_ports/list_ports_linux.d ./lib/wjwwood_serial/src/impl/list_ports/list_ports_linux.o ./lib/wjwwood_serial/src/impl/list_ports/list_ports_osx.d ./lib/wjwwood_serial/src/impl/list_ports/list_ports_osx.o ./lib/wjwwood_serial/src/impl/list_ports/list_ports_win.d ./lib/wjwwood_serial/src/impl/list_ports/list_ports_win.o

.PHONY: clean-lib-2f-wjwwood_serial-2f-src-2f-impl-2f-list_ports

