# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -pthread

# Target executable name
TARGET = simulation

# Source files (list all .c files)
SOURCES = main.c manager.c system.c resource.c event.c

# Object files (automatically generated from source files)
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS)

# Compilation rules for each source file
main.o: main.c defs.h
	$(CC) $(CFLAGS) -c main.c

manager.o: manager.c defs.h
	$(CC) $(CFLAGS) -c manager.c

system.o: system.c defs.h
	$(CC) $(CFLAGS) -c system.c

resource.o: resource.c defs.h
	$(CC) $(CFLAGS) -c resource.c

event.o: event.c defs.h
	$(CC) $(CFLAGS) -c event.c

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
