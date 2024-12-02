################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ctable-master/src/string_builder.c \
../src/ctable-master/src/string_util.c \
../src/ctable-master/src/table.c \
../src/ctable-master/src/vector.c 

C_DEPS += \
./src/ctable-master/src/string_builder.d \
./src/ctable-master/src/string_util.d \
./src/ctable-master/src/table.d \
./src/ctable-master/src/vector.d 

OBJS += \
./src/ctable-master/src/string_builder.o \
./src/ctable-master/src/string_util.o \
./src/ctable-master/src/table.o \
./src/ctable-master/src/vector.o 


# Each subdirectory must supply rules for building sources it contributes
src/ctable-master/src/%.o: ../src/ctable-master/src/%.c src/ctable-master/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_XOPEN_SOURCE=600 -I../src/ -I../src/ctable-master/src -I../src/shared -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-ctable-2d-master-2f-src

clean-src-2f-ctable-2d-master-2f-src:
	-$(RM) ./src/ctable-master/src/string_builder.d ./src/ctable-master/src/string_builder.o ./src/ctable-master/src/string_util.d ./src/ctable-master/src/string_util.o ./src/ctable-master/src/table.d ./src/ctable-master/src/table.o ./src/ctable-master/src/vector.d ./src/ctable-master/src/vector.o

.PHONY: clean-src-2f-ctable-2d-master-2f-src

