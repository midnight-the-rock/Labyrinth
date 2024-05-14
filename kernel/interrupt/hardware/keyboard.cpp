#include "definitions.h"

#include <stdio.h>
#include <bus/ps2_keyboard.h>

auto irq_keyboard(u8 code) -> void {
  u8 received  = cpu::inb(0x61);
  u8 scan_code = cpu::inb(0x60);

  // tell the interrupt that they key was received
  cpu::outb(0x61, received | 0x80);
  cpu::outb(0x61, received);

  auto keycode = ps2_keyboard.get_keycode(scan_code); 

  if (keycode != 0x00) { 
    putchar(keycode);
  }

  pic.send_eoi(code);
}
