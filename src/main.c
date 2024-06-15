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
  int fd;

  // Abre o dispositivo
  fd = open(DEVICE_PATH, O_RDWR);
  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  uint64_t write_value_background = set_background_color(5, 2, 3);
  uint64_t write_value_sprite = set_sprite(2, 10, 10, 1, 1);
  uint64_t write_value_background_block = set_background_block(10, 10, 2, 2, 7);
  uint64_t write_value_polygon = set_polygon(3, 1, 4, 5, 2, 5, 60, 60);

  // Escreve um valor no dispositivo
  write_data(fd, write_value_background);
  write_data(fd, write_value_sprite);

  // Fecha o dispositivo
  close(fd);

  return 0;
}