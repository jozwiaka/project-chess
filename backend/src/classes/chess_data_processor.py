import os
import numpy as np
import chess
import chess.pgn


class ChessDataProcessor:
    @staticmethod
    def load_data(data_dir):
        positions = []
        outcomes = []
        for filename in os.listdir(data_dir):
            if filename.endswith(".pgn"):
                pgn_file = os.path.join(data_dir, filename)
                with open(pgn_file) as f:
                    while True:
                        try:
                            game = chess.pgn.read_game(f)
                            if game is None:
                                break

                            board = game.board()
                            for move in game.mainline_moves():
                                fen = board.fen()
                                try:
                                    positions.append(
                                        ChessDataProcessor.fen_to_matrix(fen)
                                    )
                                    outcomes.append(move.uci())
                                    board.push(move)
                                except ValueError:
                                    print(f"Illegal move found in game: {filename}")
                                    positions.pop()
                                    outcomes.pop()
                                    break
                        except chess.pgn.Error:
                            print(f"Error reading PGN file: {filename}")
                            break
        return np.array(positions), np.array(outcomes)

    @staticmethod
    def fen_to_matrix(fen):
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
