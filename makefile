BIN_EXT:=bin
C_COMPILER:=gcc
C_FLAGS:=-g -W -Wall -Wextra -Wpedantic -Wconversion -Wshadow
LD_FLAGS:=
DIR_BUILD:=build
# All directories which should be created (build dir should contain them all).
DIR_ALL=$(DIR_BUILD) $(DIR_BUILD)/$(SERVER_DIR)  $(DIR_BUILD)/$(COMMON_DIR)
C_FLAGS += -DDEBUG

COMMON_DIR:=common
COMMON_SRC:=$(wildcard $(COMMON_DIR)/src/*.c)
COMMON_OBJ:=$(COMMON_SRC:$(COMMON_DIR)/src/%.c=$(DIR_BUILD)/$(COMMON_DIR)/%.o)
COMMON_DEP:=$(COMMON_OBJ:%.o=%.d)
COMMON_C_FLAGS:=-I$(COMMON_DIR)/include


SERVER_NAME:=server
SERVER_DIR:=server
SERVER_SRC:=$(wildcard $(SERVER_DIR)/src/*.c)
SERVER_OBJ:=$(SERVER_SRC:$(SERVER_DIR)/src/%.c=$(DIR_BUILD)/$(SERVER_DIR)/%.o)
SERVER_DEP:=$(SERVER_OBJ:%.o=%.d)
SERVER_C_FLAGS:=-I$(SERVER_DIR)/include -I$(COMMON_DIR)/include


$(DIR_ALL):
	mkdir -p $(@)
clean:
	rm -rf $(DIR_BUILD)
