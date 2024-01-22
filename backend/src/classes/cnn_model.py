import numpy as np
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import matplotlib.pyplot as plt
import paths


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

        # model = models.Sequential(
        #     [
        #         layers.Conv2D(64, (3, 3), activation="relu", input_shape=(8, 8, 12)),
        #         layers.Flatten(),
        #         layers.Dense(256, activation="relu", kernel_regularizer="l2"),
        #         layers.Dense(len(label_encoder.classes_), activation="softmax"),
        #     ]
        # )

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

        plt.plot(history.history["loss"], label="Training Loss")
        plt.plot(history.history["val_loss"], label="Validation Loss")
        plt.legend()
        plt.savefig(f"{paths.plots_dir}/training_validation_loss.pdf")
        plt.close()

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
