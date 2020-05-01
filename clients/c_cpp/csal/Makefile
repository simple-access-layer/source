CC=gcc
CXX=g++

BUILD_DIR ?= ./build
SRC_DIRS ?= .

SRCS := csal_attrib.cpp csal_node.cpp

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) 
DEPS := $(OBJS:.o=.d)

#INC_FLAGS := -I. -I../include

CFLAGS = -g -Wall -I. -I./include -I../include -I$(HOME)/local/include
CPPFLAGS = -std=c++11 -fPIC
LDFLAGS = -L$(HOME)/local/lib -L../ -L./build -lPocoFoundation -lPocoJSON -lPocoNet -lPocoNetSSL


#all:libcsal
all:$(BUILD_DIR)/libcsal  test

$(BUILD_DIR)/libcsal: $(OBJS) 
	@$(MKDIR_P) $(BUILD_DIR)
	$(CXX) -g -shared -fPIC $(OBJS) -o $@.so $(LDFLAGS) 

# c source
$(BUILD_DIR)/%.c.o: %.c 
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# cpp source
$(BUILD_DIR)/%.cpp.o: %.cpp 
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test:
	gcc -g ./test/main.c $(CFLAGS) $(LDFLAGS) -lcsal -o $(BUILD_DIR)/test 

.PHONY: clean test 
	test: 
	echo $(OS) 
clean: 
	 $(RM) -r $(BUILD_DIR) 

-include $(DEPS)

MKDIR_P ?= mkdir -p
