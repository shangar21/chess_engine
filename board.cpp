#include <stdio.h>
#include <stdlib.h>

#include <vector>

enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

struct Piece {
  PieceType type;
  char row, col;
  int lastMove;
};

struct Move {
  char prevRow, prevCol, nextRow, nextCol
};

class Board {
 public:
  Piece *board[8][8];
  std::vector<Piece> whitePieces;
  std::vector<Piece> blackPieces;

  Board() {
    for (char col = 0; col < 8; ++col) {
      whitePieces.push_back({Pawn, 6, col, 0});
      blackPieces.push_back({Pawn, 1, col, 0});
    }
    whitePieces.push_back({Rook, 7, 0, 0});
    whitePieces.push_back({Knight, 7, 1, 0});
    whitePieces.push_back({Bishop, 7, 2, 0});
    whitePieces.push_back({Queen, 7, 3, 0});
    whitePieces.push_back({King, 7, 4, 0});
    whitePieces.push_back({Bishop, 7, 5, 0});
    whitePieces.push_back({Knight, 7, 6, 0});
    whitePieces.push_back({Rook, 7, 7, 0});
    blackPieces.push_back({Rook, 0, 0, 0});
    blackPieces.push_back({Knight, 0, 1, 0});
    blackPieces.push_back({Bishop, 0, 2, 0});
    blackPieces.push_back({Queen, 0, 3, 0});
    blackPieces.push_back({King, 0, 4, 0});
    blackPieces.push_back({Bishop, 0, 5, 0});
    blackPieces.push_back({Knight, 0, 6, 0});
    blackPieces.push_back({Rook, 0, 7, 0});

    std::fill(begin(board), end(board), nullptr);

    for (Piece &p : whitePieces) {
      board[p.row][p.col] = &p;
    }
    for (Piece &p : blackPieces) {
      board[p.row][p.col] = &p;
    }
  }

  std::vector<Move> getPawnMoves(std::vector<Move> &moves) {
  	// Assume you are not in check
  }
};
