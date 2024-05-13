//// Kernel's putchar implementation

#include <stdio.h>
#include <bus/uart.h>
#include <bus/vga_text.h>

#define CRT_MEMORY (u16*)0xb8000
#define TAB        9
#define NEWLINE    13
#define BACKSPACE  127

static u16* crt = CRT_MEMORY;

static auto crt_character(u8 chr) -> u16 {
  u8 entry_color = (u8)0xf | (u8)0x0 << 4; // white on black background

  return chr | entry_color << 8;
}

auto get_next_multiple(u32 position) -> u32 {
  u32 i = position + 1;

  while ((i % 8) != 0) i++;

  return i - position;
}

extern "C"
auto putchar(u8 chr) -> i32 {
  auto position = uart.cursor_position();

  switch (chr) {
  case '\e':
    uart.write('\e');
    break;

  case '\t':
    for (u32 i = 0; i < get_next_multiple(position); i++) {
      uart.cursor_move_forward();
    }

    uart.write('\t');

    break;

  case '\b':
  case BACKSPACE:
    crt[position-1] = crt_character(' ');
    
    uart.write('\b');
    uart.write(' ');
    uart.write('\b');
    uart.cursor_move_backward();
    break;

  case '\n':
  case NEWLINE:
    crt[position + (80 - (position % 80))] = crt_character(' ');
    uart.write('\n');
    uart.cursor_move_newline();
    break;
    
  default:
    crt[position] = crt_character(chr);
    uart.write(chr);
    uart.cursor_move_forward();
    break;
  }
  
  // no error reporting for now, include return type for completeness
  return 0;
}
