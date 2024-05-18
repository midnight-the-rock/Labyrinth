//// PS/2 keyboard driver

// for useful information about dealing with this thing
// please refer to these links
// https://wiki.osdev.org/PS/2_Keyboard

#pragma once

#include <types.h>

class __ps2_keyboard final {
public:
  auto read() -> u8;
  auto write(u8 chr) -> void;

  auto get_keycode(u8 scan_code) -> u8;
  auto get_scan_code(u8 keycode) -> u8;

  auto chr_in() -> void;

private:
  bool m_chr_in   = false;
  bool m_shift_on = false;
};

extern __ps2_keyboard ps2_keyboard;
