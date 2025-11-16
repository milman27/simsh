CC =gcc 
WFLAGS = 
NOLIBS = -nodefaultlibs -nostdlib -fno-stack-protector -ffreestanding
LDFLAGS = $(NOLIBS) 
CFLAGS = $(WFLAGS) -O0 $(NOLIBS)
O0CFLAGS = -O0 $(NOLIBS)
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/simsh
SOURCES = main.c 
O0SOURCES = syscall.c

OBJS = $(OBJ_DIR)/main.o 
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

