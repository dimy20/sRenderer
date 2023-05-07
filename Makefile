EXEC = main
CC = gcc

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

LDFLAGS = -lSDL2 -lm
CFLAGS = -Werror -Wall -std=c11 -g -O2 -I$(INCLUDE_DIR)

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm $(BUILD_DIR)/*.o $(EXEC)
