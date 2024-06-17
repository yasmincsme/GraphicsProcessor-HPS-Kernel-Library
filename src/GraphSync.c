#include "GraphSync.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils/aux.h"

#define DEVICE_PATH "/dev/gpp_data_bus"

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

u64_t set_background_block(ground_block_t block) {
  u8_t opcode = 0010;
  u64_t memory_address = block.column * block.line;

  u32_t dataA = memory_address << 27 | opcode << 23;
  u32_t dataB = block.B << 6 | block.G << 3 | block.R;

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

u64_t set_fixed_sprite(sprite_fixed_t sprite) {
  u8_t opcode = 0b0000;

  u32_t dataA = sprite.data_register << 6 | opcode << 2;
  u32_t dataB = sprite.ativo << 29 | sprite.coord_x << 19 | sprite.coord_y << 9 | sprite.offset;

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

u64_t set_dynamic_sprite(sprite_t sprite) {
  u8_t opcode = 0b0000;

  u32_t dataA = sprite.data_register << 6 | opcode << 2;
  u32_t dataB = sprite.ativo << 29 | sprite.coord_x << 19 | sprite.coord_y << 9 | sprite.offset;

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

u64_t set_polygon(polygon_t polygon) {
  u8_t opcode = 0b0011;

  u32_t dataA = polygon.data_register << 4 | opcode;
  u32_t dataB = polygon.type << 31 | polygon.B << 28 | polygon.G << 25 | polygon.R << 22 | polygon.size << 18 |
                polygon.ref_point_y << 9 | polygon.ref_point_x;

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
