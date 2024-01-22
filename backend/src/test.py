import chess
import paths
from classes.chess_move_generator import ChessMoveGenerator


def play_chess_game(model):
    with open(paths.test_out_path, "w") as f:
        for i in range(1):
            board = chess.Board()
            while not board.is_game_over():
                move = model.predict_move(board.fen())
                legal_moves = [str(legal_move) for legal_move in board.legal_moves]
                move_raw = move
                if move not in legal_moves:
                    move = np.random.choice(legal_moves)
                turn = ""
                if move_raw == move:
                    f.write(f"{board.fen()}; {turn}: {move}")
                else:
                    f.write(f"{board.fen()}; {turn}: {move_raw} -> {move}")
                f.write("\n")
                board.push(chess.Move.from_uci(move))
            result = board.result()
            if result == "1-0" or result == "0-1":
                f.write("Model wins!")
            else:
                f.write("The game is a draw.")


if __name__ == "__main__":
    chess_move_generator = ChessMoveGenerator()
    chess_move_generator.load_model(paths.chess_model_path, paths.label_encoder_path)
    play_chess_game(chess_move_generator)
