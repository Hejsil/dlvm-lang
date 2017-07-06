#pragma once
#include <inttypes.h>

enum { __compiler_int, __compiler_float, __compiler_undefined };
typedef struct { uint8_t kind; union { int64_t ivalue; double fvalue; }; } __compiler_value;