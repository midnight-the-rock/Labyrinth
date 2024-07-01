//// virtual memory manager

// just in case I need to ever revisit this
// I'll leave some useful information regarding paging
//
// http://www.brokenthorn.com/Resources/OSDev18.html
// https://wiki.osdev.org/Paging

#pragma once

#include <types.h>
#include <memory/definitions.h>

namespace memory {

  enum class pt_entry_flag {  // page table entry flags
    present       = 0x01,
    writeable     = 0x02,
    user          = 0x04,
    writethrough  = 0x08,
    not_cacheable = 0x10,
    accessed      = 0x20,
    dirty         = 0x40,
    pat           = 0x80,
    cpu_global    = 0x100,
    lv4_global    = 0x200,
    frame         = 0x7ffff000
  };
  
  enum class pd_entry_flag { // page directory entry flags
    present       = 0x01,
    writeable     = 0x02,
    user          = 0x04,
    writethrough  = 0x08,
    not_cacheable = 0x10,
    accessed      = 0x20,
    dirty         = 0x40,
    long_page     = 0x80, // 4MB pages
    cpu_global    = 0x100,
    lv4_global    = 0x2000,
    frame         = 0x7ffff000
  };

  class pt_entry final {
  public:
    pt_entry();

    auto data() -> u32*;

    auto get_frame() -> u32;
    auto set_frame(void* physical_addr) -> void;

    auto is_present_p() -> bool;
    auto is_writeable_p() -> bool;
    
    auto add_attribute(pt_entry_flag attribute) -> void;
    auto del_attribute(pt_entry_flag attribute) -> void;

  private:
    u32 m_entry {};
  };

  class pd_entry final {
  public:
    pd_entry();

    auto get_frame() -> u32;
    auto set_frame(void* physical_addr) -> void;

    auto is_user_p() -> bool;
    auto is_present_p() -> bool;
    auto is_writeable_p() -> bool;

    auto add_attribute(pd_entry_flag attribute) -> void;
    auto del_attribute(pd_entry_flag attribute) -> void;

  public:
    u32 m_entry {};
  };

  typedef pt_entry* page_table;
  typedef pd_entry* page_directory;

  class __virtual_mm final {
  public:
    auto init() -> void;

    auto alloc() -> void*;
    auto free(void* addr) -> void;

    auto map_page(void* physical_addr, void* virtual_addr) -> void;

  private:
    page_directory m_directory;
  };

  extern __virtual_mm virtual_mm;
}
