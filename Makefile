CC = gcc

#Name of the main executable of the project
TARGET_NAME = source

#Directories/files related to the build of the project
BUILD_DIR = build

OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/BIN_DIR

COMPILATION_DATABASE = $(BUILD_DIR)/compile_commands.json

TARGET = $(BUILD_DIR)/bin/$(TARGET_NAME)

SOURCES_WITH_HEADERS = \
								#src

INCLUDE_DIRS = \
								./src \

MAIN_FILE = src/main.c 

SOURCES = \
								$(MAIN_FILE) \
								$(SOURCES_WITH_HEADERS)

HEADERS = \
								$(SOURCES_WITH_HEADERS:.c=.h) \

# Names for the Object files generated from the compilation.
OBJECT_NAMES = $(SOURCES:.c=.o)

# Paths to the Object files generated from the compilation.
OBJECTS = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))

# Flags to tune error levels in the compilation process.
WFLAGS = -Wall -Wextra -pedantic
WFLAGS += -Wno-unused-parameter -Wno-unused-variable \
					-Wno-unused-but-set-variable -Wno-missing-field-initializers

# Flags to be passed in the compilation and linking process, respectively.
CFLAGS = -std=c99
CFLAGS += $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS))
LDFLAGS = $(addprefix -I, $(INCLUDE_DIRS)) #-lintelfpgaup

help: ## Show all the available targets.
	@echo "Available targets:"
	@grep -E "^[a-zA-Z0-9_-]+:.*?## .*$$" $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

# Linking...
$(TARGET): $(OBJECTS) $(HEADERS)
	@echo $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compilation...
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $^ $(CFLAGS)

build: $(TARGET) ## Compiles the project.

run: $(TARGET) ## Runs the project with `root` permissions.
	#sudo @./$^
	sudo ./build/bin/source

clean: ## Remove all files generated in the compilation.
	@rm -rf $(BUILD_DIR)

.PHONY: all help format build run clean