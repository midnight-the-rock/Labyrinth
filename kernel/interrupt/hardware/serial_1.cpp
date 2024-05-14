#include "definitions.h"

#include <stdio.h>
#include <bus/uart.h>

auto irq_serial(u8 code) -> void {
  putchar(uart.read());
  pic.send_eoi(code);
}
