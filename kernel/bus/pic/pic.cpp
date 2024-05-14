#include "pic.h"
#include "definitions.h"
#include <stdio.h>

__pic::__pic() = default;

auto __pic::init() -> void {
  m_mask_1 = io::in_b((u16)pic_master::data);
  m_mask_2 = io::in_b((u16)pic_slave::data);

  // start initialization (in cascade mode)
  io::out_b((u16)pic_master::command, (u8)icw1::init | (u8)icw1::icw4);
  io::wait();

  io::out_b((u16)pic_slave::command, (u8)icw1::init | (u8)icw1::icw4);
  io::wait();

  // set irq vector offset
  io::out_b((u16)pic_master::data, 0x20);
  io::wait();

  io::out_b((u16)pic_slave::data, 0x28);
  io::wait();

  // tell pic master about pic slave
  io::out_b((u16)pic_master::data, 4);
  io::wait();

  // tell pic slave its cascade identity
  io::out_b((u16)pic_slave::data, 2);
  io::wait();

  // set pic to 8086 mode
  io::out_b((u16)pic_master::data, (u8)icw4::mode_8086);
  io::wait();

  io::out_b((u16)pic_slave::data, (u8)icw4::mode_8086);
  io::wait();

  // keep disable for now 
  disable();
}

auto __pic::enable() -> void {
  // enable interrupts back from their previous state
  io::out_b((u16)pic_master::data, m_mask_1);
  io::out_b((u16)pic_master::data, m_mask_2);
}

auto __pic::disable() -> void {
  // save current mask for enable()
  m_mask_1 = io::in_b((u16)pic_master::data);
  m_mask_2 = io::in_b((u16)pic_slave::data);

  // disable all interrupts
  io::out_b((u16)pic_master::data, 0xff);
  io::out_b((u16)pic_master::data, 0xff);
}

auto __pic::irq_mask(u8 irq) -> void {
  u8 port;
  u8 mask;

  if (irq >= 8) {
    irq  = irq - 8;
    port = io::in_b((u16)pic_slave::data);
  }
  else {
    port = io::in_b((u16)pic_master::data);
  }

  mask = port | (1 << irq);

  io::out_b(port, mask);
}

auto __pic::irq_unmask(u8 irq) -> void {
  u8 port;
  u8 mask;

  if (irq >= 8) {
    irq  = irq - 8;
    port = io::in_b((u16)pic_slave::data);
  }
  else {
    port = io::in_b((u16)pic_master::data);
  }

  mask = port & ~(1 << irq);

  io::out_b(port, mask);  
}

auto __pic::send_eoi(u8 irq) -> void {
  if (irq >= 8) {
    io::out_b((u16)pic_slave::command, end_of_interrupt);
  }

  io::out_b((u16)pic_master::command, end_of_interrupt);
}
