################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/ctable-master/src/string_builder.c \
../lib/ctable-master/src/string_util.c \
../lib/ctable-master/src/table.c \
../lib/ctable-master/src/vector.c 

C_DEPS += \
./lib/ctable-master/src/string_builder.d \
./lib/ctable-master/src/string_util.d \
./lib/ctable-master/src/table.d \
./lib/ctable-master/src/vector.d 

OBJS += \
./lib/ctable-master/src/string_builder.o \
./lib/ctable-master/src/string_util.o \
./lib/ctable-master/src/table.o \
./lib/ctable-master/src/vector.o 


# Each subdirectory must supply rules for building sources it contributes
lib/ctable-master/src/%.o: ../lib/ctable-master/src/%.c lib/ctable-master/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_XOPEN_SOURCE=600 -I../src/ -I../lib/ctable-master/src -I../src/shared -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-ctable-2d-master-2f-src

clean-lib-2f-ctable-2d-master-2f-src:
	-$(RM) ./lib/ctable-master/src/string_builder.d ./lib/ctable-master/src/string_builder.o ./lib/ctable-master/src/string_util.d ./lib/ctable-master/src/string_util.o ./lib/ctable-master/src/table.d ./lib/ctable-master/src/table.o ./lib/ctable-master/src/vector.d ./lib/ctable-master/src/vector.o

.PHONY: clean-lib-2f-ctable-2d-master-2f-src

