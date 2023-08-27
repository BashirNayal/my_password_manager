BIN_EXT:=bin
C_COMPILER:=gcc
C_FLAGS:=-g -W -Wall -Wextra -Wpedantic -Wconversion -Wshadow
LD_FLAGS:=-lsqlite3
DIR_BUILD:=build
DIR_ALL=$(DIR_BUILD) $(DIR_BUILD)/$(SERVER_DIR)  $(DIR_BUILD)/$(COMMON_DIR) $(DIR_BUILD)/$(CLIENT_DIR) 
C_FLAGS += -DDEBUG

DATABASE_DIR:=common
DATABASE_SRC:=$(wildcard $(DATABASE_DIR)/src/*.c)
DATABASE_OBJ:=$(COMMON_SRC:$(DATABASE_DIR)/src/%.c=$(DIR_BUILD)/$(DATABASE_DIR)/%.o)
DATABASE_DEP:=$(COMMON_OBJ:%.o=%.d)
DATABASE_C_FLAGS:=-I$(DATABASE_DIR)/include

COMMON_DIR:=common
COMMON_SRC:=$(wildcard $(COMMON_DIR)/src/*.c)
COMMON_OBJ:=$(COMMON_SRC:$(COMMON_DIR)/src/%.c=$(DIR_BUILD)/$(COMMON_DIR)/%.o)
COMMON_DEP:=$(COMMON_OBJ:%.o=%.d)
COMMON_C_FLAGS:=-I$(COMMON_DIR)/include


CLIENT_NAME:=client
CLIENT_DIR:=client
CLIENT_SRC:=$(wildcard $(CLIENT_DIR)/src/*.c)
CLIENT_OBJ:=$(CLIENT_SRC:$(CLIENT_DIR)/src/%.c=$(DIR_BUILD)/$(CLIENT_DIR)/%.o)
CLIENT_DEP:=$(CLIENT_OBJ:%.o=%.d)
CLIENT_C_FLAGS:=-I$(CLIENT_DIR)/include -I$(COMMON_DIR)/include


SERVER_NAME:=server
SERVER_DIR:=server
SERVER_SRC:=$(wildcard $(SERVER_DIR)/src/*.c)
SERVER_OBJ:=$(SERVER_SRC:$(SERVER_DIR)/src/%.c=$(DIR_BUILD)/$(SERVER_DIR)/%.o)
SERVER_DEP:=$(SERVER_OBJ:%.o=%.d)
SERVER_C_FLAGS:=-I$(SERVER_DIR)/include -I$(COMMON_DIR)/include

.PHONY: all server client  clean

all: server client 


client: $(DIR_BUILD) $(DIR_BUILD)/$(COMMON_DIR) $(DIR_BUILD)/$(CLIENT_DIR) $(DIR_BUILD)/$(CLIENT_NAME).$(BIN_EXT) 
$(DIR_BUILD)/$(CLIENT_NAME).$(BIN_EXT): $(COMMON_OBJ) $(CLIENT_OBJ)
	$(C_COMPILER) $(CLIENT_OBJ) $(COMMON_OBJ) -o $(@) $(C_FLAGS) $(CLIENT_C_FLAGS) $(LD_FLAGS)

server: $(DIR_BUILD) $(DIR_BUILD)/$(COMMON_DIR) $(DIR_BUILD)/$(SERVER_DIR) $(DIR_BUILD)/$(SERVER_NAME).$(BIN_EXT) 
$(DIR_BUILD)/$(SERVER_NAME).$(BIN_EXT): $(COMMON_OBJ) $(SERVER_OBJ)
	$(C_COMPILER) $(SERVER_OBJ) $(COMMON_OBJ) -o $(@) $(C_FLAGS) $(SERVER_C_FLAGS) $(LD_FLAGS)


$(DIR_BUILD)/$(CLIENT_DIR)/%.o: $(CLIENT_DIR)/src/%.c
	$(C_COMPILER) $(<) -o $(@) $(C_FLAGS) $(CLIENT_C_FLAGS) -c -MMD
$(DIR_BUILD)/$(SERVER_DIR)/%.o: $(SERVER_DIR)/src/%.c
	$(C_COMPILER) $(<) -o $(@) $(C_FLAGS) $(SERVER_C_FLAGS) -c -MMD 
$(DIR_BUILD)/$(COMMON_DIR)/%.o: $(COMMON_DIR)/src/%.c
	$(C_COMPILER) $(<) -o $(@) $(C_FLAGS) $(COMMON_C_FLAGS) -c -MMD


-include $(CLIENT_DEP)
-include $(SERVER_DEP)
-include $(COMMON_DEP)


$(DIR_ALL):
	mkdir -p $(@)
	
clean:
	rm -rf $(DIR_BUILD)
