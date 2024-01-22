import os
import numpy as np
import chess
import chess.pgn
import zipfile
import shutil
from selenium import webdriver
from selenium.webdriver.common.by import By
import requests


class ChessDataProcessor:
    @staticmethod
    def download_data(data_dir):
        download_dir = f"{data_dir}/tmp"
        os.makedirs(download_dir, exist_ok=True)
        driver = webdriver.Chrome()
        website_url = "https://www.pgnmentor.com/files.html#players"
        driver.get(website_url)
        download_links = driver.find_elements(
            By.XPATH, '//a[text()="Download" and contains(@href, "player")]'
        )
        for link in download_links:
            href = link.get_attribute("href")
            response = requests.get(href)
            filename = os.path.join(download_dir, os.path.basename(href))
            with open(filename, "wb") as file:
                file.write(response.content)
        driver.quit()

        os.makedirs(data_dir, exist_ok=True)
        for item in os.listdir(download_dir):
            item_path = os.path.join(download_dir, item)
            print(item_path)
            if item_path.endswith(".zip"):
                with zipfile.ZipFile(item_path, "r") as zip_ref:
                    zip_ref.extractall(download_dir)
                for root, dirs, files in os.walk(download_dir):
                    for file in files:
                        if file.endswith(".pgn"):
                            pgn_file_path = os.path.join(root, file)
                            shutil.move(pgn_file_path, os.path.join(data_dir, file))

        shutil.rmtree(download_dir)

    @staticmethod
    def load_data(data_dir):
        positions = []
        outcomes = []
        for filename in os.listdir(data_dir):
            if filename.endswith(".pgn"):
                pgn_file = os.path.join(data_dir, filename)
                with open(pgn_file) as f:
                    while True:
                        try:
                            game = chess.pgn.read_game(f)
                            if game is None:
                                break

                            board = game.board()
                            for move in game.mainline_moves():
                                fen = board.fen()
                                try:
                                    positions.append(
                                        ChessDataProcessor.fen_to_matrix(fen)
                                    )
                                    outcomes.append(move.uci())
                                    board.push(move)
                                except ValueError:
                                    print(f"Illegal move found in game: {filename}")
                                    positions.pop()
                                    outcomes.pop()
                                    break
                        except chess.pgn.Error:
                            print(f"Error reading PGN file: {filename}")
                            break
        return np.array(positions), np.array(outcomes)

    @staticmethod
    def fen_to_matrix(fen):
        board = chess.Board(fen)
        matrix = np.zeros((8, 8, 12), dtype=np.uint8)
        piece_mapping = {
            "p": 0,
            "r": 1,
            "n": 2,
            "b": 3,
            "q": 4,
            "k": 5,
            "P": 6,
            "R": 7,
            "N": 8,
            "B": 9,
            "Q": 10,
            "K": 11,
        }

        for r in range(8):
            for c in range(8):
                square = chess.square(c, 7 - r)
                piece = board.piece_at(square)

                if piece is not None:
                    matrix[r, c, piece_mapping[piece.symbol()]] = 1
        return matrix
