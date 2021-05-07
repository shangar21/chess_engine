

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

#define U64_clz(X) __builtin_clzll(X)

#endif
