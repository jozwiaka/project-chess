import chess.pgn
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder


# Function to convert FEN (Forsyth-Edwards Notation) to a numerical matrix
def fen_to_matrix(fen):
    board = chess.Board(fen)
    matrix = np.zeros(
        (8, 8, 12), dtype=np.uint8
    )  # Assuming a 8x8 board and 12 channels for piece types
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


# Load PGN files and extract features (board positions) and labels
def load_data_from_pgn(pgn_file):
    positions = []
    outcomes = []

    with open(pgn_file) as f:
        while True:
            game = chess.pgn.read_game(f)
            if game is None:
                break

            board = game.board()
            for move in game.mainline_moves():
                board.push(move)
                fen = board.fen()
                positions.append(fen_to_matrix(fen))
                outcomes.append(game.headers["Result"])

    return np.array(positions), np.array(outcomes)


# Load your PGN data and preprocess it
pgn_file = "../data/London2g6.pgn"
X, y = load_data_from_pgn(pgn_file)

# Preprocess labels (convert outcomes to numerical format)
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

# Split the data into training and validation sets
X_train, X_val, y_train, y_val = train_test_split(
    X, y_encoded, test_size=0.2, random_state=42
)

# Define your CNN model
model = models.Sequential(
    [
        layers.Conv2D(32, (3, 3), activation="relu", input_shape=(8, 8, 12)),
        layers.Flatten(),
        layers.Dense(64, activation="relu"),
        layers.Dense(len(label_encoder.classes_), activation="softmax"),
    ]
)

# 32 = number of filters = determines how many different patterns or features the convolutional layer can learn. Each filter specializes in recognizing specific patterns
# e.g. in the context of image processing, imagine you are looking at a picture of a face. One filter might learn to detect the edges of the eyes, another contours of the nose, and another the general shape of the face.
# If the number of filter will be reduces (e.g. to 16), the model may have fewer resources to campture intricate details. Inreasing the number of filter (e.g. to 64) could allow the model to learn more diverse patterns, but it might also require more computation.
# ---------------------------------------------------------------
# (3, 3) = filter dize = determines the spatial extent of the local patterns the filter is looking for in the input data.
# e.g. the convolutional operation considers 3x3 window of pixels at a time. This allows the filter to capture small, local features in the input.
# A smaller filter size enables the model to focus on fine details, while a larger filter size might capture more global patterns. However, larger filter sizes can also inscrease the number of parameters in the model, which may lead to overfitting, especially if the dataset is limited.
# ---------------------------------------------------------------
# ReLU = Rectified Linear Unit = The activation function introduces non-linearity to the model = replaces all negative values in the output with zero, allowing the model to lean complex patterns and relationships
# e.g.  In the image processing, ReLU helps the model activate when it detects certain features. If a filter recognizes an edge, ReLU ensures that the response is postive, indicating the presence ofthe edge.
# Other types are Sigmoid or Tanh
# ---------------------------------------------------------------
# 8x8 chessboard with 12 channels

# Compile the model
model.compile(
    optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"]
)

# Train the model
model.fit(X_train, y_train, epochs=10, batch_size=32, validation_data=(X_val, y_val))
