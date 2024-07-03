#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

int fd;

int main(void) {
  struct input_event ev;

  fd = open("/dev/input/event0", O_RDONLY);
  if (fd == -1) {
    perror("Erro ao abrir o dispositivo de entrada");
    exit(EXIT_FAILURE);
  }

  set_background_color(1, 1, 1);

  u16_t offset = 25;
  sprite_fixed_t new_sprite = set_new_sprite(offset);

  u16_t center_x = 320;
  u16_t center_y = 10;

  new_sprite.ativo = 1;
  new_sprite.coord_x = center_x;
  new_sprite.coord_y = center_y;
  new_sprite.data_register = 2;

  set_fixed_sprite(new_sprite);

  while (1) {
    ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
    if (bytes < sizeof(struct input_event)) {
      perror("Erro ao ler evento");
      exit(EXIT_FAILURE);
    }

    // Atualiza a posição do cursor baseado no movimento do mouse
    if (ev.type == EV_REL && ev.code == REL_X) {
      new_sprite.coord_x += ev.value;
    } else if (ev.type == EV_REL && ev.code == REL_Y) {
      // new_sprite.coord_y += ev.value;
    }

    set_fixed_sprite(new_sprite);
  }

  return 0;
}
