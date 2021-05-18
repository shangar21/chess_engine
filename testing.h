#ifndef TESTING_H
#define TESTING_H

#include "types.h"
#include <bitset>
#include <iostream>

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
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void print_binary(U64 num) {
    std::cout << "binary repr: " << std::bitset<8 * sizeof(num)>(num) << std::endl;
}



#endif