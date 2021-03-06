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

struct Move {
  U8 prev, next;
};

struct Pieces {
  U64 pawns;
  U64 rooks;
  U64 knights;
  U64 bishops;
  U64 queens;
  U64 king;
  U8 pinType[64];
};

#define U64_clz(X) __builtin_clzll(X)
#define row(location) (63 - U64_clz(location)) / 8
#define col(location) (63 - U64_clz(location)) % 8
#define index(location) (63 - U64_clz(location))
#define LSBIT(x) ((x) & -(x))
#define isSingleBit(x) (x && (x == LSBIT(x)))

#endif
