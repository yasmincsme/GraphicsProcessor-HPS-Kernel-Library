#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

int main(void) {

  u64_t instruction1 = set_background_color(7, 5, 1);

  sprite_fixed_t sprite;
  sprite.ativo = 0;
  sprite.data_register = 2;
  sprite.coord_x = 100;
  sprite.coord_y = 100;
  sprite.offset = 18;

  set_fixed_sprite(sprite);

  ground_block_t block;
  block.column = 2;
  block.line = 2;
  block.R = 1;
  block.G = 1;
  block.B = 7; //677

  //set_background_block(block);

  int j, i = 0;

  for (i; i<80; i++) {
    for(j; j<30; j++) {
      block.column = j;
      block.line = i;
      set_background_block(block);
    }
  }

    // // printf("\n%x\n", instruction1);

    // u64_t instruction2 =
    //     set_polygon(2, 1, 7, 2, 7, 2, 40,
    //                 40);  // int reg, int format, int R, int G, int B, int size, int ref_point_y, int ref_point_x
    // printf("\n%x\n", instruction2);

    // u64_t instruction3 =
    //     set_sprite(6, 1, 100, 100, 4);  // u64_t set_sprite(u8_t reg, u8_t sp, u16_t x, u16_t y, u16_t offset)
    // printf("\n%x\n", instruction3);

    // uint64_t write_value_background = set_background_color(5, 2, 3);
    // uint64_t write_value_sprite = set_sprite(2, 10, 10, 1, 1);
    // uint64_t write_value_background_block = set_background_block(10, 10, 2, 2, 7);
    // uint64_t write_value_polygon = set_polygon(3, 1, 4, 5, 2, 5, 60, 60);

    return 0;
}
