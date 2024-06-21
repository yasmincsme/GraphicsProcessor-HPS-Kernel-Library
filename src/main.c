#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

int main(void) {
  // BACKGROUND
  u64_t instruction1 = set_background_color(2, 2, 3);

  // BACKGROUND BLOCKS
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

  // ÁRVORES
  sprite_fixed_t sprite_tree1;
  sprite_tree1.ativo = 1;
  sprite_tree1.data_register = 1;
  sprite_tree1.offset = 4;
  sprite_tree1.coord_x = 100;
  sprite_tree1.coord_y = 400;

  set_fixed_sprite(sprite_tree1);

  sprite_fixed_t sprite_tree2;
  sprite_tree2.ativo = 1;
  sprite_tree2.data_register = 2;
  sprite_tree2.offset = 4;
  sprite_tree2.coord_x = 150;
  sprite_tree2.coord_y = 350;

  set_fixed_sprite(sprite_tree2);

  sprite_fixed_t sprite_tree3;
  sprite_tree3.ativo = 1;
  sprite_tree3.data_register = 3;
  sprite_tree3.offset = 4;
  sprite_tree3.coord_x = 200;
  sprite_tree3.coord_y = 450;

  set_fixed_sprite(sprite_tree3);

  sprite_fixed_t sprite_tree4;
  sprite_tree4.ativo = 1;
  sprite_tree4.data_register = 4;
  sprite_tree4.offset = 4;
  sprite_tree4.coord_x = 320;
  sprite_tree4.coord_y = 460;

  set_fixed_sprite(sprite_tree4);

  sprite_fixed_t sprite_tree5;
  sprite_tree5.ativo = 1;
  sprite_tree5.data_register = 5;
  sprite_tree5.offset = 4;
  sprite_tree5.coord_x = 360;
  sprite_tree5.coord_y = 400;

  set_fixed_sprite(sprite_tree5);

  sprite_fixed_t sprite_tree6;
  sprite_tree6.ativo = 1;
  sprite_tree6.data_register = 6;
  sprite_tree6.offset = 4;
  sprite_tree6.coord_x = 230;
  sprite_tree6.coord_y = 400;

  set_fixed_sprite(sprite_tree6);

  sprite_fixed_t sprite_tree7;
  sprite_tree7.ativo = 1;
  sprite_tree7.data_register = 7;
  sprite_tree7.offset = 4;
  sprite_tree7.coord_x = 295;
  sprite_tree7.coord_y = 350;

  set_fixed_sprite(sprite_tree7);

  sprite_fixed_t sprite_tree8;
  sprite_tree8.ativo = 1;
  sprite_tree8.data_register = 8;
  sprite_tree8.offset = 4;
  sprite_tree8.coord_x = 30;
  sprite_tree8.coord_y = 355;

  set_fixed_sprite(sprite_tree8);

  sprite_fixed_t sprite_tree9;
  sprite_tree9.ativo = 1;
  sprite_tree9.data_register = 9;
  sprite_tree9.offset = 4;
  sprite_tree9.coord_x = 40;
  sprite_tree9.coord_y = 450;

  set_fixed_sprite(sprite_tree9);

  sprite_fixed_t sprite_tree10;
  sprite_tree10.ativo = 1;
  sprite_tree10.data_register = 10;
  sprite_tree10.offset = 4;
  sprite_tree10.coord_x = 500;
  sprite_tree10.coord_y = 450;

  set_fixed_sprite(sprite_tree10);

  sprite_fixed_t sprite_tree11;
  sprite_tree11.ativo = 1;
  sprite_tree11.data_register = 11;
  sprite_tree11.offset = 4;
  sprite_tree11.coord_x = 550;
  sprite_tree11.coord_y = 350;

  set_fixed_sprite(sprite_tree11);

  // BASE DO CASTELO
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

  // TORRES DO CASTELO

  // TORRE 1
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

  // TORRE 2
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

  // TORRE 3
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

  // TELHADOS
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

  // PORTA DO CASTELO
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

  // SOL
  polygon_t pol_sun;
  pol_sun.data_register = 1;
  pol_sun.type = 0;
  pol_sun.size = 5;
  pol_sun.ref_point_x = 80;
  pol_sun.ref_point_y = 80;
  pol_sun.R = 7;
  pol_sun.G = 5;
  pol_sun.B = 0;

  set_polygon(pol_sun);

  // NÚVENS
  ground_block_t block_cloud;
  block_cloud.R = 7;
  block_cloud.G = 7;
  block_cloud.B = 7;

  block_cloud.address = 500;
  set_background_block(block_cloud);
  block_cloud.address = 501;
  set_background_block(block_cloud);
  block_cloud.address = 502;
  set_background_block(block_cloud);
  block_cloud.address = 503;
  set_background_block(block_cloud);

  block_cloud.address = 579;
  set_background_block(block_cloud);
  block_cloud.address = 580;
  set_background_block(block_cloud);
  block_cloud.address = 581;
  set_background_block(block_cloud);
  block_cloud.address = 582;
  set_background_block(block_cloud);
  block_cloud.address = 583;
  set_background_block(block_cloud);
  block_cloud.address = 584;
  set_background_block(block_cloud);

  block_cloud.address = 905;
  set_background_block(block_cloud);
  block_cloud.address = 906;
  set_background_block(block_cloud);
  block_cloud.address = 907;
  set_background_block(block_cloud);
  block_cloud.address = 908;
  set_background_block(block_cloud);

  block_cloud.address = 984;
  set_background_block(block_cloud);
  block_cloud.address = 985;
  set_background_block(block_cloud);
  block_cloud.address = 986;
  set_background_block(block_cloud);
  block_cloud.address = 987;
  set_background_block(block_cloud);
  block_cloud.address = 988;
  set_background_block(block_cloud);
  block_cloud.address = 989;
  set_background_block(block_cloud);

  return 0;
}
