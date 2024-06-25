#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

int main(void) {
  u16_t offset = 25;
  sprite_fixed_t new_sprite = set_new_sprite(offset);

  new_sprite.ativo = 1;
  new_sprite.coord_x = 10;
  new_sprite.coord_y = 10;
  new_sprite.data_register = 2;

  set_fixed_sprite(new_sprite);

  return 0;
}
