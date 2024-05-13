#include "multiboot.h"

MULTIBOOT_SECTION
auto header = multiboot::header::make_entry(MULTIBOOT_ARCHITECTURE_I386);

// MULTIBOOT_SECTION
// auto tag_fb = multiboot::header::make_tag_framebuffer(1280, 800, 32);
