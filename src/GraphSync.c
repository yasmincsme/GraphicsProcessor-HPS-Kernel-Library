#include "GraphSync.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Funções write e read para acesso ao arquivo*/
void write_data(int fd, uint64_t data) {
  ssize_t result = write(fd, &data, sizeof(data));
  if (result == -1) {
    perror("Falha na escrita do dispositivo");
    exit(EXIT_FAILURE);
  }
}

// Função para ler dados do dispositivo
uint64_t read_data(int fd) {
  uint64_t data;
  ssize_t result = read(fd, &data, sizeof(data));
  if (result == -1) {
    perror("Falha na leitura do dispositivo");
    exit(EXIT_FAILURE);
  }

  return data;
}

uint64_t set_sprite(int reg, int x, int y, int offset, int activation_bit) {
  int opcode = 0000;

  uint32_t dataA = reg << 6 | opcode << 2;
  // printf("%x\n", (uint64_t)dataA);

  uint32_t dataB = activation_bit << 29 | x << 19 | y << 9 | offset;
  // printf("%x\n", (uint64_t)dataB);

  /*Transformando dataA e dataB em caracteres e concatenando as strings*/
  char data_a_str[32];
  char data_b_str[32];
  char instruction[64];

  itoa(dataA, data_a_str, 2);
  itoa(dataB, data_b_str, 2);
  strcat(instruction, data_a_str);
  strcat(instruction, data_b_str);

  // printf("%s\n", instruction);

  return instruction;
}

uint64_t set_background_block(int column, int line, int R, int G, int B) {
  int opcode = 0010;
  int memory_address = column * line;

  uint32_t dataA = memory_address << 27 | opcode << 23;
  uint32_t dataB = B << 6 | G << 3 | R;

  /*Transformando dataA e dataB em caracteres e concatenando as strings*/
  char data_a_str[32];
  char data_b_str[32];
  char instruction[64];

  itoa(dataA, data_a_str, 2);
  itoa(dataB, data_b_str, 2);
  strcat(instruction, data_a_str);
  strcat(instruction, data_b_str);

  printf("%s\n", instruction);
  return instruction;
}

uint64_t set_background_color(int R, int G, int B) {
  long long int instruction = B << 6 | G << 3 | R;

  return instruction;
}

uint64_t set_polygon(int reg, int format, int R, int G, int B, int size, int ref_point_y, int ref_point_x) {
  int opcode = 0011;

  uint32_t dataA = reg << 5 | opcode; //TOMAR CUIDADO PORQUE SE A FORMA FOR 0 DE QUADRADO ELE TIRA OS BITS DA FORMA
  uint32_t dataB = format << 31 | R << 28 | G << 25 | R << 22 | size << 18 | ref_point_y << 9 | ref_point_x;

  /*Transformando dataA e dataB em caracteres e concatenando as strings*/
  char data_a_str[32];
  char data_b_str[32];
  char instruction[64];

  itoa(dataA, data_a_str, 2);
  itoa(dataB, data_b_str, 2);
  strcat(instruction, data_a_str);
  strcat(instruction, data_b_str);

  printf("%s\n", instruction);
  return instruction;
}

void increase_coordinate(sprite_t *sp, int mirror) {}

int collision(sprite_t *sp1, sprite_t *sp2) {}
