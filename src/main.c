#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

int main(void) {

  //BACKGROUND
  u64_t instruction1 = set_background_color(4, 5, 6);

  //BACKGROUND BLOCKS
  ground_block_t block;
  block.address = 3000;
  block.R = 1;
  block.G = 4;
  block.B = 1;

  int i = 3600;

  for (i; i < 4800; i++) {
    block.address = i;
    set_background_block(block);
  }

  // //SPRITE SPACESHIP
  // sprite_fixed_t spaceship_1;
  // spaceship_1.ativo = 0;
  // spaceship_1.data_register = 1;
  // spaceship_1.coord_x = 190;
  // spaceship_1.coord_y = 190;
  // spaceship_1.offset = 5;

  // set_fixed_sprite(spaceship_1);

  // //SPRITE SPACESHIP
  // sprite_fixed_t spaceship_2;
  // spaceship_2.ativo = 0;
  // spaceship_2.data_register = 2;
  // spaceship_2.coord_x = 50;
  // spaceship_2.coord_y = 50;
  // spaceship_2.offset = 5;

  // set_fixed_sprite(spaceship_2);

  // //SPRITE ASTEROIDES
  // sprite_fixed_t asteroid_1;
  // spaceship_2.ativo = 0;
  // spaceship_2.data_register = 3;
  // spaceship_2.coord_x = 250;
  // spaceship_2.coord_y = 250;
  // spaceship_2.offset = 23;

  // set_fixed_sprite(spaceship_2);

  //BASE DO CASTELO
  polygon_t polygon_1;
  polygon_1.data_register = 14;
  polygon_1.type = 0;
  polygon_1.size = 14;
  polygon_1.ref_point_x = 450;
  polygon_1.ref_point_y = 320;
  polygon_1.R = 5;
  polygon_1.G = 3;
  polygon_1.B = 4;

  set_polygon(polygon_1);

  //TORRES DO CASTELO

  //TORRE 1
  polygon_t pol_tower_1;
  pol_tower_1.data_register = 5;
  pol_tower_1.type = 0;
  pol_tower_1.size = 2;
  pol_tower_1.ref_point_x = 390;
  pol_tower_1.ref_point_y = 235;
  pol_tower_1.R = 5;
  pol_tower_1.G = 3;
  pol_tower_1.B = 4;

  set_polygon(pol_tower_1);

  polygon_t pol_tower_11;
  pol_tower_11.data_register = 6;
  pol_tower_11.type = 0;
  pol_tower_11.size = 2;
  pol_tower_11.ref_point_x = 390;
  pol_tower_11.ref_point_y = 220;
  pol_tower_11.R = 5;
  pol_tower_11.G = 3;
  pol_tower_11.B = 4;

  set_polygon(pol_tower_11);

  //TORRE 2
  polygon_t pol_tower_2;
  pol_tower_2.data_register = 7;
  pol_tower_2.type = 0;
  pol_tower_2.size = 2;
  pol_tower_2.ref_point_x = 510;
  pol_tower_2.ref_point_y = 235;
  pol_tower_2.R = 5;
  pol_tower_2.G = 3;
  pol_tower_2.B = 4;

  set_polygon(pol_tower_2);

  polygon_t pol_tower_22;
  pol_tower_22.data_register = 8;
  pol_tower_22.type = 0;
  pol_tower_22.size = 2;
  pol_tower_22.ref_point_x = 510;
  pol_tower_22.ref_point_y = 220;
  pol_tower_22.R = 5;
  pol_tower_22.G = 3;
  pol_tower_22.B = 4;

  set_polygon(pol_tower_22);

  //TORRE 3
  polygon_t pol_tower_3;
  pol_tower_3.data_register = 9;
  pol_tower_3.type = 0;
  pol_tower_3.size = 2;
  pol_tower_3.ref_point_x = 450;
  pol_tower_3.ref_point_y = 235;
  pol_tower_3.R = 5;
  pol_tower_3.G = 3;
  pol_tower_3.B = 4;

  set_polygon(pol_tower_3);

  polygon_t pol_tower_33;
  pol_tower_33.data_register = 10;
  pol_tower_33.type = 0;
  pol_tower_33.size = 2;
  pol_tower_33.ref_point_x = 450;
  pol_tower_33.ref_point_y = 210;
  pol_tower_33.R = 5;
  pol_tower_33.G = 3;
  pol_tower_33.B = 4;

  set_polygon(pol_tower_33);

  //TELHADOS
  polygon_t pol_tel_1;
  pol_tel_1.data_register = 11;
  pol_tel_1.type = 1;
  pol_tel_1.size = 3;
  pol_tel_1.ref_point_x = 390;
  pol_tel_1.ref_point_y = 185;
  pol_tel_1.R = 3;
  pol_tel_1.G = 1;
  pol_tel_1.B = 1;

  set_polygon(pol_tel_1);

  polygon_t pol_tel_2;
  pol_tel_2.data_register = 12;
  pol_tel_2.type = 1;
  pol_tel_2.size = 3;
  pol_tel_2.ref_point_x = 510;
  pol_tel_2.ref_point_y = 185;
  pol_tel_2.R = 3;
  pol_tel_2.G = 1;
  pol_tel_2.B = 1;

  set_polygon(pol_tel_2);

  polygon_t pol_tel_3;
  pol_tel_3.data_register = 13;
  pol_tel_3.type = 1;
  pol_tel_3.size = 5;
  pol_tel_3.ref_point_x = 450;
  pol_tel_3.ref_point_y = 165;
  pol_tel_3.R = 3;
  pol_tel_3.G = 1;
  pol_tel_3.B = 1;

  set_polygon(pol_tel_3);

  //PORTA DO CASTELO
  polygon_t pol_door;
  pol_door.data_register = 4;
  pol_door.type = 0;
  pol_door.size = 5;
  pol_door.ref_point_x = 450;
  pol_door.ref_point_y = 365;
  pol_door.R = 3;
  pol_door.G = 1;
  pol_door.B = 1;

  set_polygon(pol_door);

  return 0;
}
