//// Kernel's printf implementation code

#include <stdio.h>

extern "C"
auto printf(const char* __restrict__ format, ...) -> i32 {
  va_list  arguments;
  va_start(arguments, format);

  auto status = vprintf(format, arguments);

  va_end(arguments);

  return status;
}
