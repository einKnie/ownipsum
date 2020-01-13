BASE        = .
SRC_DIR     = $(BASE)
INC_DIR     = $(BASE)
LIB_DIR     = $(BASE)/../libraries/log
TARGET_DIR  = $(BASE)
OBJ_DIR     = $(BASE)/obj
TARGET      = out

CXX = g++

LIBS = -lcpplogging
LIBFLAGS = -I$(LIB_DIR)/inc -L$(LIB_DIR)/bin $(LIBS)
CFLAGS = -std=c++11 -Wall -Werror -I$(INC_DIR)

SRCS       := $(wildcard $(SRC_DIR)/*.cpp)

OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS        = $(patsubst %, $(INC_DIR)/%.h, *)

.PHONY: all clean

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CXX) -o $(TARGET_DIR)/$(TARGET) $^ $(CFLAGS)

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
