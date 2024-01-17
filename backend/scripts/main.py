from chess_data_processor import ChessDataProcessor
from cnn_model import CNNModel
from chess_model import ChessModel
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

if __name__ == "__main__":
    pgn_dir = "../data"
    model_path = "../models/chess_model.h5"
    label_encoder_path = "../models/label_encoder.npy"

    # data = ChessDataProcessor.load_data(pgn_dir)
    # cnn_model = CNNModel(data)
    # cnn_model.train()
    # cnn_model.save_model(model_path, label_encoder_path)

    chess_model = ChessModel()
    chess_model.load_model(model_path, label_encoder_path)

    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    move_prediction = chess_model.predict_move(fen)

    print("Predicted Move:", move_prediction)
