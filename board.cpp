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
  PieceType whitePieces[64] = { NONE };
  PieceType blackPieces[64] = { NONE };
  U64 whitePiecesIndexes;
  U64 blackPiecesIndexes;
  U64 bitboard;

  Board() {
    whitePieces = 18446462598732840960;
    blackPieces = 65535;

  }

  std::vector<Move> getMovesByPiece(std::vector<Move> &moves, PieceType pieceType) {
    switch (pieceType) {

    }
  	// Assume you are not in check
  }
};
