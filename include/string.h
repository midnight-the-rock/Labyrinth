#ifndef __STRING_HEADER
#define __STRING_HEADER

#include <types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

  void* memset(void* data, u32 value, u64 size);

#ifdef __cplusplus
}
#endif

#endif
