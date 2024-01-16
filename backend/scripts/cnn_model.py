import chess.pgn
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import os


class CNNModel:
    def __init__(self, data):
        self.data = data

    def run(self):
        X, y = self.data
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
            optimizer="adam",
            loss="sparse_categorical_crossentropy",
            metrics=["accuracy"],
        )
        model.fit(
            X_train, y_train, epochs=10, batch_size=32, validation_data=(X_val, y_val)
        )
