//// Interrupt Descriptor Table

// https://wiki.osdev.org/Interrupt_Descriptor_Table

#pragma once

#include <cpu.h>
#include <types.h>

struct [[gnu::packed]] __idt_descriptor {
  u16 isr_low;
  u16 cs;
  u8  reserved;
  u8  flags;
  u16 isr_high;
};

struct [[gnu::packed]] __idtr {
  u16 limit;
  u32 base;
};

typedef void (*isr_handler_ptr)(u8 isr_id);

static class __idt final {
public:
  __idt();

  auto init() -> void;
  auto set_isr(isr_handler_ptr handler, u8 isr_id) -> void;

private:
  [[gnu::aligned(0x10)]]
  __idt_descriptor  m_idt[256];
  __idtr            m_idtr;

private:
  auto set_handlers() -> void;
  auto make_descriptor(void* isr, u8 flags) -> __idt_descriptor;
} idt;
