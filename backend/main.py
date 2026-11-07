from io import BytesIO
from datetime import datetime
import sqlite3
from pathlib import Path

from fastapi import FastAPI, File, HTTPException, UploadFile
from PIL import Image

from backend.model_service import get_model
from prediction_history import save_prediction


# ========================================================
# PATHS
# ========================================================

BASE_DIR = Path(__file__).resolve().parent.parent

DATABASE_PATH = BASE_DIR / "prediction_history.db"


# ========================================================
# FASTAPI APPLICATION
# ========================================================

app = FastAPI(
    title="Plant Vision AI API",
    description="API for plant disease detection research",
    version="1.0.0"
)


# ========================================================
# LOAD YOLO MODEL
# ========================================================

model = get_model()


# ========================================================
# HOME
# ========================================================

@app.get("/")
def home():

    return {
        "message": "Plant Vision AI API is running"
    }


# ========================================================
# HEALTH CHECK
# ========================================================

@app.get("/health")
def health_check():

    return {
        "status": "healthy",
        "model_loaded": True,
        "number_of_classes": len(model.names)
    }


# ========================================================
# PREDICTION
# ========================================================

@app.post("/predict")
async def predict_plant(
    file: UploadFile = File(...)
):
    """
    Predict plant disease from an uploaded leaf image
    and save the prediction to SQLite history.
    """

    # ----------------------------------------------------
    # VALIDATE FILE TYPE
    # ----------------------------------------------------

    allowed_types = {
        "image/jpeg",
        "image/png",
        "image/jpg"
    }

    if file.content_type not in allowed_types:

        raise HTTPException(
            status_code=400,
            detail="Please upload a JPG or PNG image."
        )


    try:

        # ------------------------------------------------
        # READ IMAGE
        # ------------------------------------------------

        image_bytes = await file.read()


        # ------------------------------------------------
        # CONVERT TO PIL IMAGE
        # ------------------------------------------------

        image = Image.open(
            BytesIO(image_bytes)
        ).convert("RGB")


        # ------------------------------------------------
        # RUN YOLO PREDICTION
        # ------------------------------------------------

        results = model(
            image,
            verbose=False
        )


        result = results[0]

        probabilities = result.probs


        # ------------------------------------------------
        # GET TOP-1 PREDICTION
        # ------------------------------------------------

        top1_id = int(
            probabilities.top1
        )

        confidence = float(
            probabilities.top1conf
        )

        predicted_disease = (
            result.names[top1_id]
        )


        # ------------------------------------------------
        # SAVE PREDICTION TO SQLITE
        # ------------------------------------------------

        current_datetime = datetime.now()

        prediction_date = (
            current_datetime.strftime(
                "%Y-%m-%d"
            )
        )

        prediction_time = (
            current_datetime.strftime(
                "%H:%M:%S"
            )
        )


        image_reference = (
            file.filename
            if file.filename
            else "android_camera_image"
        )


        save_prediction(
            prediction_date,
            prediction_time,
            image_reference,
            predicted_disease,
            confidence
        )


        # ------------------------------------------------
        # RETURN RESULT
        # ------------------------------------------------

        return {

            "filename": file.filename,

            "predicted_disease":
                predicted_disease,

            "confidence":
                round(
                    confidence,
                    4
                )
        }


    except Exception as error:

        raise HTTPException(
            status_code=500,
            detail=f"Prediction failed: {str(error)}"
        )


# ========================================================
# PREDICTION HISTORY
# ========================================================

@app.get("/history")
def prediction_history():

    try:

        connection = sqlite3.connect(
            DATABASE_PATH
        )

        cursor = connection.cursor()


        cursor.execute(
            """
            SELECT
                id,
                prediction_date,
                prediction_time,
                image_reference,
                predicted_class,
                confidence
            FROM prediction_history
            ORDER BY id DESC
            """
        )


        records = cursor.fetchall()


        connection.close()


        history = []


        for record in records:

            history.append(
                {
                    "id": record[0],

                    "prediction_date":
                        record[1],

                    "prediction_time":
                        record[2],

                    "image_reference":
                        record[3],

                    "predicted_class":
                        record[4],

                    "confidence":
                        record[5]
                }
            )


        return {
            "total_records": len(history),
            "history": history
        }


    except Exception as error:

        raise HTTPException(
            status_code=500,
            detail=f"Could not load prediction history: {str(error)}"
        )