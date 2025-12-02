################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../shared/event_log.c 

C_DEPS += \
./shared/event_log.d 

OBJS += \
./shared/event_log.o 


# Each subdirectory must supply rules for building sources it contributes
shared/%.o: ../shared/%.c shared/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../src/ -I../src/shared -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-shared

clean-shared:
	-$(RM) ./shared/event_log.d ./shared/event_log.o

.PHONY: clean-shared

