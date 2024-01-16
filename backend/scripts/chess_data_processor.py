import chess
import chess.pgn
import os
import numpy as np


class ChessDataProcessor:
    def __init__(self, pgn_dir):
        self.pgn_dir = pgn_dir

    def load_data(self):
        positions = []
        outcomes = []
        for filename in os.listdir(self.pgn_dir):
            if filename.endswith(".pgn"):
                pgn_file = os.path.join(self.pgn_dir, filename)
                with open(pgn_file) as f:
                    while True:
                        game = chess.pgn.read_game(f)
                        if game is None:
                            break

                        board = game.board()
                        for move in game.mainline_moves():
                            board.push(move)
                            fen = board.fen()
                            positions.append(self._fen_to_matrix(fen))
                            outcomes.append(game.headers["Result"])

        return np.array(positions), np.array(outcomes)

    def _fen_to_matrix(self, fen):
        board = chess.Board(fen)
        matrix = np.zeros((8, 8, 12), dtype=np.uint8)
        piece_mapping = {
            "p": 0,
            "r": 1,
            "n": 2,
            "b": 3,
            "q": 4,
            "k": 5,
            "P": 6,
            "R": 7,
            "N": 8,
            "B": 9,
            "Q": 10,
            "K": 11,
        }

        for r in range(8):
            for c in range(8):
                square = chess.square(c, 7 - r)
                piece = board.piece_at(square)

                if piece is not None:
                    matrix[r, c, piece_mapping[piece.symbol()]] = 1
        return matrix
