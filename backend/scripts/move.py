from classes.chess_model import ChessModel
import sys
import paths
import io
import chess
import numpy as np
from flask import Flask, request, jsonify

chess_model = ChessModel()
chess_model.load_model(paths.model_path, paths.label_encoder_path)
app = Flask(__name__)


@app.route("/generate_move", methods=["POST"])
def generate_move():
    fen_data = request.json.get("fen_data")
    move = chess_model.predict_move(fen_data)
    board = chess.Board(fen_data)
    legal_moves = [str(legal_move) for legal_move in board.legal_moves]
    if move not in legal_moves:
        move = np.random.choice(legal_moves)
    return jsonify({"positionFrom": move[0:2], "positionTo": move[2:4]})


if __name__ == "__main__":
    app.run(debug=False)
