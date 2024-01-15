import chess
import chess.pgn
import chess.svg
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models


# Function to convert FEN to board state
def fen_to_board(fen):
    board = chess.Board(fen)
    return board


# Function to convert board state to one-hot encoded array
def board_to_array(board):
    piece_dict = {
        "p": -1,
        "r": -2,
        "n": -3,
        "b": -4,
        "q": -5,
        "k": -6,
        "P": 1,
        "R": 2,
        "N": 3,
        "B": 4,
        "Q": 5,
        "K": 6,
        ".": 0,
    }

    board_array = np.zeros((8, 8, 13))  # from -6 to 6

    for i in range(8):
        for j in range(8):
            piece = board.piece_at(chess.square(j, 7 - i))
            # print(piece)
            if piece is not None:
                board_array[i, j, piece_dict[str(piece)]] = 1
            else:
                board_array[i, j, 0] = 0

    return board_array


# Function to convert move to one-hot encoded array
def move_to_array(move):
    # Convert the move to a unique index
    index = chess.Move.from_uci(str(move)).uci()
    return index


# Load PGN data and preprocess it
pgn_path = "../data/London2g6.pgn"
games = []

with open(pgn_path) as pgn_file:
    while True:
        game = chess.pgn.read_game(pgn_file)
        if game is None:
            break
        games.append(game)

# Generate training data
X_train = []
y_train = []

for game in games:
    board = game.board()
    for move in game.mainline_moves():
        board.push(move)
        X_train.append(board_to_array(board))
        y_train.append(move_to_array(move))

X_train = np.array(X_train)
y_train = np.array(y_train)

# Define the CNN model
model = models.Sequential(
    [
        layers.Conv2D(64, (3, 3), activation="relu", input_shape=(8, 8, 6)),
        layers.Conv2D(64, (3, 3), activation="relu"),
        layers.Flatten(),
        layers.Dense(128, activation="relu"),
        layers.Dense(64, activation="relu"),
        layers.Dense(64, activation="relu"),
        layers.Dense(64, activation="relu"),
        layers.Dense(len(y_train[0]), activation="softmax"),
    ]
)

# Compile the model
model.compile(optimizer="adam", loss="categorical_crossentropy", metrics=["accuracy"])

# Convert labels to categorical format
y_train_categorical = tf.keras.utils.to_categorical(y_train)

# Train the model
model.fit(X_train, y_train_categorical, epochs=10, validation_split=0.2)

# Save the model
model.save("chess_model.h5")
