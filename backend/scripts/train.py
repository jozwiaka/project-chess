from classes.chess_data_processor import ChessDataProcessor
from classes.cnn_model import CNNModel
import paths
import os


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


if __name__ == "__main__":
    data = ChessDataProcessor.load_data(paths.pgn_dir)
    cnn_model = CNNModel(data)
    cnn_model.train()
    remove_files_in_directory(paths.models_dir)
    cnn_model.save_model(paths.chess_model_path, paths.label_encoder_path)
