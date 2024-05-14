#include <gdt.h>

#define kernel_code_access (u8)__gdt_access::present | (u8)__gdt_access::dpl0 | (u8)__gdt_access::code_segment | (u8)__gdt_access::code_readable
#define kernel_code_flags  (u8)__gdt_flags::bit_32   | (u8)__gdt_flags::granularity_4k

#define kernel_data_access (u8)__gdt_access::present | (u8)__gdt_access::dpl0 | (u8)__gdt_access::data_segment | (u8)__gdt_access::data_writeable
#define kernel_data_flags  (u8)__gdt_flags::bit_32   | (u8)__gdt_flags::granularity_4k

#define user_code_access   (u8)__gdt_access::present | (u8)__gdt_access::dpl3 | (u8)__gdt_access::code_segment | (u8)__gdt_access::code_readable
#define user_code_flags    (u8)__gdt_flags::bit_32   | (u8)__gdt_flags::granularity_4k

#define user_data_access   (u8)__gdt_access::present | (u8)__gdt_access::dpl3 | (u8)__gdt_access::data_segment | (u8)__gdt_access::data_writeable
#define user_data_flags    (u8)__gdt_flags::bit_32   | (u8)__gdt_flags::granularity_4k

__gdt gdt;

__gdt::__gdt() {}

extern "C" void reload_cs();

auto __gdt::init() -> void {
  m_gdtr.limit = (sizeof(__gdt_desc) * 0x2000) - 1;
  m_gdtr.base  = &m_gdt[0];

  m_gdt[0] = gdt_entry(0, 0, 0, 0);
  m_gdt[1] = gdt_entry(0, 0xfffff, kernel_code_access, kernel_code_flags);
  m_gdt[2] = gdt_entry(0, 0xfffff, kernel_data_access, kernel_data_flags);
  m_gdt[3] = gdt_entry(0, 0xfffff, user_code_access, user_code_flags);
  m_gdt[4] = gdt_entry(0, 0xfffff, user_data_access, user_data_flags);
  m_gdt[5] = gdt_entry(0, 0, 0, 0);

  cpu::lgdt(&m_gdtr);
  reload_cs();
}
