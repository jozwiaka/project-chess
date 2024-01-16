import chess.pgn
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import os


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


pgn_dir = "../data"
chess_data_processor = ChessDataProcessor(pgn_dir)
X, y = chess_data_processor.load_data()

label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

X_train, X_val, y_train, y_val = train_test_split(
    X, y_encoded, test_size=0.2, random_state=42
)

model = models.Sequential(
    [
        layers.Conv2D(32, (3, 3), activation="relu", input_shape=(8, 8, 12)),
        layers.Flatten(),
        layers.Dense(64, activation="relu"),
        layers.Dense(len(label_encoder.classes_), activation="softmax"),
    ]
)

model.compile(
    optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"]
)
model.fit(X_train, y_train, epochs=10, batch_size=32, validation_data=(X_val, y_val))
