#include "hardware/definitions.h"
#include "exceptions/definitions.h"

static auto map_hardware_interrupts() -> void {
  u32 interrupt_index = IRQ_OFFSET;

  for (auto item : hardware_interrupts) {
    if (*item != nullptr) {
      idt.set_isr(*item, interrupt_index);
    }

    interrupt_index++;
  }

  pic.enable();
}

static auto map_exception_interrupts() -> void {
  for (u8 i = 0; i < 32; i++) {
    idt.set_isr(isr_exception, i);
  }
}

auto set_interrupts() -> void {
  pic.init();

  map_hardware_interrupts();
  map_exception_interrupts();

  idt.init();
}
