#ifndef STATE_H_
#define STATE_H_

#include "window.h"
#include "mouse.h"

/**
 * struct state_t - Defines a model to hold the current state of the game, making
 * it easier to make checks and updates throughout the codebase.
 */
typedef struct state_t state_t;

/**
 * struct state_t - Defines a model to hold the current state of the game, making
 * it easier to make checks and updates throughout the codebase.
 */
struct state_t {
  window_t window;
  mouse_t mouse;

  volatile i32_t is_running;
};

/**
 * struct state_t - Defines a model to hold the current state of the game, making
 * it easier to make checks and updates throughout the codebase.
 */
extern state_t state;

#endif  // !STATE_H_