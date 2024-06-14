#include "GraphSync.h"

#include <stdio.h>
#include <unistd.h>

int set_sprite(int registrador, int x, int y, int offset, int activation_bit) {
  long long int dataA = (registrador << 4) | (0000);
  long long int dataB = (activation_bit << 29) | (x << 19) | (y << 9) | offset;

  printf("%d, %d\n", (int)dataA, (int)dataB);

  long long int instruction = (dataA << 32) | (dataB);

  printf("%d\n", (int)instruction);

  return instruction;
}

int set_background_block(int column, int line, int R, int G, int B) {}

int set_background_color(int R, int G, int B) {}

void increase_coordinate(sprite_t *sp, int mirror) {}

int collision(sprite_t *sp1, sprite_t *sp2) {}
