import chess
import paths
from classes.chess_model import ChessModel


def play_chess_game(model):
    with open(paths.test_out_path, "w") as f:
        for i in range(1):
            board = chess.Board()
            while not board.is_game_over():
                move = model.predict_move(board.fen())
                f.write(move)
                f.write("\n")
                board.push(chess.Move.from_uci(move))
            result = board.result()
            if result == "1-0":
                f.write("Model wins!")
            elif result == "0-1":
                f.write("Model wins!")
            else:
                f.write("The game is a draw.")


if __name__ == "__main__":
    chess_model = ChessModel()
    chess_model.load_model(paths.chess_model_path, paths.label_encoder_path)
    play_chess_game(chess_model)
