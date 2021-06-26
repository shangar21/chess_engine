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

  U64 getRookMoveMask(bool white, U64 location){
      Pieces player = white ? whitePieces : blackPieces;
      int pin = player.pinType[index(location)];
      U64 all = getAllPieces(true)|getAllPieces(false);

        if (pin != 0){
            if (pin == 1){
                if (col(player.king) > col(location)){
                  // king is on the right
                    U64 opp =  (U64)1 << (64 - U64_clz(seekL[index(location)] & getAllPieces(!white)));
                    return (seekL[index(location)] & seekR[index(opp)]) | opp | (seekR[index(location)] & seekL[index(player.king)]); 
                }
                else{
                  //king is on the left
                    U64 opp = LSBIT(seekR[index(location)] & getAllPieces(!white));
                    return (seekR[index(location)] & seekL[index(opp)]) | opp | (seekL[index(location)] & seekR[index(player.king)]); 
                }
            }
            else if (pin == 3){
                if (row(player.king) > row(location)){
                    // king is below location
                    U64 opp = (U64)1 << (64 - U64_clz(seekU[index(location)] & getAllPieces(!white)));
                    return (seekU[index(location)] & seekD[index(opp)]) | opp | (seekD[index(location)] & seekU[index(player.king)]); 
                }
                else{
                    // king is above location
                    U64 opp = LSBIT(seekD[index(location)] & getAllPieces(!white));
                    return (seekD[index(location)] & seekU[index(opp)]) | opp | (seekU[index(location)] & seekD[index(player.king)]); 
                }

            }
            return 0;
        }

         U64 mask = 0;
         U64 right_block =  LSBIT(seekR[index(location)] & all);
         U64 up_block = (U64)1 << 64 - U64_clz(seekU[index(location)] & all);
         U64 left_block = (U64)1 << 64 - U64_clz(seekL[index(location)] & all);
         U64 down_block =  LSBIT(seekD[index(location)] & all);
         
         mask |= right_block & getAllPieces(!white) ? seekR[index(location)] ^ seekR[index(right_block)] : (seekR[index(location)] ^ seekR[index(right_block)]) ^ right_block;
         mask |= up_block & getAllPieces(!white) ? seekU[index(location)] ^ seekU[index(up_block)] : (seekU[index(location)] ^ seekU[index(up_block)]) ^ up_block; 
         mask |= left_block & getAllPieces(!white) ? seekL[index(location)] ^ seekL[index(left_block)] : (seekL[index(location)] ^ seekL[index(left_block)]) ^ left_block;  
         mask |= down_block & getAllPieces(!white) ? seekD[index(location)] ^ seekD[index(down_block)] : (seekD[index(location)] ^ seekD[index(down_block)]) ^ down_block; 
         
         return mask;
  }

  U64 getKnightMoveMask(bool white, U64 location){
      Pieces player = white ? whitePieces : blackPieces;
      int pin = player.pinType[index(location)];
      U64 all = getAllPieces(true)|getAllPieces(false);

      return pin != 0 ? 0 : knightMasks[index(location)] ^ getAllPieces(white);
  }

  U64 getBishopMoveMask(bool white, U64 location){
    Pieces player = white ? whitePieces : blackPieces;
    int pin = player.pinType[index(location)];
    U64 all = getAllPieces(true)|getAllPieces(false);

    if (pin != 0){
          if (pin == 2){
            if (col(player.king) > col(location)){
              //king is up-right from location
              U64 opp = LSBIT(seekDL[index(location)] & getAllPieces(!white));
              return (seekDL[index(location)] & seekUR[index(opp)]) | opp | (seekUR[index(location)] & seekDL[index(player.king)]);
            }else{
              //king is down-left from location
              U64 opp = (U64)1 << (64 - U64_clz((seekUR[index(location)]) & getAllPieces(!white)));
              return (seekUR[index(location)] & seekDL[index(opp)]) | opp | (seekDL[index(location)] & seekUR[index(player.king)]);
            }
          }
          else if (pin == 4){
            if (col(player.king) < col(location)){
              // king is up-left from location
              U64 opp = LSBIT(seekDR[index(location)] & getAllPieces(!white));
              return (seekDR[index(location)] & seekUL[index(opp)]) | opp | (seekUL[index(location)] & seekDR[index(player.king)]);
            }
            else{
              // king is down-right from location
              U64 opp = (U64)1 << (64 - U64_clz(seekUL[index(location)] & getAllPieces(!white)));
              return (seekUL[index(location)] & seekDR[index(opp)]) | opp | (seekDR[index(location)] & seekUL[index(player.king)]);
            }
          }
          return 0;           
    }

    U64 mask = 0;

    U64 URblock = (U64)1 << (64 - (seekUR[index(location)] & all));
    U64 ULblock = (U64)1 << (64 - (seekUL[index(location)] & all));
    U64 DLblock = LSBIT(seekDL[index(location)] & all);
    U64 DRblock = LSBIT(seekDR[index(location)] & all);
    
    mask |= URblock & getAllPieces(!white) ? seekUR[index(location)] ^ seekUR[index(URblock)] : seekUR[index(location)] & seekDL[index(URblock)];
    mask |= ULblock & getAllPieces(!white) ? seekUL[index(location)] ^ seekUL[index(ULblock)] : seekUL[index(location)] & seekDR[index(ULblock)];
    mask |= DLblock & getAllPieces(!white) ? seekDL[index(location)] ^ seekDL[index(DLblock)] : seekDL[index(location)] & seekUR[index(DLblock)];
    mask |= DRblock & getAllPieces(!white) ? seekDR[index(location)] ^ seekDR[index(DRblock)] : seekDR[index(location)] & seekUL[index(DRblock)];

    return mask;

  }

  U64 getQueenMoveMask(bool white, U64 location){
    return getRookMoveMask(white, location) | getBishopMoveMask(white, location);
  }

  U64 getKingMoveMask(bool white, U64 location){
    U64 mask = 0;
     mask |= isPieceAttacked(location << 8, white) ? 0 : location << 8;
     mask |= isPieceAttacked(location >> 8, white) ? 0 : location >> 8;
     mask |= isPieceAttacked(location << 7, white) ? 0 : location << 7;
     mask |= isPieceAttacked(location >> 7, white) ? 0 : location >> 7;
     mask |= isPieceAttacked(location << 9, white) ? 0 : location << 9;
     mask |= isPieceAttacked(location >> 9, white) ? 0 : location >> 9;
     mask |= isPieceAttacked(location << 1, white) ? 0 : location << 1;
     mask |= isPieceAttacked(location >> 1, white) ? 0 : location >> 1;
    return mask ^ getAllPieces(white);
  }

  bool isMultCheck(bool white){
    Pieces player = white ? whitePieces : blackPieces;
    Pieces opponent = white ? blackPieces : whitePieces;

    if(!isPieceAttacked(player.king, white)){
      return false;
    }

    U64 attackingPieces = queenMasks[index(player.king)] & getAllPieces(!white);

    U64 blockingPieces_up = MSBIT(seekU[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_down = LSBIT(seekD[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_left = MSBIT(seekL[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_right = LSBIT(seekR[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_up_right = MSBIT(seekUR[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_up_left = MSBIT(seekUL[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_down_right = LSBIT(seekDR[index(player.king)] & getAllPieces(white));
    U64 blockingPieces_down_left = LSBIT(seekDL[index(player.king)] & getAllPieces(white));

    attackingPieces ^= blockingPieces_up ? seekU[index(blockingPieces_up)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_down ? seekU[index(blockingPieces_down)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_left ? seekU[index(blockingPieces_left)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_right ? seekU[index(blockingPieces_right)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_up_right ? seekU[index(blockingPieces_up_right)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_up_left ? seekU[index(blockingPieces_up_left)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_down_right ? seekU[index(blockingPieces_down_right)] & getAllPieces(white) : 0;
    attackingPieces ^= blockingPieces_down_left ? seekU[index(blockingPieces_down_left)] & getAllPieces(white) : 0;

    return isSingleBit(attackingPieces);

  }

  void moveSliding(bool white, U64 to, U64 from)
  {
    Pieces player = white ? whitePieces : blackPieces;
    Pieces opponent = white ? blackPieces : whitePieces;
    U64 mask = 0;

    if (from & player.knights){
      mask = getKnightMoveMask(white, from);
      if (mask & to){
        player.knights ^= from;
        player.knights |= to; 
      }
    }
    else if (from & player.rooks){
      mask = getRookMoveMask(white, from);
      if (mask & to){
        player.rooks ^= from;
        player.rooks |= to;
      }
    }
    else if (from & player.bishops){
      mask = getBishopMoveMask(white, from);
      if (mask & to){
        player.bishops ^= from;
        player.bishops |= to;
      }
    }
    else if (from & player.queens){
      mask = getQueenMoveMask(white, from);
      if (mask & to){
        player.queens ^= from;
        player.queens |= to;
      }
    }
    else if (from & player.king){
      mask = getKingMoveMask(white, from);
      if (mask & to){
        player.king ^= from;
        player.king |= to;
      }
    }
  }

  void move(bool white, U64 to, U64 from){
    Pieces player = white ? whitePieces : blackPieces;
    Pieces opponent = white ? blackPieces : whitePieces;
    U64 mask = 0;
    U64 currentPlay = getAllPieces(white);

    if (isPieceAttacked(player.king, white)){
      if (getKingMoveMask(white, from)){
        if(isMultCheck(white)){
          mask = getKingMoveMask(white, from);
          if((mask & to )&& (from & player.king)){
            player.king ^= from;
            player.king |= to;
          }else{
            std::cout << "invalid move ya mook";
          }
        }else{
          moveSliding(white, to, from);
          if (isPieceAttacked(player.king, white)){
            moveSliding(white, from, to);
            std::cout << "invalid move ya mook";
          }
        }        
      }else{
        std::cout << "checkmate";
      }
    }else{
      moveSliding(white, to, from);
      if(getAllPieces(white) == currentPlay){
        std::cout << "invalid move ya mook";
      }
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
