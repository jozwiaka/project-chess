from chess_data_processor import ChessDataProcessor
from cnn_model import CNNModel
import os
import numpy as np
import chess
import chess.pgn
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import os


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
        decoded_label = self.label_encoder.inverse_transform([np.argmax(prediction)])
        return decoded_label[0]
