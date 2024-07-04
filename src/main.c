#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "GraphSync.h"

#define HEX5_BASE 0x10
#define HEX4_BASE 0x20
#define HEX3_BASE 0x30
#define HEX2_BASE 0x40
#define HEX1_BASE 0x50
#define HEX0_BASE 0x60

#define HEX5_BASE_PHY (LW_VIRTUAL + HEX5_BASE)
#define HEX4_BASE_PHY (LW_VIRTUAL + HEX4_BASE)
#define HEX3_BASE_PHY (LW_VIRTUAL + HEX3_BASE)
#define HEX2_BASE_PHY (LW_VIRTUAL + HEX2_BASE)
#define HEX1_BASE_PHY (LW_VIRTUAL + HEX1_BASE)
#define HEX0_BASE_PHY (LW_VIRTUAL + HEX0_BASE)

#define KEY_BASE 0x0

#define LW_BRIDGE_SPAN 0x00005000
#define LW_BRIDGE_BASE 0xff200000

// int fd;
// --------------- MÁQUINA DE ESTADOS DAS TELAS -----------------
void change_state(u64_t state) {
  switch (state) {
    case 0:
      set_background_color(1, 1, 1);
      break;

    case 1:
      set_background_color(6, 1, 1);
      break;

    case 2:
      set_background_color(1, 6, 1);
      break;

    case 3:
      set_background_color(1, 1, 6);
      break;

    default:
      break;
  }
}

//-------------------------------------------------------------

int main(void) {
  // --------------------- TESTE DA MUDANÇA DE TELA COM OS BOTÕES -------------------------------------
  volatile u64_t *KEY_ptr;
  u64_t fd = -1;
  void *LW_virtual;

  fd = open("/dev/mem", (O_RDWR | O_SYNC));
  if (fd == -1) {
    return -1;
  }

  LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);

  if (LW_virtual == MAP_FAILED) {
    close(fd);
    return -1;
  }

  KEY_ptr = (u64_t *)(LW_virtual + KEY_BASE);

  u64_t state_game;

  state_game = 0;

  while (1) {
    if (*KEY_ptr == 0b1111) {
      // state_game = 0;
      // Mantem o estado anterior
    } else if (*KEY_ptr == 0b1110) {
      state_game = 1;
      change_state(state_game);
    } else if (*KEY_ptr == 0b1101) {
      state_game = 2;
      change_state(state_game);
    } else if (*KEY_ptr == 0b1011) {
      state_game = 3;
      change_state(state_game);
    }
  }

  if (munmap(LW_virtual, LW_BRIDGE_SPAN) != 0) {
    return -1;
  }

  close(fd);

  //----------------------- TESTE DO MÓDULO DE KERNEL (VELOCIDADE QUE DESENHA E APAGA) -------------------------------
  // draw_picture();

  // // sleep(1);
  // clean_background();
  // clean_polygon();
  // clean_sprite();

  //----------------------- TESTE DO MOVIMENTO DO MOUSE (COM SPRITE COMO CURSOR) --------------------------------

  // struct input_event ev;

  // fd = open("/dev/input/event0", O_RDONLY);
  // if (fd == -1) {
  //   perror("Erro ao abrir o dispositivo de entrada");
  //   exit(EXIT_FAILURE);
  // }

  // set_background_color(1, 1, 1);

  // u16_t offset = 25;
  // sprite_fixed_t new_sprite = set_new_sprite(offset);

  // u16_t center_x = 320;
  // u16_t center_y = 10;

  // new_sprite.ativo = 1;
  // new_sprite.coord_x = center_x;
  // new_sprite.coord_y = center_y;
  // new_sprite.data_register = 2;

  // set_fixed_sprite(new_sprite);

  // while (1) {
  //   ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
  //   if (bytes < sizeof(struct input_event)) {
  //     perror("Erro ao ler evento");
  //     exit(EXIT_FAILURE);
  //   }

  //   // Atualiza a posição do cursor baseado no movimento do mouse
  //   if (ev.type == EV_REL && ev.code == REL_X) {
  //     new_sprite.coord_x += ev.value;
  //   } else if (ev.type == EV_REL && ev.code == REL_Y) {
  //     // new_sprite.coord_y += ev.value;
  //   }

  //   set_fixed_sprite(new_sprite);
  // }

  return 0;
}
