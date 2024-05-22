//// Physical memory manager implementation 

// here some references that make more sence than this:
// 
// http://www.brokenthorn.com/Resources/OSDev17.html
//

#pragma once

#include <types.h>
#include <util/optional.h>
#include <multiboot/multiboot.h>

namespace memory {

  class __physical_mm final {
  public:
    auto init(multiboot::tag::packed* mb_info) -> void;
    auto init_bitmap(u32 msize, u32 mmap) -> void;
    auto init_region(u32 base, u32 size) -> void;
    auto disable_region(u32 base, u32 size) -> void;

    auto blocks_free() -> u32;
    auto blocks_total() -> u32;

    auto alloc() -> void*;
    auto alloc(u64 blocks) -> void*;

    auto dealloc(void* addr) -> void;
    auto dealloc(void* addr, u64 blocks) -> void;

    auto mmap_first_free() -> util::optional<u32>;

    auto mmap_end() -> void*;

    auto mmap_set(u32 bit) -> void;
    auto mmap_unset(u32 bit) -> void;
    auto mmap_check_bit(u32 bit) -> bool;

  private:
    u32* m_memory_map;
    u32  m_memory_size;

    u32  m_blocks_max;
    u32  m_blocks_in_use;

  };

  extern __physical_mm physical_mm;
}
