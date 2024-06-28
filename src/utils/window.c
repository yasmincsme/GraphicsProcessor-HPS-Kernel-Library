#include "window.h"

//#include <sys/ioctl.h>
#include <unistd.h>

void read_window(window_t* window) {
//   struct winsize winsize;

//   ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);

//   window->rows = winsize.ws_row;
//   window->cols = winsize.ws_col;
//   window->x_pixels = winsize.ws_xpixel;
//   window->y_pixels = winsize.ws_ypixel;
// }

// window_t init_window(void) {
//   window_t window = {0};

//   read_window(&window);

//   window.read = read_window;

//   return window;
}