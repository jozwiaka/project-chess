import os
import numpy as np
import chess
import chess.pgn
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import time


class ChessDataProcessor:
    @staticmethod
    def load_data(pgn_dir):
        positions = []
        outcomes = []
        for filename in os.listdir(pgn_dir):
            if filename.endswith(".pgn"):
                pgn_file = os.path.join(pgn_dir, filename)
                with open(pgn_file) as f:
                    while True:
                        game = chess.pgn.read_game(f)
                        if game is None:
                            break

                        board = game.board()
                        for move in game.mainline_moves():
                            board.push(move)
                            fen = board.fen()
                            positions.append(ChessDataProcessor.fen_to_matrix(fen))
                            outcomes.append(move.uci())  # Store the move in UCI format

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


class CNNModel:
    def __init__(self, data):
        self.data = data

    def train(self):
        X, y = self.data

        label_encoder = LabelEncoder()

        fen_list = y
        y_encoded = label_encoder.fit_transform(fen_list)
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
            optimizer="adam",
            loss="sparse_categorical_crossentropy",
            metrics=["accuracy"],
        )
        model.fit(
            X_train, y_train, epochs=10, batch_size=32, validation_data=(X_val, y_val)
        )
        self.model = model
        self.label_encoder = label_encoder

    def save_model(self, model_path, label_encoder_path):
        self.model.save(model_path)
        np.save(label_encoder_path, self.label_encoder.classes_)


class ChessModel:
    def load_model(self, model_path, label_encoder_path):
        self.model = models.load_model(model_path)
        classes = np.load(label_encoder_path)
        self.label_encoder = LabelEncoder()
        self.label_encoder.classes_ = classes

    def predict_move(self, fen):
        input_matrix = ChessDataProcessor.fen_to_matrix(fen)
        input_matrix = np.reshape(input_matrix, (1, 8, 8, 12))
        prediction = self.model.predict(input_matrix)

        # Determine the current side (white or black)
        current_side = chess.Board(fen).turn

        # Filter predictions for the current side
        if current_side == chess.WHITE:
            relevant_predictions = prediction[
                :, : len(self.label_encoder.classes_) // 2
            ]
        else:
            relevant_predictions = prediction[
                :, len(self.label_encoder.classes_) // 2 :
            ]

        decoded_label = self.label_encoder.inverse_transform(
            [np.argmax(relevant_predictions)]
        )
        return decoded_label[0]


def remove_files_in_directory(directory_path):
    if os.path.exists(directory_path):
        [
            os.remove(os.path.join(directory_path, file))
            for file in os.listdir(directory_path)
            if os.path.isfile(os.path.join(directory_path, file))
        ]
        print(f"Removed all files in: {directory_path}")
    else:
        print(f"The directory '{directory_path}' does not exist.")


def play_chess_game(model1, model2):
    board = chess.Board()

    while not board.is_game_over():
        if board.turn == chess.WHITE:
            move = model1.predict_move(board.fen())
        else:
            move = model2.predict_move(board.fen())

        legal_moves = [str(legal_move) for legal_move in board.legal_moves]
        if move not in legal_moves:
            # Handle invalid move, e.g., choose a random legal move
            move = np.random.choice(legal_moves)

        board.push(chess.Move.from_uci(move))
        print(board)
        # time.sleep(1)  # Add a delay for visualization purposes

    result = board.result()
    if result == "1-0":
        print("Model 1 wins!")
    elif result == "0-1":
        print("Model 2 wins!")
    else:
        print("The game is a draw.")


if __name__ == "__main__":
    pgn_dir = "../_data"
    model_path1 = "../models/chess_model1.h5"
    label_encoder_path1 = "../models/label_encoder1.npy"
    model_path2 = "../models/chess_model2.h5"
    label_encoder_path2 = "../models/label_encoder2.npy"

    remove_files_in_directory("../models")

    # Train and save the first model
    data1 = ChessDataProcessor.load_data(pgn_dir)
    cnn_model1 = CNNModel(data1)
    cnn_model1.train()
    cnn_model1.save_model(model_path1, label_encoder_path1)

    # Train and save the second model
    data2 = ChessDataProcessor.load_data(pgn_dir)
    cnn_model2 = CNNModel(data2)
    cnn_model2.train()
    cnn_model2.save_model(model_path2, label_encoder_path2)

    # Load models for playing the game
    chess_model1 = ChessModel()
    chess_model1.load_model(model_path1, label_encoder_path1)

    chess_model2 = ChessModel()
    chess_model2.load_model(model_path2, label_encoder_path2)

    # Play the game
    play_chess_game(chess_model1, chess_model2)
