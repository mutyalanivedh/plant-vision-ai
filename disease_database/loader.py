import json
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent

DISEASE_FILE = BASE_DIR / "disease_info.json"
DISCLAIMER_FILE = BASE_DIR / "disclaimer.json"


def load_disease_information():
    with open(DISEASE_FILE, "r", encoding="utf-8-sig") as file:
        return json.load(file)


def load_disclaimer():
    with open(DISCLAIMER_FILE, "r", encoding="utf-8-sig") as file:
        return json.load(file)["disclaimer"]
