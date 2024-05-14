#include <idt.h>
#include <stdio.h>
#include <util/panic.h>

extern "C"
void* isr_stub_table[];

static isr_handler_ptr interrupt_handler_table[256];

extern "C"
auto interrupt_handler(u8 isr_id) -> void {
  interrupt_handler_table[isr_id](isr_id);
}

extern "C"
auto unhandled_interrupt(u8 isr_id) -> void {
  printf("[ ERR ]\tunhandled interrupt %x\n", isr_id);
  util::panic("");
}

__idt::__idt() {
  set_handlers();
}

auto __idt::init() -> void {
  m_idtr.base  = (u64)m_idt;
  m_idtr.limit = (u16)((sizeof(__idt_descriptor) * 256) - 1);

  for (u16 vector = 0; vector < 256; vector++) {
    m_idt[vector] = make_descriptor(isr_stub_table[vector], 0x8e);
  }

  asm volatile ("lidt (%0)" :: "r"(&m_idtr));
  asm volatile ("sti");
}

auto __idt::set_isr(isr_handler_ptr handler, u8 isr_id) -> void {
  interrupt_handler_table[isr_id] = handler;
}

auto __idt::set_handlers() -> void {
  for (u16 i = 0; i < 256; i++) {
    interrupt_handler_table[i] = unhandled_interrupt;
  }
}

auto __idt::make_descriptor(void* isr, u8 flags) -> __idt_descriptor {
  __idt_descriptor desc;

  desc.isr_low  = (u32)isr & 0xffff;
  desc.cs       = 0x08;
  desc.flags    = flags;
  desc.isr_high = (u32)isr >> 16;
  desc.reserved = 0;

  return desc;
}
