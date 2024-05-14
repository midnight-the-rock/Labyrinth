//// Multiboot tags, used for the multiboot data pointer to retrieve information

#pragma once

#include <types.h>
#include <util/optional.h>

namespace multiboot {

  namespace tag {

    // multiboot tag type, used to retrieve specific tags from multiboot::tag::get<T>()

    enum class type {
      end              = 0,
      cmdline          = 1,
      boot_loader_name = 2,
      mb_module        = 3,
      memory_info      = 4,
      bootdev          = 5,
      memory_map       = 6,
      vbe              = 7,
      framebuffer      = 8,
      elf_sections     = 9,
      apm              = 10,
      efi32            = 11,
      efi64            = 12,
      smbios           = 13,
      acpi_old         = 14,
      acpi_new         = 15,
      network          = 16,
      efi_mmap         = 17,
      efi_bs           = 18,
      efi32_ih         = 19,
      efi64_ih         = 20,
      load_base_addr   = 21 
    };

    // base form of a multiboot tag, usually used for multiboot information pointer only
    // but you may need it for other instance where it seems reasonable to use it

    struct packed {
      u32 type;
      u32 size;
    };

    // contains basic memory information found by the bootloader
    // it will tell the amount of lower and upper memory on the system
    // lower memory starts at 0
    // upper memory starts at 1Mb

    struct memory_info {
      static constexpr u32 id = (u32)tag::type::memory_info;
      
      u32 type;
      u32 size;
      u32 mem_lower;
      u32 mem_upper;
    };

    // indicates the areas of memory that can be accessed
    // entry.type == 1 for available memory areas
    // it also implements end() and begin() for easy iteration over memory maps

    struct memory_map {
      static constexpr u32 id = (u32)tag::type::memory_map;

      u32 type;
      u32 size;
      u32 entry_size;
      u32 entry_version;

      struct entry {
	u64 addr;
	u64 length;
	u32 type;
	u32 zero;	
      } entries[0];

      auto end() -> entry* {
	auto offset = reinterpret_cast<tag::packed*>(this)->size / this->entry_size;

	return entries + offset;
      }

      auto begin() -> entry* {
	return entries;
      }

    };

#define __not_info_ptr_end(info_ptr)   info_ptr->type != (u32)tag::type::end
#define __increment_info_ptr(info_ptr) info_ptr = (tag::packed*)((u8*)info_ptr + ((info_ptr->size + 7) & ~7))

    template <typename T> [[gnu::always_inline]] 
    inline auto get(tag::packed* mb_info) -> util::optional<T*> {
      while (__not_info_ptr_end(mb_info)) {
	if (mb_info->type == T::id) {
	  return reinterpret_cast<T*>(mb_info);
	}

	__increment_info_ptr(mb_info);
      }  

      return {};
    }

  }

}
