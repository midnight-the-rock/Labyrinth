#include <cpu.h>
#include <bus/ps2/keyboard.h>

constexpr u8 NO_MAP = 0x00;

#define IS_SHIFT_PRESS(scan)   (scan == 0x2a || scan == 0x36)
#define IS_SHIFT_RELEASE(scan) (scan == 0xaa || scan == 0xb6)

static u8 normal_map[256] {
  NO_MAP, 0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
  '[', ']', '\n', NO_MAP, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
  ';', '\'', '`', NO_MAP, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
  ',', '.', '/', NO_MAP, '*', NO_MAP, ' ', NO_MAP, NO_MAP, NO_MAP, NO_MAP,
  NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP,
  '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', NO_MAP,
  NO_MAP, NO_MAP, NO_MAP
};

static u8 shift_map[256] {
  NO_MAP, 033, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
  '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
  '{', '}', '\n', NO_MAP, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
  ':', '"', '~', NO_MAP, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
  '<', '>', '?', NO_MAP, '*', NO_MAP, ' ', NO_MAP, NO_MAP, NO_MAP, NO_MAP,
  NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP, NO_MAP,
  '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', NO_MAP,
  NO_MAP, NO_MAP, NO_MAP
};

__ps2_keyboard ps2_keyboard;

auto __ps2_keyboard::get_keycode(u8 scan_code) -> u8 {
  if (IS_SHIFT_PRESS(scan_code)) {
    m_shift_on = true;
    return 0x00;
  }

  if (IS_SHIFT_RELEASE(scan_code)) {
    m_shift_on = false;
    return 0x00;
  }

  if (m_shift_on) {
    return shift_map[scan_code];
  }

  return normal_map[scan_code];
}

auto __ps2_keyboard::get_scan_code(u8 keycode) -> u8 {
  return 0;
}

auto __ps2_keyboard::chr_in() -> void {
  m_chr_in = true;
}

auto __ps2_keyboard::read() -> u8 {
 _:
  while (!m_chr_in) {}
  
  u8 scan_code = cpu::inb(0x60);
  u8 keycode   = get_keycode(scan_code);

  m_chr_in = false;

  if (keycode == 0x00) goto _;

  return keycode;
}
