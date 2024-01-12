import cv2
import numpy as np
import pyautogui
import os


def take_screenshot(file_path="screenshot.png"):
    # Take a screenshot using pyautogui
    screenshot = pyautogui.screenshot()

    # Convert the screenshot to a NumPy array
    screenshot_np = np.array(screenshot)

    # Convert from BGR to RGB (OpenCV uses BGR)
    screenshot_rgb = cv2.cvtColor(screenshot_np, cv2.COLOR_RGB2BGR)

    # Save the screenshot
    cv2.imwrite(file_path, screenshot_rgb)


def process_image(file_path="screenshot.png"):
    # Read the image
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

    print(chessboard)


if __name__ == "__main__":
    take_screenshot()
    process_image()
