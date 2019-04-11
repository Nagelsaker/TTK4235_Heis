SOURCES := main.c elev.c io.c queue.c controller.c timer.c

BUILD_DIR := build
OBJ := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

SRC_DIR := source
SRC := $(SOURCES:%.c=$(SRC_DIR)/%.c)

CC := clang
CFLAGS := -O0 -g3 -Wall -Werror -std=gnu99

LDFLAGS := -lcomedi -lm

.DEFAULT_GOAL := heis

heis : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(.DEFAULT_GOAL) $(BUILD_DIR)
