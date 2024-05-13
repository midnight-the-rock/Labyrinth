#include <types.h>

u64* __stack_chk_guard = (u64*)0x595e9fbd94fda766;

extern "C" [[noreturn]]
auto __stack_chk_fail() -> void {
  __builtin_unreachable();
}
