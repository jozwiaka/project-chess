import chess.pgn
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import os
from chess_data_processor import ChessDataProcessor


class CNNModel:
    def __init__(self, data):
        self.data = data

    def train(self):
        X, y = self.data

        print(f"X = {X[0]}")
        print(f"y = {y[0]}")
        label_encoder = LabelEncoder()

        # Convert the moves to a list of FEN strings representing the next position
        fen_list = [ChessDataProcessor.get_next_fen(move) for move in y]

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
