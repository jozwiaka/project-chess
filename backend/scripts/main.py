from chess_data_processor import ChessDataProcessor
from cnn_model import CNNModel


if __name__ == "__main__":
    pgn_dir = "../data"
    data = ChessDataProcessor.load_data(pgn_dir)
    cnn_model = CNNModel(data)
    cnn_model.run()
