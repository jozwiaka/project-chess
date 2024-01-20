import os
import zipfile
import shutil
import paths


def unzip_all_and_move_pgn(src_path, dest_path):
    for item in os.listdir(src_path):
        item_path = os.path.join(src_path, item)
        print(item_path)
        if item_path.endswith(".zip"):
            with zipfile.ZipFile(item_path, "r") as zip_ref:
                zip_ref.extractall(src_path)
            for root, dirs, files in os.walk(src_path):
                for file in files:
                    if file.endswith(".pgn"):
                        pgn_file_path = os.path.join(root, file)
                        shutil.move(pgn_file_path, os.path.join(dest_path, file))

        # shutil.rmtree(item_path)


if __name__ == "__main__":
    source_path = "c:/Users/DELL/Downloads"
    destination_path = paths.pgn_dir

    unzip_all_and_move_pgn(source_path, destination_path)
