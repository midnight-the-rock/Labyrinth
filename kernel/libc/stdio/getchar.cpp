//// Kernel's getchar implementation code

#include <bus/uart.h>

extern "C"
auto getchar() -> u8 {
  return uart.read();
}
