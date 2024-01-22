import os
import zipfile
import shutil
import paths
from selenium import webdriver
from selenium.webdriver.common.by import By
import requests


def unzip_all_and_move_pgn(src_path, dest_path):
    os.makedirs(dest_path, exist_ok=True)
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


if __name__ == "__main__":
    download_dir = f"{paths.data_dir}/tmp"
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
    unzip_all_and_move_pgn(download_dir, paths.data_dir)
    shutil.rmtree(download_dir)
