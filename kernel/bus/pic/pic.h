#pragma once

#include <io.h>
#include <types.h>

static class __pic final {
public:
  __pic();

  auto enable() -> void;
  auto disable() -> void;

  auto irq_mask(u8 irq) -> void;
  auto irq_unmask(u8 irq) -> void;

  auto send_eoi(u8 irq) -> void;

private:
  u8 m_mask_1;
  u8 m_mask_2;

} pic;
