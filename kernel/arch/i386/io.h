//// Input/Output utility functions

#pragma once

#include <types.h>

namespace io {

  [[gnu::always_inline]]
  inline auto in_b(u16 port) -> u8 {
    u8 data;
    asm volatile ("inb %1, %0" : "=a"(data) : "d"(port));

    return data;
  }

  [[gnu::always_inline]]
  inline auto in_w(u16 port) -> u16 {
    u16 data;
    asm volatile ("inw %1, %0" : "=a"(data) : "d"(port));

    return data;
  }
  
  [[gnu::always_inline]]
  inline auto in_l(u16 port) -> u32 {
    u32 data;
    asm volatile ("inl %1, %0" : "=a"(data) : "d"(port));

    return data;
  }

  [[gnu::always_inline]]
  inline auto out_b(u16 port,  u8 data) -> void {
    asm volatile ("outb %0, %1" :: "a"(data), "d"(port));
  }

  [[gnu::always_inline]]
  inline auto out_w(u16 port, u16 data) -> void {
    asm volatile ("outw %0, %1" :: "a"(data), "d"(port));
  }

  [[gnu::always_inline]]
  inline auto out_l(u16 port, u32 data) -> void {
    asm volatile ("outl %0, %1" :: "a"(data), "d"(port));
  }

  [[gnu::always_inline]]
  inline auto wait() -> void {
    out_b(0x80, 0x00);
  }
  
}
