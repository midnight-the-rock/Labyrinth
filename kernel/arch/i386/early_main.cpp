//// this is the prekernel, prepares the kernel to do its thing

#include <gdt.h>
#include <idt.h>
#include <types.h>
#include <stdio.h>
#include <util/panic.h>
#include <memory/physical_mm.h>  
#include <interrupt/interrupt.h>
#include <multiboot/multiboot.h>

extern "C" u32 kernel_size;

extern "C"
auto early_main(u32 mb_magic, multiboot::tag::packed* mb_info) -> void {
  if (mb_magic != multiboot::header::bootloader_magic) {
    util::panic("bootloader is not multiboot complainant");
  }

  set_interrupts();
  gdt.init();
  idt.init();

  u32* end_of_kernel = &kernel_size;

  namespace mb_tag = multiboot::tag;

  auto memory_map  = mb_tag::get<mb_tag::memory_map>(mb_info);
  auto memory_info = mb_tag::get<mb_tag::memory_info>(mb_info);

  auto memory_size = memory_info.value()->mem_lower + memory_info.value()->mem_upper;

  memory::physical_mm.init(memory_size, end_of_kernel);

  for (auto mmap : *memory_map.value()) {
    if (mmap.type == 1 && mmap.addr >= 0x100000) {
      memory::physical_mm.init_region(mmap.addr, mmap.length);
    }
  }

  memory::physical_mm.disable_region((u32)0x100000, (u32)end_of_kernel);

  printf("[ INF ]\t%u total memory blocks\n", memory::physical_mm.blocks_total());
  printf("[ INF ]\t%u memory blocks are free to be used\n", memory::physical_mm.blocks_free());
}
