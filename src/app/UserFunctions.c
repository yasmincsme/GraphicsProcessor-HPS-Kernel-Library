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
  int reg = to_binary_regs(0);
  int opcode = WBR;

  *DATA_A_ptr = (reg * 2 ^ 5) + (opcode * 2 ^ 4);
  *DATA_B_ptr = (to_binary_colors(B) * 2 ^ 3) + (to_binary_colors(G) * 2 ^ 3) + (to_binary_colors(R) * 2 ^ 3);
  clock_pulse();

  return 0;
}

/*TRANSFORMAR OS DADOS DESSA FUNÇÃO PARA BINÁRIO!*/
int set_sprite(int reg, int coord_x, int coord_y, int sprite, int activation) {
  reg = to_binary_regs(reg);
  int opcode = WBR;

  *DATA_A_ptr = (reg * 2 ^ 5) + (opcode * 2 ^ 4);
  *DATA_B_ptr = (activation * 2) + (coord_x *)+(coord_y *)+(sprite *); /*ATENÇÃO AQUI*/
  clock_pulse();

  return 0;
}

int close_file() {
  if (munmap(LW_virtual, LW_BRIDGE_SPAN) != 0) {
    printf("ERROR: munmap( failed...\n)");
    return (-1);
  }

  close(fd);
  return 0;
}

int to_binary_colors(int value) {
  switch (value) {
    case 0:
      return 0b000;
    case 1:
      return 0b001;
    case 2:
      return 0b010;
    case 3:
      return 0b011;
    case 4:
      return 0b100;
    case 5:
      return 0b101;
    case 6:
      return 0b110;
    case 7:
      return 0b111;
    default:
      return 0b000;
  }
}

int to_binary_regs(int value) {
  switch (value) {
    case 0:
      return 0b00000;
    case 1:
      return 0b00001;
    case 2:
      return 0b00010;
    case 3:
      return 0b00011;
    case 4:
      return 0b00100;
    case 5:
      return 0b00101;
    case 6:
      return 0b00110;
    case 7:
      return 0b00111;
    case 8:
      return 0b01000;
    case 9:
      return 0b01001;
    case 10:
      return 0b01010;
    case 11:
      return 0b01011;
    case 12:
      return 0b01100;
    case 13:
      return 0b01101;
    case 14:
      return 0b01110;
    case 15:
      return 0b01111;
    case 16:
      return 0b10000;
    case 17:
      return 0b10001;
    case 18:
      return 0b10010;
    case 19:
      return 0b10011;
    case 20:
      return 0b10100;
    case 21:
      return 0b10101;
    case 22:
      return 0b10110;
    case 23:
      return 0b10111;
    case 24:
      return 0b11000;
    case 25:
      return 0b11001;
    case 26:
      return 0b11010;
    case 27:
      return 0b11011;
    case 28:
      return 0b11100;
    case 29:
      return 0b11101;
    case 30:
      return 0b11110;
    case 31:
      return 0b11111;
    default:
      return 0b00000;
  }
}
