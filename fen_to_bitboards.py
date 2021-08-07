#!/bin/python

import chess
import random
import json

from sys import argv


def printBitBoards(bb: chess.BaseBoard, color: chess.Color):
    print(f'{{0x{int(bb.pieces(chess.PAWN, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.ROOK, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.KNIGHT, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.BISHOP, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.QUEEN, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.KING, color)):0{16}x}}}')

def bitBoardToDict(bb:chess.BaseBoard, color: chess.Color):
    return {"pawn": int(bb.pieces(chess.PAWN, color)),
            "rook": int(bb.pieces(chess.ROOK, color)),
            "knight": int(bb.pieces(chess.KNIGHT, color)),
            "bishop": int(bb.pieces(chess.BISHOP, color)),
            "queen": int(bb.pieces(chess.QUEEN, color)),
            "king":int(bb.pieces(chess.KING, color))
            }


res = argv[1].split()

baseBoard = chess.BaseBoard(board_fen=res[0])

print('WHITE BITBOARDS:')
printBitBoards(baseBoard, chess.WHITE)
print('BLACK BITBOARDS:')
printBitBoards(baseBoard, chess.BLACK)

white_pieces = bitBoardToDict(baseBoard, chess.WHITE)
black_pieces = bitBoardToDict(baseBoard, chess.BLACK)

all = {argv[1]: {"white_pieces" : white_pieces, "black_pieces": black_pieces, 'white': random.getrandbits(1)}}

with open("tests.json", "r+") as out:
    try:
        f = json.load(out)
        f.update(all)
        json.dump(f, out, indent=4)
    except json.decoder.JSONDecodeError as e:
        json.dump(all, out, indent=4)

