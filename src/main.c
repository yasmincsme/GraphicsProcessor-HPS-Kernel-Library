#include <stdio.h>

#include "/dev/gpp_data_bus"

int main(void) {
  printf("Hello world\n");

  uint64_t fd = data_bus_open();
  uint64_t data;
  uint64_t bits = data_bus_read(fd, data, sizeof(data));
  fprintf(bits);

  return 0;
}