#include <gdt.h>
#include <idt.h>
#include <stdio.h>
#include <types.h>
#include <interrupt/interrupt.h>

extern "C"
auto early_main(u32 mb_magic, u32 mb_info) -> void {
  printf("%x\n", mb_magic);

  set_interrupts();
  gdt.init();
  idt.init();
}
