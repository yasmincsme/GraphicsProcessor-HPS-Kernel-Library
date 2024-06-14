#include "GraphSync.h"

#include <stdio.h>
#include <unistd.h>

long long int set_sprite(long long int reg, int x, int y, int offset, int activation_bit) {
  long long int instruction = reg << 36 | activation_bit << 30 | x << 29 | y << 19 | offset;

  return instruction
}

int set_background_block(int column, int line, int R, int G, int B) {}

long long int set_background_color(int R, int G, int B) {
  long long int instruction = B << 6 | G << 3 | R;

  return instruction;
}

void increase_coordinate(sprite_t *sp, int mirror) {}

int collision(sprite_t *sp1, sprite_t *sp2) {}
