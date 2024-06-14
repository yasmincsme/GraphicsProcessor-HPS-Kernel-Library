obj-m += gpp_data_bus.o

TARGET_DIR = $(PWD)/src/kernel
BUILD_DIR = $(PWD)/build

# All *.c files.
SOURCES = \
					src/GraphSync.c \
					src/main.c 

# All *.h files.
HEADERS = \
					src/ui.h \
					src/types.h \
					src/GraphSync.h 		

all: kernel build 

build: build/dir build/main

build/dir:
	@mkdir -p build

build/main: build/GraphSync.o build/main.o 
	@gcc build/GraphSync.o build/main.o -o build/main

build/GraphSync.o: src/GraphSync.c src/GraphSync.h src/ui.h src/types.h
	@gcc -c src/GraphSync.c -o build/GraphSync.o -Isrc

build/main.o: src/main.c src/GraphSync.h src/ui.h src/types.h
	@gcc -c src/main.c -o build/main.o -Isrc

run: build 
	@sudo ./build/main

kernel:
	make -C /lib/modules/$(shell uname -r)/build M=$(TARGET_DIR) modules
	
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(TARGET_DIR) clean #Não executar como está
	@sudo rm -r build

load: 
	@sudo insmod src/kernel/gpp_data_bus.ko

unload:
	@sudo rmmod gpp_data_bus

IGNORE_FILES_FORMAT =
HEADERS_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(HEADERS))
SOURCES_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(SOURCES))

format:
	@clang-format -i -style=llvm $(HEADERS_FORMAT) $(SOURCES_FORMAT)

.PHONY: format
