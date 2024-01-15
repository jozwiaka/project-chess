import sys
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import chess.pgn

# import sklearn.impute
# import sklearn.compose
# import sklearn.preprocessing
# import sklearn.model_selection
# import tensorflow as tf

# tf.__version__


class CNNModel:
    def __init__(self):
        self.data = self._prepare_data()

    def run(self):
        return self._predict_move()

    def _read_data(self):
        args = sys.argv
        if len(args) <= 1:
            raise Exception("No data has been received.")

        data = args[1]
        print(data)
        rows = data.split(";")
        parsed_data = [row.split(",") for row in rows]
        return parsed_data

    def _prepare_data(self):
        data = self._read_data()
        data = np.array(data).flatten()
        label_mapping = {
            ".": 0,
            "P": 1,
            "R": 2,
            "N": 3,
            "B": 4,
            "Q": 5,
            "K": 6,
            "p": 7,
            "r": 8,
            "n": 9,
            "b": 10,
            "q": 11,
            "k": 12,
        }
        data = np.vectorize(label_mapping.get)(data)
        data = data.reshape((8, 8, 1))
        for row in data:
            print(row)
        return data

    def _predict_move(self):
        return "E4"


if __name__ == "__main__":
    cnn_model = CNNModel()
    print(cnn_model.run())
