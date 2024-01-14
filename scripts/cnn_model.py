import sys
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

# import sklearn.impute
# import sklearn.compose
# import sklearn.preprocessing
# import sklearn.model_selection
# import tensorflow as tf

# tf.__version__


class CNNModel:
    def __init__(self):
        self.data = self._read_data()

    def _read_data(self):
        args = sys.argv
        if len(args) <= 1:
            raise Exception("No data has been received.")

        data = args[1]
        rows = data.split(";")
        parsed_data = [row.split(",") for row in rows]
        return parsed_data

    def predict_move(self):
        data = self._read_data()
        return "E4"


if __name__ == "__main__":
    cnn_model = CNNModel()
    print(cnn_model.predict_move())
