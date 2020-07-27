SRC_DIR     = ../src
INC_DIR     = ../inc
DUT_DIR			= ../../
TARGET_DIR  = ../bin
OBJ_DIR     = ../bin/obj
TARGET      = testout

TEST_TARGET = out
TEST_TARGET_DIR = $(TARGET_DIR)

CC          = g++
CFLAGS      = -Wall -std=c++11 -Wno-unused-function -I$(INC_DIR) -I$(DUT_DIR)/inc
LIBS        =
LIB_FLAGS   =

#SRCS   = $(SRC_DIR)/main.cpp $(SRC_DIR)/testcases.cpp $(SRC_DIR)/strings.cpp $(DUT_DIR)/../src/replacer.cpp
SRCS   = $(SRC_DIR)/main.cpp $(SRC_DIR)/testcases.cpp $(SRC_DIR)/strings.cpp
SRCS   += $(DUT_DIR)/src/replacer.cpp $(DUT_DIR)/src/replacerBase.cpp $(DUT_DIR)/src/multiReplacer.cpp
OBJ    = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS   = $(patsubst %,$(DUT_DIR)/inc/%.h, *)


.PHONY: all clean test

all: test

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET_DIR)/$(TARGET): $(OBJ)
	$(CC) -o $(TARGET_DIR)/$(TARGET) $^ $(CFLAGS) $(LIB_FLAGS) $(LIBS)

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET)

test: $(TARGET_DIR)/$(TARGET)
	$(TARGET_DIR)/$(TARGET)
