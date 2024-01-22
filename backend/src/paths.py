import os

src_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
plots_dir = f"{src_dir}/plots"
data_dir = f"{src_dir}/data"
models_dir = f"{src_dir}/models"
os.makedirs(plots_dir, exist_ok=True)
os.makedirs(data_dir, exist_ok=True)
os.makedirs(models_dir, exist_ok=True)

chess_model_path = f"{models_dir}/chess_model.h5"
label_encoder_path = f"{models_dir}/label_encoder.npy"
test_out_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "test.out")
