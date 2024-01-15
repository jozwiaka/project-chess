import chess.pgn
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from tensorflow.keras.utils import to_categorical
from sklearn.model_selection import train_test_split


def load_pgn_data(file_path):
    data = []
    labels = []

    with open(file_path, "r") as file:
        while True:
            game = chess.pgn.read_game(file)
            if game is None:
                break

            board = game.board()
            moves = [str(move) for move in game.mainline_moves()]

            for i, move in enumerate(moves):
                # Convert the board to a feature representation
                board_representation = board_to_array(board)

                # Convert the move to a target label
                target_move = move_to_index(move)

                data.append(board_representation)
                labels.append(target_move)

                # Apply the move to the board for the next iteration
                board.push(chess.Move.from_uci(move))

    return np.array(data), np.array(labels)


def board_to_array(board):
    # Convert the chess board to a 2D array
    board_array = np.zeros((8, 8), dtype=np.int8)
    for square, piece in board.piece_map().items():
        row, col = chess.square_rank(square), chess.square_file(square)
        piece_value = piece.symbol().upper()
        piece_index = piece_to_index(piece_value)
        board_array[row][col] = piece_index
    return board_array


def piece_to_index(piece_symbol):
    # Convert chess piece symbol to an index
    pieces = {
        "P": 1,
        "N": 2,
        "B": 3,
        "R": 4,
        "Q": 5,
        "K": 6,
        "p": -1,
        "n": -2,
        "b": -3,
        "r": -4,
        "q": -5,
        "k": -6,
        ".": 0,
    }
    return pieces.get(piece_symbol, 0)


def move_to_index(move):
    # Convert chess move to an index
    return int(chess.Move.from_uci(move).uci(), 16)


# Load your PGN data
file_path = "../data/London2g6.pgn"
data, labels = load_pgn_data(file_path)

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(
    data, labels, test_size=0.2, random_state=42
)

# Convert data to a format suitable for CNN
X_train = np.array(X_train).reshape(-1, 8, 8, 1)  # Assuming 8x8 chess board
X_test = np.array(X_test).reshape(-1, 8, 8, 1)

# Convert labels to one-hot encoding
num_classes = 4096  # Assuming 4096 possible moves (64 squares * 64 squares)
y_train = to_categorical(y_train, num_classes=num_classes)
y_test = to_categorical(y_test, num_classes=num_classes)

# Build CNN model
model = models.Sequential()
model.add(layers.Conv2D(64, (3, 3), activation="relu", input_shape=(8, 8, 1)))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(128, (3, 3), activation="relu"))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Flatten())
model.add(layers.Dense(128, activation="relu"))
model.add(layers.Dense(num_classes, activation="softmax"))

# Compile the model
model.compile(optimizer="adam", loss="categorical_crossentropy", metrics=["accuracy"])

# Train the model
model.fit(X_train, y_train, epochs=10, validation_data=(X_test, y_test))

# Save the model to an H5 file
model.save("chess_model.h5")
