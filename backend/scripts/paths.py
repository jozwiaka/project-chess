import os

web_driver = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "web_driver/chromedriver.exe",
)

plots_dir = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "plots"
)

data_dir = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "data"
)

models_dir = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "models"
)

chess_model_path = f"{models_dir}/chess_model.h5"

label_encoder_path = f"{models_dir}/label_encoder.npy"

test_out_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "test.out")
