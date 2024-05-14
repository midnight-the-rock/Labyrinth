//// memset implementation

// this is simply following what the OpenGroup posix specification says
// refer there to know more implementation details
//
// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memset.html

#include <string.h>

extern "C" [[maybe_unused]]
auto memset(void* data, u32 value, u64 size) -> void* {
  auto buffer = reinterpret_cast<unsigned char*>(data);

  for (u64 i = 0; i < size; i++) {
    buffer[i] = static_cast<unsigned char>(value);
  }

  return data;
}
