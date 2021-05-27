#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <algorithm>

#include "precomputedMasks.h"
#include "testing.h"
#include "types.h"

enum PieceType { NONE, Pawn, Rook, Knight, Bishop, Queen, King };

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

class Board {
 public:
  Pieces whitePieces;
  Pieces blackPieces;
  U64 bitboard;

  Board() {
    whitePieces = {0x00FF000000000000U, 0x8100000000000000U,
                   0x4200000000000000U, 0x2400000000000000U,
                   0x0800000000000000U, 0x1000000000000000U};
    blackPieces = {0x000000000000FF00U, 0x0000000000000081U,
                   0x0000000000000042U, 0x0000000000000024U,
                   0x0000000000000008U, 0x0000000000000010U};
    std::fill(std::begin(whitePieces.pinType), std::end(whitePieces.pinType), 0);
    std::fill(std::begin(blackPieces.pinType), std::end(blackPieces.pinType), 0);
  }

  operator std::string() {
    std::string res;
    U64 idx = 1;
    for (int row = 0; row < 8; ++row) {
      for (int col = 0; col < 8; ++col) {
        if (idx & whitePieces.pawns)
          res += u8"♙";
        else if (idx & whitePieces.rooks)
          res += u8"♖";
        else if (idx & whitePieces.knights)
          res += u8"♘";
        else if (idx & whitePieces.bishops)
          res += u8"♗";
        else if (idx & whitePieces.queens)
          res += u8"♕";
        else if (idx & whitePieces.king)
          res += u8"♔";
        else if (idx & blackPieces.pawns)
          res += u8"♟︎";
        else if (idx & blackPieces.rooks)
          res += u8"♜";
        else if (idx & blackPieces.knights)
          res += u8"♞";
        else if (idx & blackPieces.bishops)
          res += u8"♝";
        else if (idx & blackPieces.queens)
          res += u8"♛";
        else if (idx & blackPieces.king)
          res += u8"♚";
        else
          res += u8"_";
        res += u8"|";
        idx <<= 1;
      }
      res += u8"\n";
    }
    return res;
  };

  U64 getAllPieces(bool white) {
    return (white)
               ? (whitePieces.pawns | whitePieces.rooks | whitePieces.knights |
                  whitePieces.bishops | whitePieces.queens | whitePieces.king)
               : (blackPieces.pawns | blackPieces.rooks | blackPieces.knights |
                  blackPieces.bishops | blackPieces.queens | blackPieces.king);
  }

  // location of square you want to check, color of that piece
  bool isPieceAttacked(const U64 &location, const bool white) {
    const Pieces &opponents = white ? blackPieces : whitePieces;
    U64 allPieces, temp;

    allPieces = getAllPieces(false) | getAllPieces(true);

    int locIdx, locRow, locCol, pieceIdx, pieceRow, pieceCol;

    locIdx = index(location);
    locRow = locIdx / 8;
    locCol = locIdx % 8;

    if (knightMasks[locIdx] & opponents.knights) return true;

    temp = (opponents.rooks | opponents.queens) & rookMasks[locIdx];
    U64 piece, inBetween;

    while (temp) {
      piece = LSBIT(temp);
      pieceIdx = index(piece);
      pieceRow = pieceIdx / 8;
      pieceCol = pieceIdx % 8;

      if (pieceRow == locRow) {
        inBetween = (pieceCol < locCol)
                        ? seekR[pieceIdx] & seekL[locIdx] & allPieces
                        : seekR[locIdx] & seekL[pieceIdx] & allPieces;
        if (!inBetween) return true;
      } else if (pieceCol == locCol) {
        inBetween = (pieceRow < locRow)
                        ? seekD[pieceIdx] & seekU[locIdx] & allPieces
                        : seekD[locIdx] & seekU[pieceIdx] & allPieces;
        if (!inBetween) return true;
      }
      temp ^= piece;
    }

    temp = (opponents.bishops | opponents.queens) & bishopMasks[locIdx];

    while (temp) {
      piece = LSBIT(temp);
      pieceIdx = index(piece);
      pieceRow = pieceIdx / 8;
      pieceCol = pieceIdx % 8;

      if (locRow - pieceRow == locCol - pieceCol) {
        inBetween = (pieceRow < locRow)
                        ? seekDR[pieceIdx] & seekUL[locIdx] & allPieces
                        : seekDR[locIdx] & seekUL[pieceIdx] & allPieces;
        if (!inBetween) return true;
      } else if (locRow - pieceRow == pieceCol - locCol) {
        inBetween = (pieceRow < locRow)
                        ? seekDL[pieceIdx] & seekUR[locIdx] & allPieces
                        : seekDL[locIdx] & seekUR[pieceIdx] & allPieces;
        if (!inBetween) return true;
      }
      temp ^= piece;
    }
    return false;
  }

  void calculatePins(bool white) {
    return;
  }
};

int main() {
  Board board;
  std::cout << std::string(board) << std::endl;
  return 0;
}
