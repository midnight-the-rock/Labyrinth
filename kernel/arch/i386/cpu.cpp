#include <cpu.h>

namespace cpu {

  auto inb(u16 port) -> u8 {
    u8 data;
    asm volatile ("in %1, %0" : "=a"(data) : "d"(port));

    return data;
  }

  auto outb(u16 port, u8 data) -> void {
    asm volatile ("out %0, %1" :: "a"(data), "d"(port));
  }

  [[noreturn]]
  auto halt() -> void {
    asm volatile ("cli; hlt");
    __builtin_unreachable();
  }

  auto lgdt(void* gdtr) -> void {
    asm volatile ("lgdt (%0)" :: "r"(gdtr));
  }
}
