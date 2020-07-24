BASE        = ..
SRC_DIR     = $(BASE)/src
INC_DIR     = $(BASE)/inc
INC_DIR_ADD = $(BASE)/../lilist/
LIB_DIR     = $(BASE)/../libraries/log
TARGET_DIR  = $(BASE)/bin
OBJ_DIR     = $(BASE)/obj
TARGET      = out

CXX = g++

LIBS = -lcpplogging
LIBFLAGS = -I$(LIB_DIR)/inc -L$(LIB_DIR)/bin $(LIBS)
CFLAGS = -std=c++11 -Wall -Werror -g -I$(INC_DIR) -I$(INC_DIR_ADD)

SRCS       := $(wildcard $(SRC_DIR)/*.cpp)
OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS        = $(patsubst %, $(INC_DIR)/%.h, *)

.PHONY: all clean

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) -c -o $@ $< $(CFLAGS)

all: $(OBJ) | $(TARGET_DIR)
	$(CXX) -o $(TARGET_DIR)/$(TARGET) $^ $(CFLAGS)

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET) $(TARGET_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)
