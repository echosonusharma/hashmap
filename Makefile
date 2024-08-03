CC = clang
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Source files
SRCS = src/main.c
# Object files are now prefixed with obj/ to indicate they should be placed in an obj directory
OBJS = $(SRCS:.c=.o)
# Executable is now placed in the bin directory
EXEC = bin/hashmap

# Ensure the bin directory exists
$(shell mkdir -p bin)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean run
