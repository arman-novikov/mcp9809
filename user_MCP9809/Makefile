CC=g++
CFLAGS=-c -Wall

all: MCP9808

MCP9808: MCP9808.o
	$(CC) -no-pie MCP9808.o -o MCP9808

clean:
	rm -rf *.o MCP9808
