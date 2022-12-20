CC = gcc

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
DEB_DIR = debug

# Compiler Flags
CFLAGS = -g -Wall -Wextra

# Files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/$(OBJ_DIR)/%.o, $(SRC))


# Targets
TARGET = prayer_times

all: $(OBJ) $(TARGET)

$(SRC_DIR)/$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | mkobj
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET) : $(OBJ) | mkdeb
	$(CC) $(CFLAGS) -o $@ $^

leaks:
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

run : FORCE
	./$(TARGET)

mkobj : FORCE
	mkdir -p $(SRC_DIR)/$(OBJ_DIR)

mkdeb: FORCE
	mkdir -p $(DEB_DIR)


clean: FORCE
	rm -rf $(DEB_DIR) $(SRC_DIR)/$(OBJ_DIR)

FORCE: 
