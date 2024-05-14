//// Global Descriptor Table utils

// https://wiki.osdev.org/GDT_Tutorial

#pragma once

#include <cpu.h>
#include <types.h>

extern "C" void lgdt(void* gdtr, u16 code, u16 data);

struct [[gnu::packed]] __gdt_desc {
  u16 limit;
  u16 base_low;
  u8  base_mid;
  u8  access;
  u8  flags;
  u8  base_high;
};

struct [[gnu::packed]] __gdtr {
  u16         limit;
  __gdt_desc* base;
};

enum class __gdt_access {
  code_readable         = 0x02,
  data_writeable        = 0x02,

  code_conforming       = 0x04,
  data_direction_normal = 0x00,
  data_direction_down   = 0x04,

  code_segment          = 0x18,
  data_segment          = 0x10,
  descriptor_tss        = 0x00,

  dpl0                  = 0x00,
  dpl3                  = 0x60,

  present               = 0x80,
};

enum class __gdt_flags {
  bit_64                = 0x20,
  bit_32                = 0x40,
  bit_16                = 0x00,

  granularity_1b        = 0x00,
  granularity_4k        = 0x80,
};

#define gdt_limit_low(limit) (limit & 0xffff)
#define gdt_base_low(base)   (base & 0xffff)
#define gdt_base_mid(base)   ((base >> 16) & 0xff)
#define gdt_flags_limit(limit, flags) (((limit >> 16) & 0xf) | (flags & 0xf0))
#define gdt_base_high(base)  ((base >> 24) & 0xff)

#define gdt_entry(base, limit, access, flags) {\
    gdt_limit_low(limit),		       \
      gdt_base_low(base),		       \
      gdt_base_mid(base),		       \
      (u8)access,			       \
      gdt_flags_limit(limit, flags),	       \
      gdt_base_high(base)		       \
}

class __gdt final {
public:
  __gdt();
  auto init() -> void;

private:
  __gdtr     m_gdtr;
  __gdt_desc m_gdt[0x2000];

};

extern __gdt gdt;
