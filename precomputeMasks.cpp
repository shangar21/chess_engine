#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "types.h"
#include "testing.h"

U64 getSeekR(U64 location) {
  U64 mask = 0;
  for (U64 i = location << 1; i && U64_clz(i) % 8 != 7; i <<= 1) {
    mask |= i;
  }
  return mask;
}

U64 getSeekUR(U64 location) {
  U64 mask = 0;
  for (U64 i = location >> 7; i && U64_clz(i) % 8 != 7; i >>= 7) {
    mask |= i;
  }
  return mask;
}

U64 getSeekU(U64 location) {
  U64 mask = 0;
  for (U64 i = location >> 8; i; i >>= 8) {
    mask |= i;
  }
  return mask;
}

U64 getSeekUL(U64 location) {
  U64 mask = 0;
  for (U64 i = location >> 9; i && U64_clz(i) % 8; i >>= 9) {
    mask |= i;
  }
  return mask;
}

U64 getSeekL(U64 location) {
  U64 mask = 0;
  for (U64 i = location >> 1; i && U64_clz(i) % 8; i >>= 1) {
    mask |= i;
  }
  return mask;
}

U64 getSeekDL(U64 location) {
  U64 mask = 0;
  for (U64 i = location << 7; i && U64_clz(i) % 8; i <<= 7) {
    mask |= i;
  }
  return mask;
}

U64 getSeekD(U64 location) {
  U64 mask = 0;
  for (U64 i = location << 8; i; i <<= 8) {
    mask |= i;
  }
  return mask;
}

U64 getSeekDR(U64 location) {
  U64 mask = 0;
  for (U64 i = location << 9; i && U64_clz(i) % 8 != 7; i <<= 9) {
    mask |= i;
  }
  return mask;
}

U64 getRookOccupancy(U64 location) {
  return getSeekR(location) | getSeekU(location) | getSeekL(location) | getSeekD(location);
}

U64 getBishopOccupancy(U64 location) {
  return getSeekUR(location) | getSeekUL(location) | getSeekDL(location) | getSeekDR(location);
}

U64 getQueenOccupancy(U64 location) {
  return getRookOccupancy(location) | getBishopOccupancy(location);
}

U64 getKnightOccupancy(U64 location) {
  U64 mask = 0;
  int knight_row = row(location);
  for (int dRow : {-2, -1, 1, 2}) {
    for (int dCol : {-2, -1, 1, 2}) {
      if (abs(abs(dRow) - abs(dCol)) == 1) {
        int shifts = 8 * dRow + dCol;
        U64 i = (shifts < 0) ? location >> abs(shifts) : location << shifts;
        if (i && abs(knight_row - row(i)) == abs(dRow)) {
          mask |= i;
        }
      }
    }
  }
  return mask;
}

int main() {
  for (U64 idx = 1; idx; idx <<= 1) {
    U64 seekDR = getSeekDR(idx);
    printf("%#018lx,", seekDR);
  }
  printf("\n");
  return 0;
}
