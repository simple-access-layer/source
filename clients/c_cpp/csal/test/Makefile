CC=gcc
CXX=g++

BUILD_DIR ?= ./build
SRC_DIRS ?= .

SRCS := main.c

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) 
DEPS := $(OBJS:.o=.d)

INC_FLAGS := -I. -I../include

CFLAGS = -g -Wall -I. -I.. -I$(HOME)/local/include
CPPFLAGS = -std=c++11 -fPIC
LDFLAGS = -L../build -lcsal -L../../ 


#all:libcsal
all:$(BUILD_DIR)/test

$(BUILD_DIR)/test: $(OBJS) 
	@$(MKDIR_P) $(BUILD_DIR)
	$(CXX) -g -fPIC $(OBJS) -o $@ $(LDFLAGS) 

# c source
$(BUILD_DIR)/%.c.o: %.c 
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# cpp source
$(BUILD_DIR)/%.cpp.o: %.cpp 
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean test 
	test: 
	echo $(OS) 
clean: 
	 $(RM) -r $(BUILD_DIR) 

-include $(DEPS)

MKDIR_P ?= mkdir -p
