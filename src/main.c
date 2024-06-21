#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GraphSync.h"

int main(void) {
  clean_polygon();
  clean_sprite();
  clean_background();

  sleep(5);

  draw_picture();

  return 0;
}
