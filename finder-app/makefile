CC = $(CROSS_COMPILE)gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Source files
SRCS = writer.c

#Output 
OUTPUT = write.o

# Executable name
EXEC = writer

# Build rule
all: $(EXEC)

$(EXEC):writer.o
	$(CC) $(CFLAGS) -o $(EXEC) writer.o
writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c
# Clean rule
clean:
	rm -f $(EXEC) *.o
