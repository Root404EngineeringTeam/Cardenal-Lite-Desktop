CC = g++
CC_FLAGS = --std=c++11 -Wall -Werror -lcryptopp

OBJ_NAME = cardenal

CORE_DIR_SOURCES = src/*.cpp

CLI_DIR_SOURCES = cli/*.cpp
CLI_DIR_BINARY = cli/bin

console:
	mkdir -p $(CLI_DIR_BINARY)
	$(CC) $(CC_FLAGS) $(CORE_DIR_SOURCES) $(CLI_DIR_SOURCES) -o $(CLI_DIR_BINARY)/$(OBJ_NAME)

clean:
	rm -rf $(CLI_DIR_BINARY)