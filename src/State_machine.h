#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "utils/types.h"

#define INICIO 0
#define JOGO 1
#define PAUSA 2
#define REINICIAR 3

void change_state(u64_t *state, u64_t *button);

#endif  // STATE_MACHINE_H_