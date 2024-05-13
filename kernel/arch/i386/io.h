//// Input/Output utility functions

// mostly being used at kernel/bus/ 

#pragma once

#include <types.h>

namespace io {

  auto in_b(u16 port) ->  u8;
  auto in_w(u16 port) -> u16;
  auto in_l(u16 port) -> u32;

  auto out_b(u16 port,  u8 data) -> void;
  auto out_w(u16 port, u16 data) -> void;
  auto out_l(u16 port, u32 data) -> void;

  auto wait() -> void;
  
}
