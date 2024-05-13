#ifndef __STDIO_HEADER
#define __STDIO_HEADER

#include <types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

  u8 getchar();

  i32 printf(const char* __restrict__ format, ...);
  i32 putchar(u8 chr);

  i32 puts(const char* str);
  
  i32 vprintf(const char* __restrict__ format, va_list arguments);


#ifdef __cplusplus
}
#endif

#endif
