from classes.chess_data_processor import ChessDataProcessor
from classes.cnn_model import CNNModel
import paths
import os

if __name__ == "__main__":
    data = ChessDataProcessor.load_data(paths.data_dir)
    cnn_model = CNNModel(data)
    cnn_model.train()
    cnn_model.save_model(paths.chess_model_path, paths.label_encoder_path)
