################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/shared/crc/crc_.c 

C_DEPS += \
./src/shared/crc/crc_.d 

OBJS += \
./src/shared/crc/crc_.o 


# Each subdirectory must supply rules for building sources it contributes
src/shared/crc/%.o: ../src/shared/crc/%.c src/shared/crc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-shared-2f-crc

clean-src-2f-shared-2f-crc:
	-$(RM) ./src/shared/crc/crc_.d ./src/shared/crc/crc_.o

.PHONY: clean-src-2f-shared-2f-crc

