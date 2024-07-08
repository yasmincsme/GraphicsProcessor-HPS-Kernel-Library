#include "State_machine.h"

void change_state(u64_t *state, u64_t *button) {
  switch (*state) {
    case INICIO:

      if (*button == 0b0111) {
        *state = JOGO;
        while (*button == 0b0111) {
          /* code */
        }
      }

      else {
        *state = INICIO;
      }

      break;

    case JOGO:
      if (*button == 0b0111) {
        *state = PAUSA;
        while (*button == 0b0111) {
          /* code */
        }
      } else {
        *state = JOGO;
      }
      // set_dynamic_sprite(sprite_car);

      break;

    case PAUSA:

      if (*button == 0b0111) {
        *state = JOGO;
        while (*button == 0b0111) {
          /* code */
        }
      } else if (*button == 0b1011) {
        *state = REINICIAR;
      } else if (*button == 0b1101) {
        *state = INICIO;
      } else {
        *state = PAUSA;
      }
      break;

    case REINICIAR:
      sleep(2);
      *state = JOGO;
      break;

    default:
      break;
  }
}