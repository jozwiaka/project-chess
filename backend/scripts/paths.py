import os

pgn_dir = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "data"
)

model_path = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "models/chess_model.h5"
)

label_encoder_path = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "models/label_encoder.npy",
)


move_out_path = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "out/move.out"
)
