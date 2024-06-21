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

#include "utils/aux.h"

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

  u8_t i;
  for (i = 0; i <= 32; ++i) {
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
  for (i = 0; i <= 32; ++i) {
    null_polygon.data_register = i;
    set_polygon(null_polygon);
  }
}

void clean_background() {
  ground_block_t null_block;
  null_block.R = 1;
  null_block.G = 1;
  null_block.B = 1;

  u8_t i;
  for (i = 0; i < 4800; ++i) {
    null_block.address = i;
    set_background_block(null_block);
  }
}

// u8_t increase_coordinate_sprite(sprite_t *sprite, int mirror) {
//   u64_t angle_rad = sprite->direction * M_PI() / 180.0;

//   int delta_x = (int)round(sprite->step_x * cos(angle_rad));
//   int delta_y = (int)round(sprite->step_y * sin(angle_rad));

//   if (sprite->coord_x < 0 || sprite->coord_x > 639 || sprite->coord_y < 0 || sprite->coord_y > 479) {
//     if (mirror) {
//       // Espelhar as coordenadas do sprite
//       sprite->coord_x = clamp(sprite->coord_x, 0, 639);
//       sprite->coord_y = clamp(sprite->coord_y, 0, 479);
//     }
//     return -1;
//   }

//   sprite->coord_x += delta_x;
//   sprite->coord_y += delta_y;

//   sprite_t sprite_copy = *sprite;
//   set_dynamic_sprite(sprite_copy);

//   return 0;
// }
