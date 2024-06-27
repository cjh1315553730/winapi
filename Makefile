# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Libraries
LIBS = -llua -lpsapi -lkernel32 -lntdll -lws2_32 -lwsock32

# Directories
LUA_INCLUDE_DIR = -I./lua/lua-5.4.6/src
LUA_LIB_DIR = -L"./lua/lua-5.4.6/src"

# Target DLL
DLL_TARGET = winapi.dll

# Source directory
SRCDIR = src_cplusplus
# DEST_DIR = example\\

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Main target (all)
all: $(DLL_TARGET)

$(DLL_TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LUA_INCLUDE_DIR) $(LUA_LIB_DIR) -shared -o $@ $^ $(LIBS)

# copy $(DLL_TARGET) $(DEST_DIR)
# if exist $(DLL_TARGET) del $(DLL_TARGET)

# Rule for compiling .c files to .o files (for executable)
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(LUA_INCLUDE_DIR) -c -o $@ $<

# Clean target
clean:
	if exist $(DLL_TARGET) del $(DLL_TARGET)

# if exist $(DEST_DIR) del $(DEST_DIR)$(DLL_TARGET)

# Phony targets to avoid conflicts with files named 'all' or 'clean'
.PHONY: all clean