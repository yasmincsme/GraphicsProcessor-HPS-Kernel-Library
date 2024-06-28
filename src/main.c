#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h> 

#include "GraphSync.h"
#include "utils/state.h"

int fd;

int main(void) {
  clean_polygon();
  clean_sprite();
  clean_background();

  struct input_event ev;

  fd = open("/dev/input/event0", O_RDONLY);
  if (fd == -1) {
    perror("Erro ao abrir o dispositivo de entrada");
    exit(EXIT_FAILURE);
  }

  //draw_picture();

  polygon_t cursor;
  cursor.data_register = 10;
  cursor.R = 7;
  cursor.G = 2;
  cursor.B = 2;
  cursor.size = 0001;
  cursor.type = 1;

  //set_polygon(cursor);

  u16_t center_x = 320;
  u16_t center_y = 240;

  cursor.ref_point_x = center_x;
  cursor.ref_point_y = center_y;

  while (1) {
    // ...

    ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
    if (bytes < sizeof(struct input_event)) {
        perror("Erro ao ler evento");
        exit(EXIT_FAILURE);
    }

    // Atualiza a posição do cursor baseado no movimento do mouse
    if (ev.type == EV_REL && ev.code == REL_X) {
        cursor.ref_point_x += ev.value;
    } else if (ev.type == EV_REL && ev.code == REL_Y) {
        cursor.ref_point_y += ev.value;
    }

    // Verifica outros eventos de mouse, como cliques
    if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {
        printf("Botão esquerdo pressionado\n");
        // Lógica para tratar o clique do botão esquerdo, se necessário
    } else if (ev.type == EV_KEY && ev.code == BTN_RIGHT && ev.value == 1) {
        printf("Botão direito pressionado\n");
        // Lógica para tratar o clique do botão direito, se necessário
    }

    // Limpa e redefine o polígono ou sprite na nova posição
    set_polygon(cursor);

  }

  return 0;
}
