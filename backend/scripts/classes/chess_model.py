from classes.chess_data_processor import ChessDataProcessor
from tensorflow.keras import models
import chess
import numpy as np
from sklearn.preprocessing import LabelEncoder


class ChessModel:
    def load_model(self, model_path, label_encoder_path):
        self.model = models.load_model(model_path)
        classes = np.load(label_encoder_path)
        self.label_encoder = LabelEncoder()
        self.label_encoder.classes_ = classes

    def predict_move(self, fen_data):
        input_matrix = ChessDataProcessor.fen_to_matrix(fen_data)
        input_matrix = np.reshape(input_matrix, (1, 8, 8, 12))
        prediction = self.model.predict(input_matrix)

        # Determine the current side (white or black)
        current_side = chess.Board(fen_data).turn

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

        move = decoded_label[0]

        board = chess.Board(fen_data)
        legal_moves = [str(legal_move) for legal_move in board.legal_moves]
        if move not in legal_moves:
            move = np.random.choice(legal_moves)

        return move
