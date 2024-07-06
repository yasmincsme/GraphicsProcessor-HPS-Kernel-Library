#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "Device_manager.h"
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

#define INICIO 0
#define JOGO 1
#define PAUSA 2
#define REINICIAR 3

#define KEY_BASE 0x0

#define LW_BRIDGE_SPAN 0x00005000
#define LW_BRIDGE_BASE 0xff200000

static u64_t fd_mouse = -1;
static u64_t fd_map = -1;
static void* LW_virtual = NULL;

// --------------- MÁQUINA DE ESTADOS DAS TELAS -----------------
void change_state(u64_t state) {
  switch (state) {
    case INICIO:
      set_background_color(1, 1, 1);
      break;

    case JOGO:
      set_background_color(6, 1, 1);
      break;

    case PAUSA:
      set_background_color(1, 6, 1);
      break;

    case REINICIAR:
      set_background_color(1, 1, 6);
      break;

    default:
      break;
  }
}

//-------------------------------------------------------------

int main(void) {
  // --------------------- TESTE DA MUDANÇA DE TELA COM OS BOTÕES -------------------------------------

  // MOUSE

  // struct input_event ev;

  // fd_mouse = open("/dev/input/event0", O_RDONLY);
  // if (fd_mouse == -1) {
  //   perror("Erro ao abrir o dispositivo de entrada");
  //   exit(EXIT_FAILURE);
  // }

  // mouse_t mouse = init_mouse(fd_mouse);

  u16_t offset = 25;
  // u16_t selec_sprite = C_font;
  // sprite_t new_sprite = set_new_sprite(offset, selec_sprite);

  u16_t center_x = 320;
  u16_t center_y = 10;

  // new_sprite.ativo = 1;
  // new_sprite.coord_x = center_x;
  // new_sprite.coord_y = center_y;
  // new_sprite.data_register = 2;

  // u8_t i;
  // u8_t result;

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

  // volatile u64_t *KEY_ptr;
  // u64_t fd = -1;
  // void *LW_virtual;

  // fd = open("/dev/mem", (O_RDWR | O_SYNC));
  // if (fd == -1) {
  //   return -1;
  // }

  // LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);

  // if (LW_virtual == MAP_FAILED) {
  //   close(fd);
  //   return -1;
  // }

  sprite_t sprite_car;
  sprite_car.coord_x = 50;
  sprite_car.coord_y = 10;
  sprite_car.step_x = 1;
  sprite_car.step_y = 0;
  sprite_car.offset = 8;
  sprite_car.ativo = 1;

  // KEY_ptr = (u64_t *)(LW_virtual + KEY_BASE);

  mouse_t mouse = init_mouse(fd_mouse);

  u64_t* KEY_ptr = (u64_t*)read_KEY(fd_map, LW_virtual);

  u64_t state_game = INICIO;

  set_background_color(6, 7, 7);

  while (1) {
    change_state(state_game);

    if (state_game == INICIO) {
      if (*KEY_ptr == 0b0111) {
        state_game = JOGO;
        while (*KEY_ptr == 0b0111) {
          /* code */
        }
      } else {
        state_game == INICIO;
      }
      // change_state(state_game);
    } else if (state_game == JOGO) {
      if (*KEY_ptr == 0b0111) {
        state_game = PAUSA;
        while (*KEY_ptr == 0b0111) {
          /* code */
        }

      } else {
        state_game = JOGO;
      }

      //mouse.read(&mouse);

      //     // Atualiza a posição do cursor baseado no movimento do mouse
      //     if (ev.type == EV_REL && ev.code == REL_X) {
      //   new_sprite.coord_x += ev.value;
      // }
      // else if (ev.type == EV_REL && ev.code == REL_Y) {
      //   // new_sprite.coord_y += ev.value;
      // }

      set_dynamic_sprite(sprite_car);

      // change_state(state_game);
    } else if (state_game == PAUSA) {
      if (*KEY_ptr == 0b0111) {
        state_game = JOGO;
        while (*KEY_ptr == 0b0111) {
          /* code */
        }
      } else if (*KEY_ptr == 0b1011) {
        state_game = REINICIAR;
      } else if (*KEY_ptr == 0b1101) {
        state_game = INICIO;
      } else {
        state_game = PAUSA;
      }
      // change_state(state_game);
    } else if (state_game == REINICIAR) {
      sleep(2);
      state_game = JOGO;
    }
    // change_state(state_game);
  }


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
  // sprite_t new_sprite = set_new_sprite(offset);

  // u16_t center_x = 320;
  // u16_t center_y = 10;

  // new_sprite.ativo = 1;
  // new_sprite.coord_x = center_x;
  // new_sprite.coord_y = center_y;
  // new_sprite.data_register = 2;

  // set_dynamic_sprite(new_sprite);

  // sprite_t sprite_car;
  // sprite_car.coord_x = 50;
  // sprite_car.coord_y = 10;
  // sprite_car.step_x = 1;
  // sprite_car.step_y = 0;
  // sprite_car.offset = 8;
  // sprite_car.ativo = 1;

  // u8_t i;
  // u8_t result;

  // while (i = 0, i < 20, ++i) {
  //   increase_coordinate_sprite(&sprite_car);
  //   set_dynamic_sprite(sprite_car);
  //   usleep(10000);
  //   result = collision(&sprite_car, &new_sprite);
  //   if (result) {
  //     printf("COLIDIU!\n");
  //     break;
  //   }
  // }

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
