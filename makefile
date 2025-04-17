# Makefile for echo.c

CC = gcc
CFLAGS = -Wall -g
TARGET = echo
SRC = echo.c
CLIENT_TARGET = echo_client
CLIENT_SRC = echo_client.c

all: $(TARGET) $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRC)
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)