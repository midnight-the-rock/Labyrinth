#include "vga_text.h"

static constexpr u16 CRT_PORT   = 0x3d4;
static constexpr u16 CRT_WIDTH  = 80;
static constexpr u16 CRT_HEIGHT = 25;

__vga::__vga() {
  
}

auto __vga::clear() -> void {
  cursor_reset();

  for (u32 i = 0; i < (CRT_WIDTH * CRT_HEIGHT); i++) {
    crt_memory[cursor_position()] = ' ' | 0x0f << 8;
    cursor_inc();
  }

  cursor_reset();
}

auto __vga::write(u8 chr) -> void {
  crt_memory[cursor_position()] = chr | 0xf << 8;
  cursor_inc();
}

auto __vga::cursor_init() -> void {
  io::out_b(CRT_PORT, 0x0a);
  io::out_b(CRT_PORT + 1, (io::in_b(CRT_PORT + 1) & 0xc0) | 12);
  
  io::out_b(CRT_PORT, 0x0b);
  io::out_b(CRT_PORT + 1, (io::in_b(CRT_PORT + 1) & 0xe0) | 13);
}

auto __vga::cursor_inc() -> void {
  auto position = cursor_position() + 1;

  // clear screen once the end of screen has been reached
  if (position > (80 * 25)) {
    position = 0;
  }

  // set cursor low register to 0
  io::out_b(CRT_PORT,     0x0f);
  io::out_b(CRT_PORT + 1, static_cast<u8>(position & 0xff));

  // set cursor high register to 0
  io::out_b(CRT_PORT,     0x0e);
  io::out_b(CRT_PORT + 1, static_cast<u8>((position >> 8) & 0xff));
}

auto __vga::cursor_dec() -> void {
  auto position = cursor_position() - 1;

  // set cursor low register to 0
  io::out_b(CRT_PORT,     0x0f);
  io::out_b(CRT_PORT + 1, static_cast<u8>(position & 0xff));

  // set cursor high register to 0
  io::out_b(CRT_PORT,     0x0e);
  io::out_b(CRT_PORT + 1, static_cast<u8>((position >> 8) & 0xff));
}

auto __vga::cursor_reset() -> void {
  // set cursor low register to 0
  io::out_b(CRT_PORT,     0x0f);
  io::out_b(CRT_PORT + 1, 0x00);

  // set cursor high register to 0
  io::out_b(CRT_PORT,     0x0e);
  io::out_b(CRT_PORT + 1, 0x00);
}

auto __vga::cursor_newline() -> void {
  auto position_current = cursor_position();
  auto offset           = 80 - (position_current % 80);
  auto position_new     = position_current + offset;

  // modify cursor location low register
  io::out_b(CRT_PORT, 0x0f);
  io::out_b(CRT_PORT + 1, static_cast<u8>(position_new & 0xff));

  // modify cursor location high register
  io::out_b(CRT_PORT, 0x0e);
  io::out_b(CRT_PORT + 1, static_cast<u8>((position_new >> 8) & 0xff));
}

auto __vga::cursor_position() -> u16 {
  u16 position = 0;

  // cursor position high register
  io::out_b(CRT_PORT, 0x0e);
  position = io::in_b(CRT_PORT + 1) << 8;

  // cursor position low register
  io::out_b(CRT_PORT, 0x0f);
  position = position | io::in_b(CRT_PORT + 1);

  return position;
}
