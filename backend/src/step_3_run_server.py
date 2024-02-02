from classes.chess_move_generator import ChessMoveGenerator
import paths
from flask import Flask, request, jsonify

chess_move_generator = ChessMoveGenerator()
chess_move_generator.load_model(paths.chess_model_path, paths.label_encoder_path)
app = Flask(__name__)


@app.route("/generate_move", methods=["POST"])
def generate_move():
    fen_data = request.json.get("fen_data")
    move = chess_move_generator.generate_move(fen_data)
    return jsonify({"positionFrom": move[0:2], "positionTo": move[2:4]})


if __name__ == "__main__":
    app.run(debug=False, host="0.0.0.0")
