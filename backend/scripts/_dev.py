import os
import numpy as np
import chess
import chess.pgn
from tensorflow.keras import layers, models, callbacks
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import paths
import matplotlib.pyplot as plt


class ChessModel:
    def load_model(self, chess_model_path, label_encoder_path):
        self.model = models.load_model(chess_model_path)
        classes = np.load(label_encoder_path)
        self.label_encoder = LabelEncoder()
        self.label_encoder.classes_ = classes

    def predict_move(self, fen_data):
        input_matrix = ChessDataProcessor.fen_to_matrix(fen_data)
        input_matrix = np.reshape(input_matrix, (1, 8, 8, 12))
        prediction = self.model.predict(input_matrix)
        move = self.label_encoder.inverse_transform([np.argmax(prediction)])[0]
        return move


class CNNModel:
    def __init__(self, data):
        self.data = data

    def train(self):
        X, y = self.data

        label_encoder = LabelEncoder()
        y_encoded = label_encoder.fit_transform(y)

        X_train, X_val, y_train, y_val = train_test_split(
            X, y_encoded, test_size=0.2, random_state=42
        )

        model = models.Sequential(
            [
                layers.Conv2D(64, (3, 3), activation="relu", input_shape=(8, 8, 12)),
                layers.BatchNormalization(),
                layers.Conv2D(128, (3, 3), activation="relu"),
                layers.BatchNormalization(),
                layers.Conv2D(128, (3, 3), activation="relu"),
                layers.BatchNormalization(),
                layers.Flatten(),
                layers.Dense(256, activation="relu", kernel_regularizer="l2"),
                layers.Dropout(0.5),
                layers.Dense(len(label_encoder.classes_), activation="softmax"),
            ]
        )

        model.compile(
            optimizer="adam",
            loss="sparse_categorical_crossentropy",
            metrics=["accuracy"],
        )

        history = model.fit(
            X_train,
            y_train,
            epochs=10,
            batch_size=32,
            validation_data=(X_val, y_val),
        )

        # Plotting Training and Validation Loss
        plt.plot(history.history["loss"], label="Training Loss")
        plt.plot(history.history["val_loss"], label="Validation Loss")
        plt.legend()
        plt.savefig(f"{paths.plots_dir}/training_validation_loss.pdf")
        plt.close()

        # Plotting Training and Validation Accuracy
        plt.plot(history.history["accuracy"], label="Training Accuracy")
        plt.plot(history.history["val_accuracy"], label="Validation Accuracy")
        plt.legend()
        plt.savefig(f"{paths.plots_dir}/training_validation_accuracy.pdf")
        plt.close()

        self.model = model
        self.label_encoder = label_encoder

    def save_model(self, chess_model_path, label_encoder_path):
        self.model.save(chess_model_path)
        np.save(label_encoder_path, self.label_encoder.classes_)


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
                        game = chess.pgn.read_game(f)
                        if game is None:
                            break

                        board = game.board()
                        for move in game.mainline_moves():
                            fen = board.fen()
                            positions.append(ChessDataProcessor.fen_to_matrix(fen))
                            outcomes.append(move.uci())
                            board.push(move)
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
    data = ChessDataProcessor.load_data(paths.data_dir)
    cnn_model = CNNModel(data)
    cnn_model.train()
    cnn_model.save_model(paths.chess_model_path, paths.label_encoder_path)
    chess_model = ChessModel()
    chess_model.load_model(paths.chess_model_path, paths.label_encoder_path)
    play_chess_game(chess_model)
