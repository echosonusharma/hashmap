CC = clang
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -Ilib

# Source files
SRCS = src/main.c lib/siphash.c
# Specify a separate directory for object files
OBJ_DIR = build
# Object files in the OBJ_DIR
OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
EXEC = bin/hashmap

# Ensure the directories exist
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p bin)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule for compiling C source files into object files in the specified OBJ_DIR
$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: lib/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

.PHONY: all clean run
