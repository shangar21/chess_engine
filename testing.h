#ifndef TESTING_H
#define TESTING_H

#include "types.h"

void printMask(U64 mask) {
  U64 i = 1;
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      if (i & mask)
        std::cout << '#';
      else
        std::cout << '_';
      i <<= 1;
    }
    std::cout << std::endl;
  }
}

#endif