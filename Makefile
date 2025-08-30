CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

SERVER = $(BIN_DIR)/server
CLIENT = $(BIN_DIR)/client

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJ_DIR)/server.o $(OBJ_DIR)/sockets.o | $(BIN_DIR)
	$(CC) $^ -o $@

$(CLIENT): $(OBJ_DIR)/client.o $(OBJ_DIR)/sockets.o | $(BIN_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
