import paths
from classes.chess_data_processor import ChessDataProcessor


if __name__ == "__main__":
    ChessDataProcessor.download_data(paths.data_dir)
