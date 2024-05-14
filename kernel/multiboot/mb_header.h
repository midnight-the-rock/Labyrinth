//// Multiboot header definitions

#pragma once

#include <types.h>

// for multiboot header and header tags declaration
#define MULTIBOOT_SECTION [[gnu::section(".multiboot"), gnu::aligned(0x08)]]

namespace multiboot {

  namespace header {

    constexpr u32 kernel_magic     = 0xe85250d6;
    constexpr u32 bootloader_magic = 0x36d76289;

    /* multiboot entry, needed by the bootloader
       to know we are a multiboot complainant kernel */ 
    struct entry {
      u32 magic;
      u32 arch;
      u32 length;
      u32 checksum;
    };

    /* generate an entry based on arch
       and the rest of the fields are practically the
       same on any architecture (I think) */
    consteval auto make_entry(u32 arch) -> entry {
      constexpr auto size  = static_cast<u32>(sizeof(entry));
      constexpr auto magic = kernel_magic;

      return {
	.magic    = magic,
	.arch     = arch,
	.length   = size,
	.checksum = -(magic + arch + size)
      };
    }

    struct tag_framebuffer {
      u16 type;
      u16 flags;
      u32 size;
      u32 width;
      u32 height;
      u32 depth;
    };

    consteval auto make_tag_framebuffer(u32 width, u32 height, u32 depth) -> tag_framebuffer {
      constexpr auto size = static_cast<u32>(sizeof(tag_framebuffer));

      return {
	.type   = 5,
	.flags  = 1,
	.size   = size,
	.width  = width,
	.height = height,
	.depth  = depth
      };
    }
  }
  
}    
