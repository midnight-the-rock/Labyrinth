#include "definitions.h"

auto irq_timer(u8 irq_id) -> void {
  pic.send_eoi(irq_id);
}
