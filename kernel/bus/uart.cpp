#include <stdio.h>
#include <bus/uart.h>

#define COM1 0x3f8
#define CRT_PORT 0x3d4

__uart uart;

__uart::__uart() {
   init_serial();
   init_cursor();
}

auto __uart::read() -> u8 {
  while (received() == 0) {}

  return cpu::inb(COM1);
}

auto __uart::write(u8 data) -> void {
  while (no_transmit() == 0) {}

  cpu::outb(COM1, data);
}

auto __uart::cursor_position() -> i32 {
  i32 pos = 0;

  cpu::outb(CRT_PORT, 14);
  pos  = cpu::inb(CRT_PORT + 1) << 8;

  cpu::outb(CRT_PORT, 15);
  pos |= cpu::inb(CRT_PORT + 1);

  return pos;
}

auto __uart::cursor_move_newline() -> void {
  auto position = cursor_position();
  position += 80 - (position % 80);

  cpu::outb(CRT_PORT, 0x0f);
  cpu::outb(CRT_PORT + 1, u8(position & 0xff));
  cpu::outb(CRT_PORT, 0x0e);
  cpu::outb(CRT_PORT + 1, u8((position >> 8) & 0xff));    
}

auto __uart::cursor_move_forward() -> void {
  auto position = cursor_position() + 1;

  if (position > (80 * 25)) {
    position = 0;
  }

  cpu::outb(CRT_PORT, 0x0f);
  cpu::outb(CRT_PORT + 1, u8(position & 0xff));
  cpu::outb(CRT_PORT, 0x0e);
  cpu::outb(CRT_PORT + 1, u8((position >> 8) & 0xff));    
}

auto __uart::cursor_move_backward() -> void {
  auto position = cursor_position() - 1;

  if (position < 0) {
    return;
  }

  cpu::outb(CRT_PORT, 0x0f);
  cpu::outb(CRT_PORT + 1, u8(position & 0xff));
  cpu::outb(CRT_PORT, 0x0e);
  cpu::outb(CRT_PORT + 1, u8((position >> 8) & 0xff));      
}

auto __uart::is_enabled_p() -> bool {
  return m_is_enabled_p;
}

auto __uart::init_serial() -> void {
  cpu::outb(COM1 + 1, 0x00);
  cpu::outb(COM1 + 3, 0x80);
  cpu::outb(COM1 + 0, 0x03);
  cpu::outb(COM1 + 1, 0x00);
  cpu::outb(COM1 + 3, 0x03);
  cpu::outb(COM1 + 2, 0xc7);
  cpu::outb(COM1 + 4, 0x0b);
  cpu::outb(COM1 + 4, 0x1e);
  cpu::outb(COM1 + 0, 0xae);

  if (cpu::inb(COM1 + 0) != 0xae) {
    m_is_enabled_p = false;
    return;
  }

  cpu::outb(COM1 + 1, 0x01);
  cpu::outb(COM1 + 4, 0x0f);

  m_is_enabled_p = true;
}

auto __uart::init_cursor() -> void {
  cpu::outb(CRT_PORT, 0x0a);
  cpu::outb(CRT_PORT + 1, (cpu::inb(CRT_PORT + 1) & 0xc0) | 12);
  
  cpu::outb(CRT_PORT, 0x0b);
  cpu::outb(CRT_PORT + 1, (cpu::inb(CRT_PORT + 1) & 0xe0) | 13);
}

auto __uart::received() -> i32 {
  return cpu::inb(COM1 + 5) & 1;
}

auto __uart::no_transmit() -> i32 {
  return cpu::inb(COM1 + 5) & 0x20;
}
