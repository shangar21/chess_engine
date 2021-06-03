#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "precomputedMasks.h"
#include "testing.h"
#include "types.h"

// Pintype:
// 1 -
// 2 /
// 3 |
// 4 \

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

    std::fill(std::begin(whitePieces.pinType), std::end(whitePieces.pinType),
              0);
    std::fill(std::begin(blackPieces.pinType), std::end(blackPieces.pinType),
              0);
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

  void clear_board(){
    whitePieces = {0, 0,
                   0, 0,
                   0, 0};
    blackPieces = {0, 0,
                   0, 0,
                   0, 0};
    std::fill(std::begin(whitePieces.pinType), std::end(whitePieces.pinType),
              0);
    std::fill(std::begin(blackPieces.pinType), std::end(blackPieces.pinType),
              0);
  }

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

      if (locRow - pieceRow == pieceCol - locCol) {
        inBetween = (pieceRow < locRow)
                        ? seekDL[pieceIdx] & seekUR[locIdx] & allPieces
                        : seekDL[locIdx] & seekUR[pieceIdx] & allPieces;
        if (!inBetween) return true;
      } else if (locRow - pieceRow == locCol - pieceCol) {
        inBetween = (pieceRow < locRow)
                        ? seekDR[pieceIdx] & seekUL[locIdx] & allPieces
                        : seekDR[locIdx] & seekUL[pieceIdx] & allPieces;
        if (!inBetween) return true;
      }
      temp ^= piece;
    }
    return false;
  }

  /*
   * If white calculate which of white's pieces are pinned.
   * Otherwise calculate which of black's pieces are pinned.
   */
  void calculatePins(bool white) {
    Pieces &mine = (white) ? whitePieces : blackPieces;
    Pieces &opponents = (white) ? blackPieces : whitePieces;
    const U64 allMyPieces = getAllPieces(white);
    const U64 allPieces = allMyPieces | getAllPieces(!white);

    int kingIdx, kingRow, kingCol, pieceIdx, pieceRow, pieceCol;

    kingIdx = index(mine.king);
    kingRow = kingIdx / 8;
    kingCol = kingIdx % 8;

    U64 temp, piece, inBetween;
    temp = (opponents.rooks | opponents.queens) & rookMasks[kingIdx];

    while (temp) {
      piece = LSBIT(temp);
      pieceIdx = index(piece);
      pieceRow = pieceIdx / 8;
      pieceCol = pieceIdx % 8;

      if (pieceRow == kingRow) {
        inBetween = (pieceCol < kingCol)
                        ? seekR[pieceIdx] & seekL[kingIdx] & allPieces
                        : seekR[kingIdx] & seekL[pieceIdx] & allPieces;
        if (isSingleBit(inBetween) && inBetween & allMyPieces)
          mine.pinType[index(inBetween)] = 1;
      } else if (pieceCol == kingCol) {
        inBetween = (pieceRow < kingRow)
                        ? seekD[pieceIdx] & seekU[kingIdx] & allPieces
                        : seekD[kingIdx] & seekU[pieceIdx] & allPieces;
        if (isSingleBit(inBetween) && inBetween & allMyPieces)
          mine.pinType[index(inBetween)] = 3;
      }
      temp ^= piece;
    }

    temp = (opponents.bishops | opponents.queens) & bishopMasks[kingIdx];

    while (temp) {
      piece = LSBIT(temp);
      pieceIdx = index(piece);
      pieceRow = pieceIdx / 8;
      pieceCol = pieceIdx % 8;

      if (kingRow - pieceRow == pieceCol - kingCol) {
        inBetween = (pieceRow < kingRow)
                        ? seekDL[pieceIdx] & seekUR[kingIdx] & allPieces
                        : seekDL[kingIdx] & seekUR[pieceIdx] & allPieces;
        if (isSingleBit(inBetween) && inBetween & allMyPieces)
          mine.pinType[index(inBetween)] = 2;
      } else if (kingRow - pieceRow == kingCol - pieceCol) {
        inBetween = (pieceRow < kingRow)
                        ? seekDR[pieceIdx] & seekUL[kingIdx] & allPieces
                        : seekDR[kingIdx] & seekUL[pieceIdx] & allPieces;
        if (isSingleBit(inBetween) && inBetween & allMyPieces)
          mine.pinType[index(inBetween)] = 4;
      }
      temp ^= piece;
    }
  }


  U64 getMoveMask(bool white, U64 location){

      Pieces player = white ? whitePieces : blackPieces;
      int pin = player.pinType[index(location)];
      U64 all = getAllPieces(true)|getAllPieces(false);
      if(isPieceAttacked(player.king)){
        std::cout << "check lol"
      }
      else if (location & player.knights){
        return pin != 0 ? 0 : knightMasks[index(location)];
      }
      else if (location & player.rooks){
        if (pin != 0){
            if (pin == 1){
                if (col(player.king) < col(location)){
                    return 1 << (64 - U64_clz(seekL[index(location)] & getAllPieces(!white)));
                }
                else{
                    return LSBIT(seekR[index(location)] & getAllPieces(!white));
                }
            }
            else if (pin == 3){
                if (row(player.king) < row(location)){
                    return 1 << (64 - U64_clz(seekU[index(location)] & getAllPieces(!white)));
                }
                else{
                    return LSBIT(seekD[index(location)] & getAllPieces(!white));
                }

            }
            return 0;
        }
         U64 mask = 0;
         // going right
         U64 right_block =  LSBIT(seekR[index(location)] & all);
         U64 up_block = (U64)1 << 64 - U64_clz(seekU[index(location)] & all);
         U64 left_block = (U64)1 << 64 - U64_clz(seekL[index(location)] & all);
         U64 down_block =  LSBIT(seekD[index(location)] & all);
         
         mask |= right_block & getAllPieces(!white) ? seekR[index(location)] ^ seekR[index(right_block)] : (seekR[index(location)] ^ seekR[index(right_block)]) ^ right_block;
         mask |= up_block & getAllPieces(!white) ? seekR[index(location)] ^ seekR[index(up_block)] : (seekR[index(location)] ^ seekR[index(up_block)]) ^ up_block; 
         mask |= left_block & getAllPieces(!white) ? seekR[index(location)] ^ seekR[index(left_block)] : (seekR[index(location)] ^ seekR[index(left_block)]) ^ left_block;  
         mask |= down_block & getAllPieces(!white) ? seekR[index(location)] ^ seekR[index(down_block)] : (seekR[index(location)] ^ seekR[index(down_block)]) ^ down_block; 
         
         return mask;
         
      }
          
  } 
};

int main() {
  Board board;
  board.clear_board();
  board.blackPieces.rooks = 1 << 16;
  board.whitePieces.rooks = 1 << 24;
  board.whitePieces.king = (U64)1 << 32;
  board.calculatePins(true);

  printPinTypes(board.whitePieces.pinType);
  
  // printMask(board.blackPieces.rooks & board.whitePieces.rooks & board.whitePieces.king);
  std::cout << std::string(board) << std::endl;

  return 0;
}
