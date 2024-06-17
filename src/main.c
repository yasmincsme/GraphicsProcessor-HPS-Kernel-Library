#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

#define DEVICE_PATH "/dev/gpp_data_bus"

int main(void) {
  u16_t fd = open(DEVICE_PATH, O_RDWR);

  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  u64_t instruction1 =  set_background_color(1, 1, 1);
  //printf("\n%x\n", instruction1);

  u64_t instruction2 = set_polygon(2, 1, 7, 2, 7, 2, 40, 40); //int reg, int format, int R, int G, int B, int size, int ref_point_y, int ref_point_x
  printf("\n%x\n", instruction2);

  u64_t instruction3 = set_sprite(6, 1, 100, 100, 4); //u64_t set_sprite(u8_t reg, u8_t sp, u16_t x, u16_t y, u16_t offset) 
  printf("\n%x\n", instruction3);

  write_data(fd, instruction1);
  write_data(fd, instruction2);
  write_data(fd, instruction3);
  close(fd);

  // uint64_t write_value_background = set_background_color(5, 2, 3);
  // uint64_t write_value_sprite = set_sprite(2, 10, 10, 1, 1);
  // uint64_t write_value_background_block = set_background_block(10, 10, 2, 2, 7);
  // uint64_t write_value_polygon = set_polygon(3, 1, 4, 5, 2, 5, 60, 60);

  // Escreve um valor no dispositivo
  // write_data(fd, write_value_background);
  // write_data(fd, write_value_sprite);

  // Fecha o dispositivo
  // close(fd);

  return 0;
}
