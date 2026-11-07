from pathlib import Path
from ultralytics import YOLO


# Project root directory
BASE_DIR = Path(__file__).resolve().parent.parent

# Trained YOLO model
MODEL_PATH = BASE_DIR / "best.pt"


# Verify model file exists
if not MODEL_PATH.is_file():
    raise FileNotFoundError(
        f"Model file not found: {MODEL_PATH}"
    )


# Load the trained model
model = YOLO(str(MODEL_PATH))


def get_model():
    return model