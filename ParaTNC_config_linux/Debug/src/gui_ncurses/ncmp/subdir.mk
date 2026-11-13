################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gui_ncurses/ncmp/Menu.cpp \
../src/gui_ncurses/ncmp/Panel.cpp \
../src/gui_ncurses/ncmp/Window.cpp 

CPP_DEPS += \
./src/gui_ncurses/ncmp/Menu.d \
./src/gui_ncurses/ncmp/Panel.d \
./src/gui_ncurses/ncmp/Window.d 

OBJS += \
./src/gui_ncurses/ncmp/Menu.o \
./src/gui_ncurses/ncmp/Panel.o \
./src/gui_ncurses/ncmp/Window.o 


# Each subdirectory must supply rules for building sources it contributes
src/gui_ncurses/ncmp/%.o: ../src/gui_ncurses/ncmp/%.cpp src/gui_ncurses/ncmp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++20 -D_XOPEN_SOURCE=600 -I../shared -I../ -I../lib/nlohman_json_3_7_1/include -I../lib/ctable-master/src -I../src/ -I../lib/wjwwood_serial/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-gui_ncurses-2f-ncmp

clean-src-2f-gui_ncurses-2f-ncmp:
	-$(RM) ./src/gui_ncurses/ncmp/Menu.d ./src/gui_ncurses/ncmp/Menu.o ./src/gui_ncurses/ncmp/Panel.d ./src/gui_ncurses/ncmp/Panel.o ./src/gui_ncurses/ncmp/Window.d ./src/gui_ncurses/ncmp/Window.o

.PHONY: clean-src-2f-gui_ncurses-2f-ncmp

