#include <cpu.h>
#include <stdio.h>
#include "definitions.h"

auto isr_exception(u8 isr_id) -> void {
  printf("[ ERR ]\texception %d: %s\n", isr_id, exceptions[isr_id]);
  cpu::halt();
}
