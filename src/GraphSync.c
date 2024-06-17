#include "GraphSync.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils/aux.h"

#define DEVICE_PATH "/dev/gpp_data_bus"

// build WBM DP

void write_DP(u8_t type, u8_t R, u8_t G, u8_t B, u8_t lenght, u16_t x, u16_t y, u16_t adress) {
  u8_t opcode = 0b0001; ////////////

  u32_t dataA = adress << 4 | opcode;
  u32_t dataB = type << 31 | B << G << R <<

  char dataA_str[32];
  char dataB_str[32];
  char instruction[64];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcat(instruction, dataA_str);
  strcat(instruction, dataB_str);

  u16_t fd = open(DEVICE_PATH, O_RDWR);

  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  write_data(fd, (u64_t)instruction);
  close(fd);
}
}

void write_WSM(u8_t R, u8_t G, u8_t B, u32_t adress) {
  u8_t opcode = 0b0001;

  u32_t dataA = adress << 4 | opcode;
  u32_t dataB = B << 6 | G << 3 | R;

  char dataA_str[32];
  char dataB_str[32];
  char instruction[64];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcat(instruction, dataA_str);
  strcat(instruction, dataB_str);

  u16_t fd = open(DEVICE_PATH, O_RDWR);

  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  write_data(fd, (u64_t)instruction);
  close(fd);
}

void write_WBR_sprite(u8_t ativo, u16_t x, u16_t y, u16_t offset, u8_t reg) {
  u8_t opcode = 0b0000;

  u32_t dataA = reg << 6 | opcode;
  u32_t dataB = ativo << 29 | x << 19 | y << 9 | offset;

  char dataA_str[32];
  char dataB_str[32];
  char instruction[64];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcat(instruction, dataA_str);
  strcat(instruction, dataB_str);

  u16_t fd = open(DEVICE_PATH, O_RDWR);

  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  write_data(fd, (u64_t)instruction);
  close(fd);
}

void write_WBR_background(u8_t R, u8_t G, u8_t B) {
  u8_t opcode = 0b0000;
  u8_t reg = 0b00000;

  u32_t dataA = reg << 6 | opcode;
  u32_t dataB = B << 6 | G << 3 | R;

  char dataA_str[32];
  char dataB_str[32];
  char instruction[64];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcat(instruction, dataA_str);
  strcat(instruction, dataB_str);

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcat(instruction, dataA_str);
  strcat(instruction, dataB_str);

  u16_t fd = open(DEVICE_PATH, O_RDWR);

  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  write_data(fd, (u64_t)instruction);
  close(fd);
}

void write_data(u16_t fd, u64_t data) {
  ssize_t result = write(fd, &data, sizeof(data));
  if (result == -1) {
    perror("Falha na escrita do dispositivo");
    exit(EXIT_FAILURE);
  }
}

u64_t read_data(u64_t fd) {
  u64_t data;
  ssize_t result = read(fd, &data, sizeof(data));
  if (result == -1) {
    perror("Falha na leitura do dispositivo");
    exit(EXIT_FAILURE);
  }

  return data;
}

sprite_fixed_t init_fixed_sprite(u8_t reg, u8_t offset) {
  sprite_fixed_t sprite;
  sprite.coord_x = 0;
  sprite.coord_y = 0;
  sprite.offset = offset;
  sprite.data_register = reg;
  sprite.ativo = 0;
  return sprite;
}

void view_sprite(void *sprite_ptr, sprite_type_t type) {
  switch (type) {
    case SPRITE_NORMAL: {
      sprite_t *sprite = (sprite_t *)sprite_ptr;
      sprite->ativo = 1;
      break;
    }
    case SPRITE_FIXED: {
      sprite_fixed_t *fixed_sprite = (sprite_fixed_t *)sprite_ptr;
      fixed_sprite->ativo = 1;
      break;
    }
    default:
      break;
  }
}

void hide_sprite(void *sprite_ptr, sprite_type_t type) {
  switch (type) {
    case SPRITE_NORMAL: {
      sprite_t *sprite = (sprite_t *)sprite_ptr;
      sprite->ativo = 0;
      break;
    }
    case SPRITE_FIXED: {
      sprite_fixed_t *fixed_sprite = (sprite_fixed_t *)sprite_ptr;
      fixed_sprite->ativo = 0;
      break;
    }
    default:
      break;
  }
}

u8_t set_fixed_sprite(sprite_fixed_t *sprite_ptr, u8_t x, u8_t y) {
  sprite_ptr->coord_x = x;
  sprite_ptr->coord_y = y;
}

// u32_t set_fixed_sprite(sprite_fixed_t) {}

uint64_t set_background_block(int column, int line, int R, int G, int B) {
  int opcode = 0010;
  int memory_address = column * line;

  uint32_t dataA = memory_address << 27 | opcode << 23;
  uint32_t dataB = B << 6 | G << 3 | R;

  /*Transformando dataA e dataB em caracteres e concatenando as strings*/
  char data_a_str[32];
  char data_b_str[32];
  char instruction[64];

  itoa(dataA, data_a_str, 2);
  itoa(dataB, data_b_str, 2);
  strcat(instruction, data_a_str);
  strcat(instruction, data_b_str);

  printf("%s\n", instruction);
  return instruction;
}

uint64_t set_background_color(int R, int G, int B) {
  long long int instruction = B << 6 | G << 3 | R;

  return instruction;
}

uint64_t set_polygon(int reg, int format, int R, int G, int B, int size, int ref_point_y, int ref_point_x) {
  int opcode = 0011;

  uint32_t dataA = reg << 5 | opcode;  // TOMAR CUIDADO PORQUE SE A FORMA FOR 0 DE QUADRADO ELE TIRA OS BITS DA FORMA
  uint32_t dataB = format << 31 | R << 28 | G << 25 | R << 22 | size << 18 | ref_point_y << 9 | ref_point_x;

  /*Transformando dataA e dataB em caracteres e concatenando as strings*/
  char data_a_str[32];
  char data_b_str[32];
  char instruction[64];

  itoa(dataA, data_a_str, 2);
  itoa(dataB, data_b_str, 2);
  strcat(instruction, data_a_str);
  strcat(instruction, data_b_str);

  printf("%s\n", instruction);
  return instruction;
}

void increase_coordinate(sprite_t *sp, int mirror) {}

int collision(sprite_t *sp1, sprite_t *sp2) {}
