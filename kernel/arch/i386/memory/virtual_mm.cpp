#include <stdio.h>
#include <string.h>
#include <memory/definitions.h>

namespace memory {

  [[gnu::always_inline]]
  inline auto map_address(page_table table, u32 physical_addr, u32 virtual_addr) -> void {
    memset(table, 0x00, PAGES_PER_TABLE);

    for (u32 i = 0; i < PAGES_PER_TABLE; i++) {
      auto page_entry = &table[ pt_index(virtual_addr) ];

      page_entry->set_frame(reinterpret_cast<void*>(physical_addr));
      page_entry->add_attribute(pt_entry_flag::present);
      page_entry->add_attribute(pt_entry_flag::writeable);

      virtual_addr  += PAGE_SIZE;
      physical_addr += PAGE_SIZE;
    }
  };

  [[gnu::always_inline]]
  inline auto map_table(page_directory dir, page_table table) -> void {
    dir->add_attribute(pd_entry_flag::present);
    dir->add_attribute(pd_entry_flag::writeable);
    dir->set_frame(table);
  }


  // page table entry implementation

  pt_entry::pt_entry() = default;

  auto pt_entry::data() -> u32* {
    return &m_entry;
  }

  auto pt_entry::get_frame() -> u32 {
    return m_entry & (u32)pt_entry_flag::frame;
  }

  auto pt_entry::set_frame(void* physical_addr) -> void {
    m_entry = (m_entry & ~((u32)pt_entry_flag::frame)) | (u32)physical_addr;
  }

  auto pt_entry::is_present_p() -> bool {
    return m_entry & (u32)pt_entry_flag::present;
  }

  auto pt_entry::is_writeable_p() -> bool {
    return m_entry & (u32)pt_entry_flag::writeable;
  }

  auto pt_entry::add_attribute(pt_entry_flag attribute) -> void {
    m_entry |=  (u32)attribute;
  }

  auto pt_entry::del_attribute(pt_entry_flag attribute) -> void {
    m_entry &= ~(u32)attribute;
  }

  // page directory entry implementation

  pd_entry::pd_entry() = default;

  auto pd_entry::get_frame() -> u32 {
    return m_entry & (u32)pd_entry_flag::frame;
  }

  auto pd_entry::set_frame(void* physical_addr) -> void {
    m_entry = (m_entry & ~((u32)pd_entry_flag::frame)) | (u32)physical_addr;
  }

  auto pd_entry::is_user_p() -> bool {
    return m_entry & (u32)pd_entry_flag::user;
  }

  auto pd_entry::is_present_p() -> bool {
    return m_entry & (u32)pd_entry_flag::present;
  }

  auto pd_entry::is_writeable_p() -> bool {
    return m_entry & (u32)pd_entry_flag::writeable;
  }

  auto pd_entry::add_attribute(pd_entry_flag attribute) -> void {
    m_entry |=  (u32)attribute;
  }

  auto pd_entry::del_attribute(pd_entry_flag attribute) -> void {
    m_entry &= ~(u32)attribute;
  }

  // virtual memory manager implementation 

  __virtual_mm virtual_mm;

  auto __virtual_mm::init() -> void {
    m_directory  = reinterpret_cast<page_directory>(physical_mm.alloc(3));

    // clear the directory table
    memset(m_directory, 0x00, TABLES_PER_DIRECTORY);

    map_page((void*)0x10000, (void*)0xc0000000);
    map_page((void*)0x00000, (void*)0x00000000); 
    map_page(physical_mm.mmap_begin(), physical_mm.mmap_begin());

    // load page directory and enable paging
    load_page_directory(m_directory);
    enable_paging();

    printf("[ INF ]\tpaging enabled\n");
  }

  auto __virtual_mm::alloc() -> void* {
    return nullptr;
  }

  auto __virtual_mm::free(void* addr) -> void {
    auto physical_addr = reinterpret_cast<void*>(virtual_to_physical_addr(addr));

    physical_mm.dealloc(physical_addr);
  }

  auto __virtual_mm::map_page(void* physical_addr, void* virtual_addr) -> void {
    auto entry = &m_directory[ pd_index((u32)virtual_addr) ];
    auto table = reinterpret_cast<page_table>(physical_mm.alloc());

    // clear the page table
    memset(table, 0x00, PAGES_PER_TABLE);

    for (u32 i = 0; i < PAGES_PER_TABLE; i++) {
      auto page_entry = &table[ pt_index((u32)virtual_addr) ];

      page_entry->set_frame(physical_addr);
      page_entry->add_attribute(pt_entry_flag::present);
      page_entry->add_attribute(pt_entry_flag::writeable);

      virtual_addr  = reinterpret_cast<void*>((u32)virtual_addr  + PAGE_SIZE);
      physical_addr = reinterpret_cast<void*>((u32)physical_addr + PAGE_SIZE);
    }

    entry->set_frame(table);
    entry->add_attribute(pd_entry_flag::present);
    entry->add_attribute(pd_entry_flag::writeable);
  }
}
