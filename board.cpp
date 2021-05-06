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

  U64 get_occupancy(U64 location, PieceType piece){
  	// function takes in piece location in bitboard format and returns all possible moves across board
  	U64 mask;
  	switch(piece){
  		case NONE:
  			break;
  		case Pawn:
  			break; 
  		case Rook:
  			mask = 0; 
  			//vertical above piece
  			for (int i = location+8; i<= 63-8; i+=8){
  				mask |= (((U64)1)<<i);
  			}
  			//vertical below piece
  			for (int i = location-8; i>= 8; i-=8){
  				mask |= (((U64)1)<<i);
  			}
  			//horizontal to the right
  			for (int i = location + 1; i%8<=7 && i%8!=0; i++){
  				mask |= (((U64)1)<<i)
  			}
  			//horizontal to the left
  			for (int i = location - 1; i%8<=7 && i&8!=0; i--){
  				mask |= (((U64)1)<<i)
  			}
  			break
  		case Knight:
  			break;
  		case Bishop:
  			break; 
  		case Queen:
  			break;
  		case KingL
  			break;
  	}
  	return mask;
  }


};
