#include "multiboot.h"

namespace mb_header = multiboot::header;

MULTIBOOT_SECTION
auto header = mb_header::make_entry(mb_header::arch_i386);

// MULTIBOOT_SECTION
// auto tag_fb = mb_header::make_tag_framebuffer(1280, 800, 32);
