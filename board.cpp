#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include "types.h"

enum PieceType { NONE, Pawn, Rook, Knight, Bishop, Queen, King };

struct Move {
  char prev, next;
};

class Board {
 public:
  PieceType whitePieces[64] = {NONE};  // U64 whitePieces = 0;
  PieceType blackPieces[64] = {NONE};  // U64 whitePieces = 0;
  U64 whitePiecesIndexes;
  U64 blackPiecesIndexes;
  U64 bitboard;

  Board() {
    whitePiecesIndexes = 0xFFFF000000000000U;
    blackPiecesIndexes = 0x000000000000FFFFU;
  }

  void print_mask(U64 mask) {
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

  U64 get_rook_occupancy(U64 location) {
    U64 mask = 0;
    // vertical above piece
    for (U64 i = location >> 8; i; i >>= 8) {
      mask |= i;
    }
    // vertical below piece
    for (U64 i = location << 8; i; i <<= 8) {
      mask |= i;
    }
    // horizontal to the left
    for (U64 i = location >> 1; i && U64_clz(i) % 8; i >>= 1) {
      mask |= i;
    }
    // horizontal to the right
    for (U64 i = location << 1; i && U64_clz(i) % 8 != 7; i <<= 1) {
      mask |= i;
    }
    return mask;
  }

  U64 get_bishop_occupancy(U64 location) {
    U64 mask = 0;
    // Up left
    for (U64 i = location >> 9; i && U64_clz(i) % 8; i >>= 9) {
      mask |= i;
    }
    // Down left
    for (U64 i = location << 7; i && U64_clz(i) % 8; i <<= 7) {
      mask |= i;
    }
    // Up right
    for (U64 i = location >> 7; i && U64_clz(i) % 8 != 7; i >>= 7) {
      mask |= i;
    }
    // Down right
    for (U64 i = location << 9; i && U64_clz(i) % 8 != 7; i <<= 9) {
      mask |= i;
    }
    return mask;
  }

  U64 get_queen_occupancy(U64 location) {
    return get_rook_occupancy(location) | get_bishop_occupancy(location);
  }
};

int main() {
  Board b = Board();
  U64 idx;
  for (idx = 1; idx; idx <<= 1) {
    U64 bishop_mask = b.get_bishop_occupancy(idx);
    b.print_mask(bishop_mask);
    printf(
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    // std::cout << rook_occupancy << ',' << std::endl;
  }
}
