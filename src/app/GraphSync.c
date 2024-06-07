#include "GraphSync.h"

#include <stdio.h>

int set_sprite(int registrador, int x, int y, int offset, int activation_bit) {
  __uint32_t dataA = (registrador << 4) | (0000);
  __uint32_t dataB = (activation_bit << 29) | (x << 19) | (y << 9) | offset;
  printf("%x, %x\n", dataA, dataB);
  __uint64_t instruction = (dataA << 32) | (dataB);
  printf("%x\n", instruction);

  return instruction;
}

int set_background_block(int column, int line, int R, int G, int B) {}

int set_background_color(int R, int G, int B) {}

void increase_coordinate(sprite_t *sp, int mirror) {}

int collision(sprite_t *sp1, sprite_t *sp2) {}
