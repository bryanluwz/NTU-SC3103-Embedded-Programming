# Compiler and Compilation Flags
CC = gcc
CFLAGS = -Wall -O0
DEBUG_CFLAGS = -g

# Sourcce and Object Files Variables
SOURCES = hello-main.c hello-funct1.c hello-funct2.c
OBJECTS = $(SOURCES:%.c=%.o)

# Output File
EXECUTABLE = helloworld
DEBUG_EXECUTABLE = helloworld-d

# PHONY targets
.PHONY: all clean debug gprof

# Default Target
all: $(EXECUTABLE)

# Debug Target
debug: $(DEBUG_EXECUTABLE)
debug: CFLAGS+= $(DEBUG_CFLAGS)

# Gprof
gprof: CFLAGS += -pg
gprof: $(EXECUTABLE)

# Build Executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Build Debug Executable
$(DEBUG_EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile to Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DEBUG_EXECUTABLE)
