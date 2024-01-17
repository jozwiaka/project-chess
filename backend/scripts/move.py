from classes.chess_model import ChessModel
import sys
import paths
import io

if __name__ == "__main__":
    chess_model = ChessModel()
    chess_model.load_model(paths.model_path, paths.label_encoder_path)
    move = chess_model.predict_move(sys.argv[1])
    with open(paths.move_out_path, "w") as f:
        f.write(move)
