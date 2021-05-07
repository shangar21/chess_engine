#include <stdio.h>
#include <stdlib.h>

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

  U64 get_rook_occupancy(U64 location, PieceType piece) {
    U64 mask = 0;
    // vertical above piece
    for (int i = location + 8; i <= 63 - 8; i += 8) {
      mask |= (((U64)1) << i);
    }
    // vertical below piece
    for (int i = location - 8; i >= 8; i -= 8) {
      mask |= (((U64)1) << i);
    }
    // horizontal to the right
    for (int i = location + 1; i % 8 <= 7 && i % 8 != 0; i++) {
      mask |= (((U64)1) << i);
    }
    // horizontal to the left
    for (int i = location - 1; i % 8 <= 7 && i & 8 != 0; i--) {
      mask |= (((U64)1) << i);
    }
    return mask;
  }

  U64 get_bishop_occupancy(U64 location, PieceType piece) { return 0; }

  U64 get_queen_occupancy(U64 location, PieceType piece) {
    return get_rook_occupancy(location, piece) |
           get_bishop_occupancy(location, piece);
  }
};
