#include "aux.h"

#include <stdio.h>
#include <string.h>

void reverse(char str[], u32_t length) {
  u32_t start = 0;
  u32_t end = length - 1;

  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

char* itoa(u32_t num, char* str, u32_t base) {
  u32_t i = 0;
  u32_t isNegative = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  if (num < 0 && base == 10) {
    isNegative = 1;
    num = -num;
  }

  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  if (isNegative) {
    str[i++] = '-';
  }

  str[i] = '\0';

  reverse(str, i);

  return str;
}