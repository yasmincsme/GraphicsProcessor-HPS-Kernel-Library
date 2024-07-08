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
#include "State_machine.h"

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

int main(void) {
  // --------------------- TESTE DA MUDANÇA DE TELA COM OS BOTÕES -------------------------------------

  sprite_t sprite_car;
  sprite_car.coord_x = 50;
  sprite_car.coord_y = 10;
  sprite_car.step_x = 1;
  sprite_car.step_y = 0;
  sprite_car.offset = 8;
  sprite_car.ativo = 1;

  mouse_t mouse = init_mouse(fd_mouse);
  u64_t* KEY_ptr = (u64_t*)read_KEY(fd_map, LW_virtual);
  u64_t state_game = INICIO;

  set_background_color(6, 7, 7);

  while (1) {
    change_state(&state_game, KEY_ptr);

    if (state_game == INICIO) {
      set_background_color(1, 1, 1);

    } else if (state_game == JOGO) {
      set_background_color(6, 1, 1);
      set_dynamic_sprite(sprite_car);

    } else if (state_game == PAUSA) {
      set_background_color(1, 6, 1);

    } else if (state_game == REINICIAR) {
      set_background_color(1, 1, 6);
    }
  }

  return 0;
}
