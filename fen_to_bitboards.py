#!/bin/python

import chess
import json

from sys import argv


def bitBoardToDict(bb: chess.BaseBoard, color: chess.Color):
    return {'pawn': int(bb.pieces(chess.PAWN, color)),
            'rook': int(bb.pieces(chess.ROOK, color)),
            'knight': int(bb.pieces(chess.KNIGHT, color)),
            'bishop': int(bb.pieces(chess.BISHOP, color)),
            'queen': int(bb.pieces(chess.QUEEN, color)),
            'king': int(bb.pieces(chess.KING, color))
            }


board = chess.Board(argv[1])
baseBoard = chess.BaseBoard(board_fen=argv[1].split()[0])

newTest = {
    argv[1]: {
        'white_pieces': bitBoardToDict(baseBoard, chess.WHITE),
        'black_pieces': bitBoardToDict(baseBoard, chess.BLACK),
        'white': True if board.turn == chess.WHITE else False}}

filename = 'tests.json'

with open(filename, 'r+') as f:
    try:
        d = json.load(f)
    except json.decoder.JSONDecodeError as e:
        d = {}

d.update(newTest)

with open(filename, 'w') as f:
    json.dump(d, f, indent=4)
