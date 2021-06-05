#!/bin/python

import chess

from sys import argv


def printBitBoards(bb: chess.BaseBoard, color: chess.Color):
    print(f'{{0x{int(bb.pieces(chess.PAWN, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.ROOK, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.KNIGHT, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.BISHOP, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.QUEEN, color)):0{16}x},')
    print(f'0x{int(bb.pieces(chess.KING, color)):0{16}x}}}')


baseBoard = chess.BaseBoard(board_fen=argv[1])
print('WHITE BITBOARDS:')
printBitBoards(baseBoard, chess.WHITE)
print('BLACK BITBOARDS:')
printBitBoards(baseBoard, chess.BLACK)
