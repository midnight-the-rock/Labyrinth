//// Kernel's vprintf implementation code

#include <stdio.h>

auto print_string(const char* str) -> void {
  while (*str != 0) {
    putchar(*str);
    str++;
  }
}

auto print_integer_base(i16 base) -> void {
  putchar('0');
  
  switch (base) {
  case 2:
    putchar('b');
    break;
    
  case 16:
    putchar('x');
    break;

  default:
    putchar('\b');
    break;
  }
}

static auto print_integer(i16 base, i64 num) -> void {
  static const char* digits = "0123456789abcdef";

  if (num < 0) {
    putchar('-');
    num = -num;
  }

  if (num < 10 && base != 2) {
    print_integer_base(base);
    putchar(digits[num]);
    return;
  }

  i64 rem       {};
  i64 index     {};
  i64 array[21] {};

  while (num != 0) {
    rem = num % base;
    num = num / base;

    array[index] = rem;
    index++;
  }

  index--;
  print_integer_base(base);

  while (index >= 0) {
    putchar(digits[array[index]]);
    index--;
  }
}

static const char* printf_numbers = "0123456789abcdef";

auto print_hex_32(u32 value) -> void {
  // print the hex prefix
  putchar('0');
  putchar('x');

  // quick exit in case no operations are necessary
  if (value < 10) {
    putchar(printf_numbers[value]);
    return;
  }

  i16 index     {};
  u64 items[16] {}; 
  u64 remaining {};

  // extract each digit from value and store on an array for later use
  while (value != 0) {
    remaining = value % 16;
    value     = value / 16;

    items[index] = remaining;
    index++;
  }

  while (index >= 0) {
    putchar(printf_numbers[items[index]]);
    index--;
  }
}

auto print_hex_64(u64 value) -> void {
  // print the hex prefix
  putchar('0');
  putchar('x');

  // quick exit in case no operations are necessary
  if (value < 10) {
    putchar(printf_numbers[value]);
    return;
  }

  i16 index     {};
  u64 items[16] {}; 
  u64 remaining {};

  // extract each digit from value and store on an array for later use
  while (value != 0) {
    remaining = value % 16;
    value     = value / 16;

    items[index] = remaining;
    index++;
  }

  while (index >= 0) {
    putchar(printf_numbers[items[index]]);
    index--;
  }
}

extern "C"
auto vprintf(const char* __restrict__ format, va_list arguments) -> i32 {
  for (const char* i = format; *i != 0; i++) {
    if (*i == '%') {
      switch (*(i + 1)) {
      case '%':
	putchar('%');
	break;

      case 's':
	print_string(va_arg(arguments, const char*));
	break;

      case 'l':
	print_integer(10, va_arg(arguments, i64));
	break;

      case 'u':
	print_integer(10, va_arg(arguments, u32));
	break;

      case 'd':
	print_integer(10, va_arg(arguments, i32));
	break;

      case 'b':
	print_integer(2, va_arg(arguments, u32));
	break;

      case 'B':
	print_integer(2, va_arg(arguments, u64));
	break;

      case 'x':
	print_hex_32(va_arg(arguments, u32));
	break;

      case 'X':
	print_hex_64(va_arg(arguments, u64));
	break;

      case 'c':
	putchar(va_arg(arguments, i32));
	break;

      default:
	break;
      }

      i++;
      continue;
    }

    putchar(*i);
  }

  // no actual error reporting for now, include return type for completeness 
  return 0;
}
