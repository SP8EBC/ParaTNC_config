################################################################################
# Builds and runs unit tests for ConfigVer0_specific.h
#
#   make        - build unit test executable
#   make test   - build and run unit tests
#   make clean  - remove build artifacts
################################################################################

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O0 -g3 -Wall -Wextra -fmessage-length=0
CPPFLAGS += -I. -I.. -MMD -MP
LDFLAGS  += -L/usr/lib/x86_64-linux-gnu
LDLIBS   += -lboost_unit_test_framework

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/config_specific_test

SRCS := ConfigVer0_specific_test.cpp

OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

test: $(TARGET)
	./$(TARGET) --log_level=test_suite --report_level=short

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

clean:
	-$(RM) -r $(BUILD_DIR)

-include $(DEPS)

.PHONY: all test clean
