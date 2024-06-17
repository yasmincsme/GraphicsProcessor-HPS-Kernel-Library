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

// void write_DP(u8_t type, u8_t R, u8_t G, u8_t B, u8_t lenght, u16_t x, u16_t y, u16_t adress) {
//   u8_t opcode = 0b0011; ////////////

//   u32_t dataA = adress << 4 | opcode;
//   u32_t dataB = type << 31 | B << 28 | G << 25 | R << 22 | lenght << 18 | y << 9 | x;

//   char dataA_str[32];
//   char dataB_str[32];
//   char instruction[64];

//   itoa(dataA, dataA_str, 2);
//   itoa(dataB, dataB_str, 2);
//   strcat(instruction, dataA_str);
//   strcat(instruction, dataB_str);

//   u16_t fd = open(DEVICE_PATH, O_RDWR);

//   if (fd == -1) {
//     perror("Falha ao abrir o dispositivo");
//     exit(EXIT_FAILURE);
//   }

//   write_data(fd, (u64_t)instruction);
//   close(fd);
// }

// void write_WSM(u8_t R, u8_t G, u8_t B, u32_t adress) {
//   u8_t opcode = 0b0001;

//   u32_t dataA = adress << 4 | opcode;
//   u32_t dataB = B << 6 | G << 3 | R;

//   char dataA_str[32];
//   char dataB_str[32];
//   char instruction[64];

//   itoa(dataA, dataA_str, 2);
//   itoa(dataB, dataB_str, 2);
//   strcat(instruction, dataA_str);
//   strcat(instruction, dataB_str);

//   u16_t fd = open(DEVICE_PATH, O_RDWR);

//   if (fd == -1) {
//     perror("Falha ao abrir o dispositivo");
//     exit(EXIT_FAILURE);
//   }

//   write_data(fd, (u64_t)instruction);
//   close(fd);
// }

// void write_WBR_sprite(u8_t ativo, u16_t x, u16_t y, u16_t offset, u8_t reg) {
//   u8_t opcode = 0b0000;

//   u32_t dataA = reg << 6 | opcode;
//   u32_t dataB = ativo << 29 | x << 19 | y << 9 | offset;

//   char dataA_str[32];
//   char dataB_str[32];
//   char instruction[64];

//   itoa(dataA, dataA_str, 2);
//   itoa(dataB, dataB_str, 2);
//   strcat(instruction, dataA_str);
//   strcat(instruction, dataB_str);

//   u16_t fd = open(DEVICE_PATH, O_RDWR);

//   if (fd == -1) {
//     perror("Falha ao abrir o dispositivo");
//     exit(EXIT_FAILURE);
//   }

//   write_data(fd, (u64_t)instruction);
//   close(fd);
// }

// void write_WBR_background(u8_t R, u8_t G, u8_t B) {
//   u8_t opcode = 0b0000;
//   u8_t reg = 0b00000;

//   u32_t dataA = reg << 6 | opcode;
//   u32_t dataB = B << 6 | G << 3 | R;

//   char dataA_str[32];
//   char dataB_str[32];
//   char instruction[64];

//   itoa(dataA, dataA_str, 2);
//   itoa(dataB, dataB_str, 2);
//   strcat(instruction, dataA_str);
//   strcat(instruction, dataB_str);

//   itoa(dataA, dataA_str, 2);
//   itoa(dataB, dataB_str, 2);
//   strcat(instruction, dataA_str);
//   strcat(instruction, dataB_str);

//   u16_t fd = open(DEVICE_PATH, O_RDWR);

//   if (fd == -1) {
//     perror("Falha ao abrir o dispositivo");
//     exit(EXIT_FAILURE);
//   }

//   write_data(fd, (u64_t)instruction);
//   close(fd);
// }

