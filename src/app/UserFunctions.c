#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <utils/ui.h>
#include "/home/aluno/TEC499/LabExemples/auxFiles/address_map_arm.h"

volatile int *WRREG_ptr;
volatile int *DATA_A_ptr;
volatile int *DATA_B_ptr;
volatile int *VIDEO_IN_BASE_ptr;

int fd;                            // used to open /dev/mem
void *LW_virtual = get_mapping();  // physical addresses for light-weight bridge

int to_binary_colors(int value) {
    switch (value) {
        case 0:
        return 000;
        case 1:
        return 001;
        case 2:
        return 010;
        case 3:
        return 011;
        case 4:
        return 100;
        case 5:
        return 101;
        case 6:
        return 110;
        case 7:
        return 111;
        default:
        return 000;
    }
}

/**/
int open_file() {
    fd = -1;

    /*Open /dev/mem to give access to physical addresses*/
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return (-1);
    }

    return 0;
}

/**/
void *get_mapping() {
    /*Get a mapping from physical addresses to virtual addresses*/
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return (-1);
    }

    WRREG_ptr = (int *)(LW_virtual + WRREG);
    DATA_A_ptr = (int *)(LW_virtual + DATA_A);
    DATA_B_ptr = (int *)(LW_virtual + DATA_B);

    return LW_virtual;
}

/**/
void clock_pulse() {
    *WRREG_ptr = LOW_LOGIC_LEVEL;
    *WRREG_ptr = HIGHT_LOGIC_LEVEL;
}

/**/
int set_background_color(int R, int G, int B) {
    int reg = 0b00000;
    int opcode = WBR;

    *DATA_A_ptr = (reg * 2 ^ 5) + (opcode * 2 ^ 4);
    *DATA_B_ptr = (to_binary_colors(B) * 2^3) + (to_binary_colors(G) * 2^3) + (to_binary_colors(R) * 2^3);
    clock_pulse();

    return 0;
}

set_sprite(int reg, int coord_x, int coord_y, ) {}

close