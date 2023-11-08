# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -lpthread -O0

# Define the source files and corresponding executables
SRCS = $(wildcard *.c)
EXES = $(SRCS:.c=)

# Phony target to build all executables
.PHONY: all clean

# Define the default target
all: $(EXES)

# Define the rules for compiling each source file to an executable
$(EXES): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Define a clean target to remove all generated files
clean:
	rm -f $(EXES)
