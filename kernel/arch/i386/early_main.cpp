#include <stdio.h>
#include <types.h>

extern "C"
auto early_main(u32 mb_magic, u32 mb_info) -> void {
  printf("%x", mb_magic);
}
