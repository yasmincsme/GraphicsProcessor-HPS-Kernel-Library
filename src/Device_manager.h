#ifndef DEVICE_MANAGER_H_
#define DEVICE_MANAGER_H_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "utils/types.h"
#include "utils/ui.h"

#define KEY_BASE 0x0
#define LW_BRIDGE_SPAN 0x00005000
#define LW_BRIDGE_BASE 0xff200000

#define HEIGHT 360
#define WIDTH 480
#define SENSIBILITY 2

typedef struct mouse_t {
  u64_t fd;
  u32_t x;
  u32_t y;
  u8_t left;
  u8_t middle;
  u8_t right;

  void (*read)(struct mouse_t* mouse);
} mouse_t;

void read_mouse(mouse_t* mouse);
mouse_t init_mouse(u64_t fd_mouse);
u64_t* read_KEY(u64_t fd_map, void* LW_virtual);
void close_KEY(u64_t fd_map, void* LW_virtual);

#endif  // DEVICE_MANAGER_H_