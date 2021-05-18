#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "precomputedMasks.h"
#include "types.h"
#include "testing.h"

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
  U64 attackPieces(U64 location, bool white){
  	Pieces opponents = white ? blackPieces : whitePieces;
  	U64 RookAttacks =  0;
  	U64 BishopAttacks = 0;
    U64 allPieces = getAllPieces(white) | getAllPieces(!white);
    U64 temp = 0;
          
    temp = (opponents.rooks | opponents.queens) & rookMasks[index(location)];

    

  	while (temp){
  		U64 piece = LSBIT(temp);


  		if (row(piece) == row(location)){

            U64 horizontalPiecesL = (seekL[index(location)]&seekR[index(piece)])^(piece|location);
            U64 horizontalPiecesR = (seekR[index(location)]&seekL[index(piece)])^(piece|location);                       
            U64 horizontalPieces = (horizontalPiecesL | horizontalPiecesR) & (allPieces^piece);
            
            if (!horizontalPieces)
                RookAttacks |= piece;                
        }
  		else
        {
            U64 verticalPiecesU = (seekU[index(location)]&seekD[index(piece)])^(piece|location);
            U64 verticalPiecesD = (seekD[index(location)]&seekU[index(piece)])^(piece|location);
            U64 verticalPieces = (verticalPiecesU | verticalPiecesD) & (allPieces^piece);
              
            if (!verticalPieces)
                RookAttacks |= piece;            
        }        
        temp ^= piece;

    }

    temp = (opponents.bishops | opponents.queens) & bishopMasks[index(location)];

    while (temp){
    	U64 piece = LSBIT(temp);

    	U64 URpieces = (seekUR[index(location)]&seekDL[index(piece)])^(piece|location);
    	U64 DRpieces = (seekDR[index(location)]&seekUL[index(piece)])^(piece|location);
    	U64 ULpieces = (seekUL[index(location)]&seekDR[index(piece)])^(piece|location);
    	U64 DLpieces = (seekDL[index(location)]&seekUR[index(piece)])^(piece|location);

    	U64 inbetween = (URpieces | DRpieces | ULpieces | DLpieces) & (allPieces^piece);

    	if (!inbetween){
    		BishopAttacks |= piece;
    	}

    	temp ^= piece; 
    }

    return RookAttacks | BishopAttacks; //| BishopAttacks | QueenAttacks
  }
};

int main(){
  Board board;
  board.whitePieces.rooks = (U64)(pow(2,30)) | (U64)(pow(2,29)) | (U64)(pow(2,39)) | (U64)(pow(2,47));
  board.whitePieces.bishops = (U64)(pow(2,22)) | (U64)(pow(2,13)) | (U64)(pow(2,38)) | (U64)(pow(2,45));
  board.whitePieces.queens = (U64)(pow(2,7));
  //board.attackPieces(pow(2,31), false);
  printMask(board.whitePieces.rooks | board.whitePieces.bishops | board.whitePieces.queens);
  printMask(pow(2,31));
  printMask(board.attackPieces(pow(2,31), false)| (U64)pow(2,31));
} 
