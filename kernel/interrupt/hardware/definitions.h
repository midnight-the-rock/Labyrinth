//// all interrupts related to hardware interrupts

#pragma once

#include <idt.h>
#include <types.h>
#include <util/array.h>
#include <bus/pic/pic.h>

/* hardware interrupt requests, each handler is defined in
   different files (remember to add them to hardware_interrupt[n] */

auto set_hardware_interrupts() -> void;
auto irq_timer(u8 irq_id) -> void;
auto irq_keyboard(u8 irq_id) -> void;
auto irq_serial(u8 irq_id) -> void;

/* this array of interrupts should be looped trough and
   set to the isr with the interrupt number + IRQ_OFFSET  */
static auto hardware_interrupts = util::array<isr_handler_ptr, 16> {
  irq_timer,          // PIT
  irq_keyboard,       // keyboard interrupts
  nullptr,            // cascade (should not be called)
  nullptr,            // COM2 (if enabled)
  irq_serial,         // COM1 (if enabled)
  nullptr,            // LPT2  (if enabled)
  nullptr,            // floppy
  nullptr,            // LPT1  (if enabled)
  nullptr,            // CMOS real-time clock (if enabled)
  nullptr,            // free for peripherals
  nullptr,            // free for peripherals
  nullptr,            // free for peripherals
  nullptr,            // PS2 mouse
  nullptr,            // FPU / coprocessor / inter-processor
  nullptr,            // primary ATA HHD
  nullptr,            // secondary ATA HHD
};

constexpr u8 IRQ_OFFSET = 0x20;
constexpr u8 IRQ_TOTAL_ENTRIES = sizeof(hardware_interrupts) / sizeof(isr_handler_ptr);
