#include <stdio.h>

typedef struct sprite_t sprite_t;

struct sprite_t {
  int coord_x, coord_y;
  int direction, offset, data_register;
  int step_x, step_y;
  int ativo, collision;
};

typedef struct sprite_fixed_t sprite_fixed_t;

struct sprite_fixed_t {
  int coord_x, coord_y, offset;
  int data_register, ativo;
};

int set_sprite(int registrador, int x, int y, int offset, int activation_bit);

int set_background_block(int column, int line, int R, int G, int B);

int set_background_color(int R, int G, int B);

void increase_coordinate(sprite_t *sp, int mirror);

int collision(sprite_t *sp1, sprite_t *sp2);