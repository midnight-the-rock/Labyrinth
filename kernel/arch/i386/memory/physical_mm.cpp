#include <stdio.h>
#include <string.h>
#include <util/panic.h>
#include <memory/physical_mm.h>
#include <memory/definitions.h>

namespace memory {

  __physical_mm physical_mm;

  auto __physical_mm::init(multiboot::tag::packed* mb_info) -> void {
    namespace mb_tag = multiboot::tag;

    auto kernel_size = reinterpret_cast<u32>(end_of_kernel);

    auto memory_map  = mb_tag::get<mb_tag::memory_map>(mb_info);
    auto memory_info = mb_tag::get<mb_tag::memory_info>(mb_info);

    auto memory_size = memory_info.value()->mem_lower + memory_info.value()->mem_upper;

    init_bitmap(memory_size, kernel_size * 512);

    for (auto mmap : *memory_map.value()) { 
      if (mmap.type == 1 && mmap.addr >= 0x100000) {
	init_region(mmap.addr, mmap.length);
      }
    }

    disable_region(0x100000, kernel_size * 512);

    printf("[ INF ]\t%u total memory blocks\n", blocks_total());
    printf("[ INF ]\t%u memory blocks are free to be used\n", blocks_free());
  }

  auto __physical_mm::init_bitmap(u32 msize, u32 mmap) -> void {
    m_memory_map    = (u32*)mmap;
    m_memory_size   = msize;

    m_blocks_max    = (msize * 1024) / MEMORY_BLOCK_SIZE;
    m_blocks_in_use = m_blocks_max;

    memset(m_memory_map, 0xffffffff, m_blocks_max / MEMORY_BLOCKS_PER_BYTE);
  }

  auto __physical_mm::init_region(u32 base, u32 size) -> void {
    u32 align  = base / MEMORY_BLOCK_SIZE;
    u32 blocks = size / MEMORY_BLOCK_SIZE;

    while (blocks > 0) {
      mmap_unset(align++);
      m_blocks_in_use--;

      blocks--;
    }

    mmap_set(0);
  }

  auto __physical_mm::blocks_free() -> u32 {
    return m_blocks_max - m_blocks_in_use;
  }

  auto __physical_mm::blocks_total() -> u32 {
    return m_blocks_max;
  }

  auto __physical_mm::disable_region(u32 base, u32 size) -> void {
    u32 align  = base / MEMORY_BLOCK_SIZE;
    u32 blocks = size / MEMORY_BLOCK_SIZE;

    while (blocks > 0) {
      mmap_set(align++);
      m_blocks_in_use++;

      blocks--;
    }

    mmap_set(0);
  }

  auto __physical_mm::alloc() -> void* {
    if (m_blocks_in_use <= 0) {
      util::panic("out of memory");
    }

    auto frame = mmap_first_free();

    if (!frame.has_value()) {
      util::panic("out of memory");
    }

    if (frame.value() == 0) {
      util::panic("out of memory");
    }

    mmap_set(frame.value());
    m_blocks_in_use++;

    return reinterpret_cast<void*>(frame.value() * MEMORY_BLOCK_SIZE);
  }

  auto __physical_mm::alloc(u64 blocks) -> void* {
    if (m_blocks_in_use < blocks) {
      util::panic("out of memory");
    }

    auto frame = mmap_first_free();

    if (!frame.has_value()) {
      util::panic("out of memory");
    }

    if (frame.value() == 0) {
      util::panic("out of memory");
    }

    for (u32 i = 0; i < blocks; i++) {
      mmap_set(frame.value() + i);
    }

    m_blocks_in_use += blocks;

    return reinterpret_cast<void*>(frame.value() * MEMORY_BLOCK_SIZE);
  }

  auto __physical_mm::dealloc(void* block) -> void {
    u32 frame = (u32)block / MEMORY_BLOCK_SIZE;

    mmap_unset(frame);
    m_blocks_in_use--;
  }

  auto __physical_mm::dealloc(void* block, u64 blocks) -> void {
    u32 frame = (u32)block / MEMORY_BLOCK_SIZE;

    for (u32 i = 0; i < blocks; i++) {
      mmap_unset(frame + i);
    }

    m_blocks_in_use -= blocks;
  }

  auto __physical_mm::mmap_first_free() -> util::optional<u32> {
    for (u32 i = 0; i < (m_blocks_max / 32); i++) {
      if (m_memory_map[i] != 0xffffffff) {
	for (u32 j = 0; j < 32; j++) {
	  u32 bit = 1 << j;
	  
	  if (!(m_memory_map[i] & bit)) {
	    return (i * 4 * 8 + j);
	  }
	}
      }
    }

    return {};
  }

  auto __physical_mm::mmap_end() -> void* {
    return &m_memory_map[m_blocks_max / 32];
  }

  auto __physical_mm::mmap_set(u32 bit) -> void {
    m_memory_map[bit / 32] |= (1 << (bit % 32));
  }

  auto __physical_mm::mmap_unset(u32 bit) -> void {
    m_memory_map[bit / 32] &= ~(1 << (bit % 32));
  }

  auto __physical_mm::mmap_check_bit(u32 bit) -> bool {
    return m_memory_map[bit / 32] & (1 << (bit % 32));
  }

}
