# Compiler to be used.
CC = gcc

# Name for the main executable of the project.
TARGET_NAME = source

# Directories/Files related to the build of the project.
BUILD_DIR = build

OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

COMPILATION_DATABASE = $(BUILD_DIR)/compile_commands.json

# Location of the project final executable.
TARGET = $(BUILD_DIR)/bin/$(TARGET_NAME)

# Files to be included in the compilation.
SOURCES_WITH_HEADERS = \
											 


# Directories to be included in the compilation.
INCLUDE_DIRS = \
							 ./src \

# Entry point for the project.
MAIN_FILE = src/main.c

# All *.c files.
SOURCES = \
					$(MAIN_FILE) \
					$(SOURCES_WITH_HEADERS)

# All *.h files.
HEADERS = \
					$(SOURCES_WITH_HEADERS:.c=.h) \
					

# Files (*.c or *.h) to be ignored in the `format` target.
IGNORE_FILES_FORMAT =

# Files (*.c or *.h) to be included in the `format` target.
HEADERS_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(HEADERS))
SOURCES_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(SOURCES))

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
LDFLAGS = $(addprefix -I, $(INCLUDE_DIRS)) -lintelfpgaup

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

format: ## Formats code using `clang-format`.
ifeq (, $(shell which clang-format 2> /dev/null))
	$(error `clang-format` wasn't found! Consider installing it trough your package manager)
else
	@clang-format -i -style=llvm $(SOURCES_FORMAT) $(HEADERS_FORMAT)
endif


.PHONY: all help format build run clean