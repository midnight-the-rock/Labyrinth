//// uart class controlling serial communication through COM1 port

//   information regarding serial ports can be found at:
//   https://wiki.osdev.org/Serial_ports

#pragma once

#include <cpu.h>
#include <types.h>

class __uart final {
public:
  __uart();

  auto read() -> u8;
  auto write(u8 data) -> void;

  auto cursor_position() -> i32;
  auto cursor_move_newline() -> void;
  auto cursor_move_forward() -> void;
  auto cursor_move_backward() -> void;


  auto is_enabled_p() -> bool;

private:
  bool m_is_enabled_p;

private:
  auto init_serial() -> void;
  auto init_cursor() -> void;

  auto received() -> i32;
  auto no_transmit() -> i32;
};

extern __uart uart;
