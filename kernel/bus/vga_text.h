/* VGA text mode interaction
   basic writing to screen for now */

#pragma once

#include <io.h>
#include <types.h>

static class __vga final {
public:
  __vga();

  auto clear() -> void;
  auto write(u8 chr) -> void;

private:
  u16* crt_memory = (u16*)0xb8000;

private:
  auto cursor_init() -> void;
  auto cursor_inc() -> void;
  auto cursor_dec() -> void;
  auto cursor_reset() -> void;
  auto cursor_newline() -> void;
  auto cursor_position() -> u16;

} vga;
