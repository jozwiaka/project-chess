from classes.chess_model import ChessModel
import sys
import paths
import io
import chess
import numpy as np

if __name__ == "__main__":
    chess_model = ChessModel()
    chess_model.load_model(paths.model_path, paths.label_encoder_path)
    fen = sys.argv[1]
    move = chess_model.predict_move(fen)
    board = chess.Board(fen)
    legal_moves = [str(legal_move) for legal_move in board.legal_moves]
    if move not in legal_moves:
        move = np.random.choice(legal_moves)
    with open(paths.move_out_path, "w") as f:
        f.write(move)
