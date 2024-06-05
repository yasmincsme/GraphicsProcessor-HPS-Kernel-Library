#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/data_bus"

// Função para escrever dados no dispositivo
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

int main(void) {
  // printf("Hello world\n");

  int fd;
  uint64_t write_value = 0x0000000000000f1f;
  uint64_t read_value;

  // Abre o dispositivo
  fd = open(DEVICE_PATH, O_RDWR);
  if (fd == -1) {
    perror("Falha ao abrir o dispositivo");
    exit(EXIT_FAILURE);
  }

  // Escreve um valor no dispositivo
  write_data(fd, write_value);

  // Lê o valor do dispositivo
  read_value = read_data(fd);

  // Verifica se o valor lido é igual ao valor escrito
  if (read_value != write_value) {
    fprintf(stderr, "Erro: valor lido (0x%016lx) diferente do valor escrito (0x%016lx)\n", read_value, write_value);
  } else {
    printf("Valor lido é igual ao valor escrito. Sucesso!\n");
  }

  // Fecha o dispositivo
  close(fd);

  return 0;
}