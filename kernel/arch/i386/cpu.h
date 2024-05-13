// cpu specific function wrappers

#pragma once

#include <types.h>

namespace cpu {
  auto inb(u16 port) -> u8;

  auto outb(u16 port, u8 data) -> void;

  [[noreturn]]
  auto halt() -> void;

  auto lgdt(void* gdtr) -> void;
}
