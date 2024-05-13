#pragma once

#include <cpu.h>
#include <stdio.h>

namespace util {
  [[noreturn, gnu::always_inline]]
  inline auto panic(const char* __restrict__ reason) -> void {
    if (reason[0] != 0) {
      printf("[ ERR ]\t%s\n", reason);
    }

    printf("[ INF ] kernel panic");
    cpu::halt();
  }
}
