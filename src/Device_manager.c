#include "Device_manager.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void read_mouse(mouse_t* mouse) {
  u8_t data[3] = {0};

  if (read(mouse->fd, data, sizeof(data))) {
    mouse->left = (data[0] & 0x1) ? 1 : 0;
    mouse->right = (data[0] & 0x02) ? 1 : 0;
    mouse->middle = (data[0] & 0x04) ? 1 : 0;

    mouse->x += (data[1] / ((SENSIBILITY > 0) ? SENSIBILITY : 1));

    if (mouse->x < 0) {
      mouse->x = 0;
    }
    if (mouse->x > WIDTH) {
      mouse->x = WIDTH;
    }

    mouse->y += ((data[2] * -1) / ((SENSIBILITY > 0) ? SENSIBILITY : 1));

    if (mouse->y < 0) {
      mouse->y = 0;
    }
    if (mouse->y > HEIGHT) {
      mouse->y = HEIGHT;
    }
  }
}

mouse_t init_mouse(u64_t fd_mouse) {
  fd_mouse = open("/dev/input/event0", O_RDONLY);

  if (fd_mouse == -1) {
    perror("Erro ao abrir o dispositivo de memória");
    exit(EXIT_FAILURE);
  }

  return (mouse_t){
      .fd = fd_mouse,
      .x = 50,
      .y = 50,
      .left = 0,
      .right = 0,
      .middle = 0,
      .read = read_mouse,
  };
}

u64_t* read_KEY(u64_t fd_map, void* LW_virtual) {
  fd_map = open("/dev/mem", (O_RDWR | O_SYNC));
  if (fd_map == -1) {
    perror("Erro ao abrir o dispositivo de memória");
    exit(EXIT_FAILURE);
  }

  LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_map, LW_BRIDGE_BASE);
  if (LW_virtual == MAP_FAILED) {
    perror("Erro ao mapear memória");
    exit(EXIT_FAILURE);
  }

  u64_t* KEY_ptr = (u64_t*)(LW_virtual + KEY_BASE);
  return KEY_ptr;
}

void close_KEY(u64_t fd_map, void* LW_virtual) {
  if (munmap(LW_virtual, LW_BRIDGE_SPAN) == -1) {
    perror("Erro ao desmapear memória");
    exit(EXIT_FAILURE);
  }

  if (close(fd_map) == -1) {
    perror("Erro ao fechar dispositivo de memória");
    exit(EXIT_FAILURE);
  }
}