void write_data(u64_t fd, u64_t data) {
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

// sprite_fixed_t init_fixed_sprite(u8_t reg, u8_t offset) {
//   sprite_fixed_t sprite;
//   sprite.coord_x = 0;
//   sprite.coord_y = 0;
//   sprite.offset = offset;
//   sprite.data_register = reg;
//   sprite.ativo = 0;
//   return sprite;
// }

// void view_sprite(void *sprite_ptr, sprite_type_t type) {
//   switch (type) {
//     case SPRITE_NORMAL: {
//       sprite_t *sprite = (sprite_t *)sprite_ptr;
//       sprite->ativo = 1;
//       break;
//     }
//     case SPRITE_FIXED: {
//       sprite_fixed_t *fixed_sprite = (sprite_fixed_t *)sprite_ptr;
//       fixed_sprite->ativo = 1;
//       break;
//     }
//     default:
//       break;
//   }
// }

// void hide_sprite(void *sprite_ptr, sprite_type_t type) {
//   switch (type) {
//     case SPRITE_NORMAL: {
//       sprite_t *sprite = (sprite_t *)sprite_ptr;
//       sprite->ativo = 0;
//       break;
//     }
//     case SPRITE_FIXED: {
//       sprite_fixed_t *fixed_sprite = (sprite_fixed_t *)sprite_ptr;
//       fixed_sprite->ativo = 0;
//       break;
//     }
//     default:
//       break;
//   }
// }

// u8_t set_fixed_sprite(sprite_fixed_t *sprite_ptr, u8_t x, u8_t y) {
//   sprite_ptr->coord_x = x;
//   sprite_ptr->coord_y = y;
// }

u64_t set_background_block(u64_t column, u64_t line, u8_t R, u8_t G, u8_t B) {
  u8_t opcode = 0010;
  u64_t memory_address = column * line;

  u32_t dataA = memory_address << 27 | opcode << 23;
  u32_t dataB = B << 6 | G << 3 | R;

  char dataA_str[33];
  char dataB_str[33];
  char instruction_str[65];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcpy(instruction_str, dataA_str);
  strcat(instruction_str, dataB_str);

  u64_t instruction = strtoull(instruction_str, NULL, 2);

  return instruction;
}

u64_t set_sprite(u8_t reg, u8_t sp, u16_t x, u16_t y, u16_t offset) {
  u8_t opcode = 0b0000;

  u32_t dataA = reg << 6 | opcode << 2;
  u32_t dataB = sp << 29 | x << 19 | y << 9 | offset;

  char dataA_str[33];
  char dataB_str[33];
  char instruction_str[65];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcpy(instruction_str, dataA_str);
  strcat(instruction_str, dataB_str);

  u64_t instruction = strtoull(instruction_str, NULL, 2);

  return instruction;
}

u64_t set_background_color(u8_t R, u8_t G, u8_t B) {
  u8_t opcode = 0b0000;
  u8_t reg = 0b00000;

  u32_t dataA = reg << 4 | opcode;
  u32_t dataB = B << 6 | G << 3 | R;

  char dataA_str[33];
  char dataB_str[33];
  char instruction_str[65];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcpy(instruction_str, dataA_str);
  strcat(instruction_str, dataB_str);

  u64_t instruction = strtoull(instruction_str, NULL, 2);

  return instruction;
}

u64_t set_polygon(int reg, int format, int R, int G, int B, int size, int ref_point_y, int ref_point_x) {
  u8_t opcode = 0b0011;

  u32_t dataA = reg << 4 | opcode;  
  u32_t dataB = format << 31 | R << 28 | G << 25 | R << 22 | size << 18 | ref_point_y << 9 | ref_point_x;

  char dataA_str[33];
  char dataB_str[33];
  char instruction_str[65];

  itoa(dataA, dataA_str, 2);
  itoa(dataB, dataB_str, 2);
  strcpy(instruction_str, dataA_str);
  strcat(instruction_str, dataB_str);

  printf("\n%s\n", instruction_str);


  u64_t instruction = strtoull(instruction_str, NULL, 2);

  printf("\n%X\n", instruction);


  return instruction;
}

void increase_coordinate(sprite_t *sp, int mirror) {}

int collision(sprite_t *sp1, sprite_t *sp2) {}
