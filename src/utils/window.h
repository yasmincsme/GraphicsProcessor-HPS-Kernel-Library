#ifndef WINDOW_H_
#define WINDOW_H_

#include "types.h"

typedef struct window_t window_t;

/**
 * struct mouse_t - Defines a model to store the information related to the
 * window, like its proportions.
 */
struct window_t {
  i32_t rows;
  i32_t cols;
  i32_t x_pixels;
  i32_t y_pixels;

  /**
   * @brief Reads the information available about the current window, through
   * the `<sys/ioctl.h>` API, assigning the received values to the proper
   * fields.
   *
   * @param[inout] window The `struct window_t` to be updated.
   */
  void (*read)(window_t* window);
};

/**
 * @brief Initialize the `struct window_t` by assigning its defaults. *
 *
 * @return The properly initialized `struct window_t`.
 */
window_t init_window(void);

#endif  // !WINDOW_H_