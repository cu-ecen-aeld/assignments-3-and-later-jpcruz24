CC = $(CROSS_COMPILE)gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Source files
SRCS = aesdsocket.c

#Output 
OUTPUT = aesdsocket.o

# Executable name
EXEC = aesdsocket

# Build rule
all: $(EXEC)

$(EXEC):aesdsocket.o
	$(CC) $(CFLAGS) -o $(EXEC) aesdsocket.o
aesdsocket.o: aesdsocket.c
	$(CC) $(CFLAGS) -c aesdsocket.c
# Clean rule
clean:
	rm -f $(EXEC) *.o
