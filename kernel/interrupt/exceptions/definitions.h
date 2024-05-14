//// all interrupts related to exceptions

#pragma once

#include <idt.h>
#include <types.h>
#include <util/array.h>

auto isr_exception(u8 isr_id) -> void;

/* since I completely ignore whether the exception contains error code
   or not, it will just be a message for the exception and halt the cpu.

   maybe setup actual handling for exceptions that does not need halting */

static auto exceptions = util::array<const char*, 32> {
  "division error",
  "debug",
  "non-maskable interrupt",
  "breakpoint",
  "overflow",
  "bound range exceeded",
  "invalid opcode",
  "device not available",
  "double fault",
  "coprocessor segment overrun",
  "invalid TSS",
  "segment not present",
  "stack-segment fault",
  "general protection fault",
  "page fault",
  "reserved",
  "floating point exception",
  "alignment check",
  "machine check",
  "SIMD floating-point exception",
  "virtualization exception",
  "control protection exception",
  "reserved",
  "hypervisor injection exception",
  "VMM communication exception",
  "security exception",
  "reserved",
  "triple fault",
  "FPU error interrupt",
};
