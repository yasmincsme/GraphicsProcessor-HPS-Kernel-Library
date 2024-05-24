#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "/home/aluno/TEC499/LabExemples/auxFiles/address_map_arm.h"

#define WRREG 0xc0
#define DATA_B 0x70
#define DATA_A 0x80

volatile int * WRREG_ptr;
volatile int * DATA_A_ptr;
volatile int * DATA_B_ptr;
volatile int * VIDEO_IN_BASE_ptr;

int fd; // used to open /dev/mem
void *LW_virtual = get_mapping(); // physical addresses for light-weight bridge

// int to_binary (int value) {
//     switch (value) {
//         case 1: return ;
//         default: return 0000000;
//     }
// }

int open_file () {

    fd = -1;

    /*Open /dev/mem to give access to physical addresses*/
    if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf ("ERROR: could not open \"/dev/mem\"...\n");
        return (-1);
    }

    return 0;
}

void * get_mapping() {

    /*Get a mapping from physical addresses to virtual addresses*/
    LW_virtual = mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (-1);
    }

    WRREG_ptr = (int *) (LW_virtual + WRREG);
    DATA_A_ptr = (int *) (LW_virtual + DATA_A);
    DATA_B_ptr = (int *) (LW_virtual + DATA_B);

    return LW_virtual;
}

set_background_color(int Red, int Green, int Blue) {

    int reg = 0b00000;
    int opcode = 0b0000;





}

set_sprite() {

}

close