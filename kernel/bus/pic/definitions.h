//// definitions required by the pic controller

// https://wiki.osdev.org/8259_PIC#Common_Definitions

#pragma once

#include <types.h>

enum class pic_master : u8 {
  data    = 0x21,
  command = 0x20,
};

enum class pic_slave : u8 {
  data    = 0xa1,
  command = 0xa0,
};

enum class icw1 : u8 {
  icw4      = 0x01, // icw4 present
  single    = 0x02, // single (cascade) mode
  interval4 = 0x04, // address interval
  level     = 0x08, // level triggered (edge) mode
  init      = 0x10, // initialization
};

enum class icw4 : u8 {
  mode_8086   = 0x01, // 8086 mode
  mode_auto   = 0x02, // automatic EOI
  mode_buff_s = 0x08, // buffered mode (slave)
  mode_buff_m = 0x0c, // buffered mode (master)
};

constexpr u8 end_of_interrupt  = 0x20;
