#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "precomputedMasks.h"
#include "types.h"

enum PieceType { NONE, Pawn, Rook, Knight, Bishop, Queen, King };

struct Move {
  char prev, next;
};

struct Pieces {
  U64 pawns;
  U64 rooks;
  U64 knights;
  U64 bishops;
  U64 queens;
  U64 king;
};

class Board {
 public:
  Pieces whitePieces;
  Pieces blackPieces;
  U64 bitboard;

  Board() {
    whitePieces = {0x00FF000000000000U, 0x8100000000000000U,
                   0x4200000000000000U, 0x2400000000000000U,
                   0x0800000000000000U, 0x1000000000000000U};
    blackPieces = {0x000000000000FF00U, 0x0000000000000080U,
                   0x0000000000000042U, 0x0000000000000024U,
                   0x0000000000000008U, 0x0000000000000010U};
  }

  U64 getAllPieces(bool white) {
    return (white)
               ? (whitePieces.pawns | whitePieces.rooks | whitePieces.knights |
                  whitePieces.bishops | whitePieces.queens | whitePieces.king)
               : (blackPieces.pawns | blackPieces.rooks | blackPieces.knights |
                  blackPieces.bishops | blackPieces.queens | blackPieces.king);
  }

  // location of square you want to check, and color of attacking piece
  bool isRookAttacking(U64 location, bool white) {
    U64 opponentRooks = (white) ? blackPieces.rooks : whitePieces.rooks;
    U64 allxOppRooks = (getAllPieces(whitePieces) | getAllPieces(blackPieces)) & ~opponentRooks;
    return 0;
  }
};
