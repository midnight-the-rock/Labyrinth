//// some definitions required by memory utilities

//   they are here just so that they don't get included
//   on places that do not have anything related to
//   implementation details

#pragma once

#include <types.h>
#include <string.h>
#include <util/array.h>
#include <util/panic.h>
#include <memory/virtual_mm.h>
#include <memory/physical_mm.h>

// physical memory manager related

constexpr u32 MEMORY_BLOCK_SIZE      = 4096;
constexpr u32 MEMORY_BLOCKS_PER_BYTE = 8;

extern "C" u8 end_of_kernel[]; // determined by the linker

// virtual memory manager related

constexpr u32 PAGE_SIZE            = 4096;
constexpr u32 PAGE_TABLE_SIZE      = 0x400000;
constexpr u64 PAGE_DIRECTORY_SIZE  = 0x100000000;

constexpr u32 PAGES_PER_TABLE      = 1024;
constexpr u32 TABLES_PER_DIRECTORY = 1024;

[[gnu::always_inline]]
inline auto pt_index(u32 addr) -> u32 {
  return (addr >> 12) & 0x3fff;
}

[[gnu::always_inline]]
inline auto pd_index(u32 addr) -> u32 {
  return (addr >> 22);
}

[[gnu::always_inline]]
inline auto virtual_to_physical_addr(void* virt_addr) -> u32 {
  return (*(u32*)virt_addr & ~0xfff);
}

[[gnu::always_inline]]
inline auto load_page_directory(void* directory) -> void {
  asm volatile ("mov %0, %%cr3" :: "a"(directory));
}

[[gnu::always_inline]]
inline auto enable_paging() -> void {
  asm volatile ("mov %cr0, %eax;       "   
		"or  $0x80000001, %eax;"
		"mov %eax, %cr0;       ");
}
