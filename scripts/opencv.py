import cv2
import numpy as np
import pyautogui
import os

# Load piece templates
pieces = {}
template_folder = "../resources"  # Update this path to your actual resources folder

for filename in os.listdir(template_folder):
    if filename.endswith(".png"):
        piece_name = os.path.splitext(filename)[0]
        piece_path = os.path.join(template_folder, filename)
        pieces[piece_name] = cv2.imread(piece_path, cv2.IMREAD_GRAYSCALE)


def recognize_piece(piece_image):
    best_match = None
    best_score = float("-inf")

    for name, template in pieces.items():
        # Convert template to grayscale (if not already)
        template_gray = (
            cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
            if template.ndim == 3
            else template
        )
        # Convert square image to grayscale
        piece_image_gray = cv2.cvtColor(piece_image, cv2.COLOR_BGR2GRAY)

        result = cv2.matchTemplate(
            piece_image_gray, template_gray, cv2.TM_CCOEFF_NORMED
        )
        min_val, max_val, _, _ = cv2.minMaxLoc(result)

        if max_val > best_score:
            best_score = max_val
            best_match = name

    return best_match


def take_screenshot(file_path="screenshot.png"):
    screenshot = pyautogui.screenshot()
    screenshot_np = np.array(screenshot)

    # Convert from BGR to RGB (OpenCV uses BGR)
    screenshot_rgb = cv2.cvtColor(screenshot_np, cv2.COLOR_RGB2BGR)

    # Save the screenshot
    cv2.imwrite(file_path, screenshot_rgb)


def process_image(file_path="screenshot.png"):
    img = cv2.imread(file_path)

    # Convert the image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Apply thresholding to highlight the chessboard
    _, thresh = cv2.threshold(gray, 200, 255, cv2.THRESH_BINARY)

    # Find contours in the thresholded image
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Iterate through the contours and find the largest one (assuming it's the chessboard)
    max_contour = max(contours, key=cv2.contourArea)

    # Get the bounding box of the contour
    x, y, w, h = cv2.boundingRect(max_contour)

    # Crop the chessboard region
    chessboard = img[y : y + h, x : x + w]

    # Display the cropped chessboard (optional)
    # cv2.imshow("Cropped Chessboard", chessboard)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    # Iterate through squares on the chessboard and recognize pieces
    square_size = 8  # Adjust this based on your chessboard size
    board_array = np.empty((8, 8), dtype=object)

    for i in range(8):
        for j in range(8):
            x_start, y_start = x + j * square_size, y + i * square_size
            x_end, y_end = x_start + square_size, y_start + square_size

            # Crop the square from the chessboard
            square_image = chessboard[y_start:y_end, x_start:x_end]

            # Recognize the piece on the square
            piece_name = recognize_piece(square_image)

            # Store the piece in the board array
            board_array[i, j] = piece_name

    # Print the resulting board array
    return board_array


if __name__ == "__main__":
    take_screenshot()
    print(process_image())
