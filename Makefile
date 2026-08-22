CC = gcc 
WFLAGS = -Wall -Werror
NOLIBS = -nodefaultlibs -nostdlib -fno-stack-protector -ffreestanding -fno-omit-frame-pointer
LDFLAGS = $(NOLIBS) -static
CFLAGS = $(WFLAGS) -O3 $(NOLIBS) -g
O0CFLAGS = -O0 $(NOLIBS) -g
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/simsh
SOURCES = $(shell ls|grep .c) 
O0SOURCES = syscall.c 
LOL = $(filter-out $(O0SOURCES), $(SOURCES))
OBJS = $(LOL:%.c=$(OBJ_DIR)/%.o)
O0OBJS = $(OBJ_DIR)/syscall.oa

$(TARGET): $(OBJS) $(O0OBJS) 
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) 

$(OBJ_DIR)/%.oa: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $^ $(O0CFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $^ $(CFLAGS)

run: $(TARGET) 
	$(TARGET) $(RUN_DIR) 2> log.txt || true
clean:
	rm $(OBJ_DIR)/*
	rm $(BIN_DIR)/*
