# Copyright 2017 Root404, Co
#                Alvaro Stagg [alvarostagg@openmailbox.org]
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

CC = g++
CC_FLAGS = -Wall -Werror --std=c++11
ADD_FLAGS = -lcryptopp

OBJ_NAME = cardenal

CORE_DIR_SOURCES = src/*.cpp

CLI_DIR_SOURCES = cli/*.cpp
CLI_DIR_BINARY = cli/bin

# g++ -Wall -Werror --std=c++11 ../cli/Main.cpp Cipher.cpp -lcryptopp -o ../cli/bin/cardenal
console:
	mkdir -p $(CLI_DIR_BINARY)
	$(CC) $(CC_FLAGS) $(CORE_DIR_SOURCES) $(CLI_DIR_SOURCES) $(ADD_FLAGS) -o $(CLI_DIR_BINARY)/$(OBJ_NAME)

clean:
	rm -rf $(CLI_DIR_BINARY)
