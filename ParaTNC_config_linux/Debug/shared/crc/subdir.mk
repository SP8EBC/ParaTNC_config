################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../shared/crc/crc_.c 

C_DEPS += \
./shared/crc/crc_.d 

OBJS += \
./shared/crc/crc_.o 


# Each subdirectory must supply rules for building sources it contributes
shared/crc/%.o: ../shared/crc/%.c shared/crc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_XOPEN_SOURCE=600 -I../src/ -I../src/ctable-master/src -I../src/shared -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared-2f-crc

clean-shared-2f-crc:
	-$(RM) ./shared/crc/crc_.d ./shared/crc/crc_.o

.PHONY: clean-shared-2f-crc

