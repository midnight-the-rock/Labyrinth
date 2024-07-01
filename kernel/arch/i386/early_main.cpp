//// this is the prekernel, prepares the kernel to do its thing

#include <gdt.h>
#include <idt.h>
#include <io.h>
#include <stdio.h>
#include <util/array.h>
#include <util/panic.h>
#include <bus/pic/pic.h>
#include <memory/virtual_mm.h>  
#include <memory/physical_mm.h>  
#include <interrupt/interrupt.h>
#include <multiboot/multiboot.h>

extern u8 end_of_kernel[];

extern "C"
auto early_main(u32 mb_magic, multiboot::tag::packed* mb_info) -> void {
  if (mb_magic != multiboot::header::bootloader_magic) {
    util::panic("bootloader is not multiboot complainant");
  }

  gdt.init();

  set_interrupts();

  namespace mb_tag = multiboot::tag;

  auto mmap  = mb_tag::get<mb_tag::memory_map>(mb_info);
  auto minfo = mb_tag::get<mb_tag::memory_info>(mb_info);
  auto kernel_end  = (void*)0;
  auto memory_size = (minfo.value()->mem_lower + minfo.value()->mem_upper);

  printf("kernel size: %x\n", end_of_kernel);
  printf("memory size: %dMb\n", (minfo.value()->mem_lower + minfo.value()->mem_upper) / 1024);

  for (auto map : *mmap.value()) {
    printf("%d\t%x\n", map.type, map.addr);
  }
}
