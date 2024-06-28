obj-m +=  kernel/gpp_data_bus.o

TARGET_DIR = $(PWD)/kernel
BUILD_DIR = $(PWD)/build
KERNEL_DIR = /lib/modules/$(shell uname -r)/build

# All *.c files.
SOURCES =  GraphSync.c  main.c 

# All *.h files.
HEADERS =  utils/ui.h  utils/types.h  GraphSync.h

all: build kernel

build: build/dir build/main

build/dir:
	@mkdir -p build

build/main: build/GraphSync.o build/aux.o build/main.o 
	@gcc build/GraphSync.o build/aux.o build/main.o -o build/main

build/aux.o:  utils/aux.c  utils/types.h
	@gcc -c  utils/aux.c -o build/aux.o -Isrc -Iutils

build/mouse.o: utils/state.h utils/mouse.h utils/types.h 
	@gcc -c utils/mouse.c -o build/mouse.o -Isrc -Iutils 

build/window.o: utils/window.h utils/types.h 
	@gcc -c utils/window.c -o build/window.o -Isrc -Iutils

build/GraphSync.o:  GraphSync.c  GraphSync.h  utils/ui.h  utils/types.h build/aux.o
	@gcc -c  GraphSync.c -o build/GraphSync.o -Isrc -Iutils


build/main.o:  main.c  GraphSync.h  utils/ui.h  utils/types.h
	@gcc -c  main.c -o build/main.o -Isrc -Iutils

run: build 
	@sudo ./build/main

kernel:
	make -C $(KERNEL_DIR) M=$(TARGET_DIR) modules

clean: 
	make -C $(KERNEL_DIR) M=$(TARGET_DIR) clean
	@sudo rm -r build

load: 
	@sudo insmod $(TARGET_DIR)/gpp_data_bus.ko

unload:
	@sudo rmmod gpp_data_bus

IGNORE_FILES_FORMAT =
HEADERS_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(HEADERS))
SOURCES_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(SOURCES))

format:
	@clang-format -i -style=llvm $(HEADERS_FORMAT) $(SOURCES_FORMAT)

.PHONY: format unload load clean kernel build all
