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
