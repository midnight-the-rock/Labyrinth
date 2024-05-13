//// Kernel's puts implementation code
#include <stdio.h>

extern "C" 
auto puts(const char* str) -> i32 {
  while (*str != 0) {
    putchar(*str);
    str++;
  }

  putchar('\n');

  return 0;
}
