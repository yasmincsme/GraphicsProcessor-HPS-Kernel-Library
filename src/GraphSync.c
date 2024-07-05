#include "GraphSync.h"

#include <fcntl.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/gpp_data_bus" /**< Path to device data bus */
#define FROG "frog"
#define A_font "a-font"
#define C_font "c-font"
#define H_font "h-font"
#define K_font "k-font"
#define O_font "o-font"
#define R_font "r-font"
#define S_font "s-font"
#define U_font "u-font"

static int device_fd = -1;

void write_data(u64_t data) {
  device_fd = open(DEVICE_PATH, O_WRONLY | O_CREAT | O_TRUNC);

  if (device_fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  ssize_t result = write(device_fd, &data, sizeof(data));

  if (result == -1) {
    perror("Falha na escrita do dispositivo");
    exit(EXIT_FAILURE);
  }
}

u64_t read_data() {
  u64_t data;
  ssize_t result = read(device_fd, &data, sizeof(data));

  if (result == -1) {
    perror("Falha na leitura do dispositivo");
    exit(EXIT_FAILURE);
  }

  return data;
}

u8_t close_data() {
  ssize_t result = close(device_fd);

  if (result == -1) {
    perror("Falha no encerramento do dispositivo\n");
    return -1;
  }
  return 0;
}

u64_t set_background_block(ground_block_t block) {
  u8_t opcode = 0b0010;
  u64_t memory_address = block.address;
  u32_t dataA = memory_address << 4 | opcode;
  u32_t dataB = block.B << 6 | block.G << 3 | block.R;

  char instruction_str[65];

  instruction_str[64] = '\0';

  snprintf(instruction_str, sizeof(instruction_str), "%x%08x", dataA, dataB);

  u64_t instruction = strtoull(instruction_str, NULL, 16);

  write_data(instruction);
  close_data(device_fd);

  return instruction;
}

u64_t set_fixed_sprite(sprite_fixed_t sprite) {
  u8_t opcode = 0b0000;

  u32_t dataA = sprite.data_register << 4 | opcode;
  u32_t dataB = sprite.ativo << 29 | sprite.coord_x << 19 | sprite.coord_y << 9 | sprite.offset;

  char instruction_str[65];

  snprintf(instruction_str, sizeof(instruction_str), "%x%08x", dataA, dataB);

  u64_t instruction = strtoull(instruction_str, NULL, 16);

  write_data(instruction);
  close_data(device_fd);

  return instruction;
}

u64_t set_dynamic_sprite(sprite_t sprite) {
  u8_t opcode = 0b0000;

  u32_t dataA = sprite.data_register << 4 | opcode;
  u32_t dataB = sprite.ativo << 29 | sprite.coord_x << 19 | sprite.coord_y << 9 | sprite.offset;

  char instruction_str[65];

  instruction_str[64] = '\0';

  snprintf(instruction_str, sizeof(instruction_str), "%x%08x", dataA, dataB);

  u64_t instruction = strtoull(instruction_str, NULL, 16);

  write_data(instruction);
  close_data(device_fd);

  return instruction;
}

u64_t set_background_color(u32_t R, u32_t G, u32_t B) {
  u8_t opcode = 0b0000;
  u8_t reg = 0b00000;

  u32_t dataA = reg << 4 | opcode;
  u32_t dataB = B << 6 | G << 3 | R;

  char instruction_str[65];

  instruction_str[64] = '\0';

  snprintf(instruction_str, sizeof(instruction_str), "%x%08x", dataA, dataB);

  u64_t instruction = strtoull(instruction_str, NULL, 16);

  write_data(instruction);
  close_data(device_fd);

  return instruction;
}

u64_t set_polygon(polygon_t polygon) {
  u8_t opcode = 0b0011;

  u32_t dataA = polygon.data_register << 4 | opcode;
  u32_t dataB = polygon.type << 31 | polygon.B << 28 | polygon.G << 25 | polygon.R << 22 | polygon.size << 18 |
                polygon.ref_point_y << 9 | polygon.ref_point_x;

  char instruction_str[65];

  instruction_str[64] = '\0';

  snprintf(instruction_str, sizeof(instruction_str), "%x%08x", dataA, dataB);

  u64_t instruction = strtoull(instruction_str, NULL, 16);

  write_data(instruction);
  close_data(device_fd);

  return instruction;
}

void clean_sprite() {
  sprite_fixed_t sprite;
  sprite.ativo = 0;
  sprite.coord_x = 1;
  sprite.coord_y = 1;
  sprite.offset = 1;

  u8_t i;
  for (i = 1; i <= 32; ++i) {
    sprite.data_register = i;
    set_fixed_sprite(sprite);
  }
}

void clean_polygon() {
  polygon_t null_polygon;
  null_polygon.type = 0;
  null_polygon.size = 0;
  null_polygon.ref_point_x = 50;
  null_polygon.ref_point_y = 50;
  null_polygon.R = 5;
  null_polygon.G = 3;
  null_polygon.B = 4;

  u8_t i;
  for (i = 1; i <= 32; ++i) {
    null_polygon.data_register = i;
    set_polygon(null_polygon);
  }
}

void clean_background() {
  ground_block_t null_block;
  null_block.R = 6;
  null_block.G = 7;
  null_block.B = 7;

  u64_t i;
  for (i = 0; i < 4800; ++i) {
    null_block.address = i;
    set_background_block(null_block);
  }
}

sprite_fixed_t set_new_sprite(u16_t sprite_offset) {
  u32_t dataA, dataB;

  char instruction_str[65];
  instruction_str[64] = '\0';

  u8_t opcode = 0b0001;
  u16_t init_addr = sprite_offset * 400;

  // Cores da imagem para a sprite
  string sprite_name = FROG;
  u8_t pixel_data[400][3] = select_sprite(sprite_name);

  u16_t max_addr = init_addr + 400;
  u16_t addr;

  for (addr = init_addr; addr < max_addr; addr++) {
    u16_t pixel_idx = addr - init_addr;
    u8_t R = pixel_data[pixel_idx][0];
    u8_t G = pixel_data[pixel_idx][1];
    u8_t B = pixel_data[pixel_idx][2];

    dataA = addr << 4 | opcode;
    dataB = B << 6 | G << 3 | R;

    snprintf(instruction_str, sizeof(instruction_str), "%x%08x", dataA, dataB);

    u64_t instruction = strtoull(instruction_str, NULL, 16);

    write_data(instruction);
  }

  sprite_fixed_t new_sprite;
  new_sprite.offset = sprite_offset;

  close_data(device_fd);

  return new_sprite;
}

u8_t select_sprite(u8_t sprite_name) {
  u8_t pixel_data[400][3];

  switch (sprite_name) {
    case FROG:
      pixel_data = {
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {3, 7, 4}, {3, 7, 4}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {3, 7, 4}, {3, 7, 4}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {3, 7, 4}, {7, 7, 7}, {7, 7, 7}, {3, 7, 4}, {6, 7, 7},
          {6, 7, 7}, {3, 7, 4}, {7, 7, 7}, {7, 7, 7}, {3, 7, 4}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {7, 7, 7}, {6, 7, 7}, {6, 7, 7}, {7, 7, 7}, {3, 7, 4},
          {3, 7, 4}, {7, 7, 7}, {6, 7, 7}, {6, 7, 7}, {7, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {7, 7, 7}, {6, 7, 7}, {6, 7, 7}, {7, 7, 7}, {1, 5, 2},
          {1, 5, 2}, {7, 7, 7}, {6, 7, 7}, {6, 7, 7}, {7, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {2, 5, 3}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2},
          {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {2, 5, 3}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2},
          {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2},
          {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {5, 6, 1}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {5, 6, 1}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {5, 7, 0}, {5, 6, 1}, {5, 6, 1}, {5, 6, 1},
          {5, 6, 1}, {5, 6, 1}, {5, 6, 1}, {5, 7, 0}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {5, 7, 0}, {5, 7, 0}, {5, 7, 0}, {5, 7, 0},
          {5, 7, 0}, {5, 7, 0}, {5, 7, 0}, {5, 7, 0}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {3, 5, 0}, {5, 7, 0}, {5, 7, 0}, {5, 7, 0},
          {5, 7, 0}, {5, 7, 0}, {5, 7, 0}, {3, 5, 0}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {2, 5, 3}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {3, 5, 0}, {5, 7, 0}, {5, 7, 0},
          {5, 7, 0}, {5, 7, 0}, {3, 5, 0}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {2, 5, 3}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {2, 5, 3}, {2, 5, 3}, {2, 5, 3}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {3, 5, 0}, {5, 7, 0}, {5, 7, 0},
          {5, 7, 0}, {5, 7, 0}, {3, 5, 0}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {2, 5, 3}, {2, 5, 3}, {2, 5, 3}, {6, 7, 7},
          {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {3, 5, 0}, {5, 7, 0}, {5, 7, 0},
          {5, 7, 0}, {5, 7, 0}, {3, 5, 0}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7},
          {6, 7, 7}, {0, 3, 1}, {0, 3, 1}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {3, 5, 0}, {5, 7, 0},
          {5, 7, 0}, {3, 5, 0}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {0, 3, 1}, {0, 3, 1}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {3, 5, 0},
          {3, 5, 0}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2},
          {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {1, 5, 2}, {6, 7, 7}, {1, 5, 2}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {0, 2, 1}, {0, 3, 1}, {0, 3, 1}, {6, 7, 7}, {6, 7, 7}, {0, 3, 1}, {6, 7, 7}, {0, 3, 1},
          {0, 3, 1}, {6, 7, 7}, {0, 3, 1}, {6, 7, 7}, {6, 7, 7}, {0, 3, 1}, {0, 3, 1}, {0, 2, 1}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}};
      break;

    case A_font:
      pixel_data = {
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {5, 4, 0}, {5, 4, 0}, {5, 4, 0}, {5, 4, 0}, {5, 4, 0},
          {5, 4, 0}, {5, 4, 0}, {5, 4, 0}, {5, 4, 0}, {5, 4, 0}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {5, 4, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0},
          {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {5, 4, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0},
          {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {5, 4, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 0, 1},
          {7, 0, 1}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {5, 4, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 0, 1}, {7, 0, 1},
          {7, 0, 1}, {7, 0, 1}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 4, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 0, 1}, {7, 0, 1},
          {7, 0, 1}, {7, 0, 1}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 4, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 0, 1}, {5, 0, 0},
          {5, 0, 0}, {7, 0, 1}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {7, 6, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 4, 3}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 0, 1}, {6, 7, 7},
          {6, 7, 7}, {7, 0, 1}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {6, 7, 7},
          {6, 7, 7}, {5, 4, 3}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4},
          {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {7, 7, 4}, {6, 7, 7},
          {6, 7, 7}, {5, 2, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0},
          {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 2, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0},
          {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 2, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 0, 1}, {7, 0, 1},
          {7, 0, 1}, {7, 0, 1}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 2, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 0, 1}, {7, 0, 1},
          {7, 0, 1}, {7, 0, 1}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 2, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {5, 0, 0}, {5, 0, 0},
          {5, 0, 0}, {5, 0, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {7, 3, 0}, {6, 7, 7},
          {6, 7, 7}, {5, 0, 0}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {6, 7, 7},
          {6, 7, 7}, {5, 0, 0}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {6, 7, 7},
          {6, 7, 7}, {5, 0, 0}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {7, 0, 1}, {6, 7, 7},
          {6, 7, 7}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {5, 0, 0}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7},
          {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}, {6, 7, 7}};
      break;

    case C_font:
      pixel_data = ;
      break;

    default:
      break;
  }
}