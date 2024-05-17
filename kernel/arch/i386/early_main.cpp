//// this is the prekernel, prepares the kernel to do its thing

#include <gdt.h>
#include <idt.h>
#include <types.h>
#include <stdio.h>
#include <util/array.h>
#include <util/panic.h>
#include <bus/pic/pic.h>
#include <memory/physical_mm.h>  
#include <interrupt/interrupt.h>
#include <multiboot/multiboot.h>

extern "C"
auto early_main(u32 mb_magic, multiboot::tag::packed* mb_info) -> void {
  if (mb_magic != multiboot::header::bootloader_magic) {
    util::panic("bootloader is not multiboot complainant");
  }

  gdt.init();
  set_interrupts();
  
  memory::physical_mm.init(mb_info);
}
