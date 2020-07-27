BASE        = ..
SRC_DIR     = $(BASE)/src
INC_DIR     = $(BASE)/inc
TARGET_DIR  = $(BASE)/bin
OBJ_DIR     = $(BASE)/obj
TARGET      = ownipsum

CXX = g++
CFLAGS = -std=c++11 -Wall -Werror -g -I$(INC_DIR)

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
