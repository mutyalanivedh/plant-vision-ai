\# 🌱 Plant Vision AI



\### AI-Powered Plant Disease Detection System using Deep Learning, Computer Vision \& IoT



Plant Vision AI is an intelligent agriculture solution designed to identify plant diseases using a trained YOLO classification model. It combines Artificial Intelligence, a web application, backend APIs and hardware integration to support early disease detection and smarter farming.



\## 🎯 Project Objectives



\- Identify plant diseases from leaf images.

\- Provide disease predictions with confidence scores.

\- Offer disease-specific information and recommendations.

\- Integrate software with ESP32-based camera hardware.

\- Support accessible AI-powered agricultural applications.



\## ✨ Key Features



\- 🌿 AI-powered plant disease classification.

\- 📸 Leaf image upload and camera-based image input.

\- 🎯 Disease prediction with confidence scores.

\- 💊 Disease information and treatment recommendations.

\- 🌐 Interactive Streamlit web application.

\- ⚡ FastAPI backend for model inference.

\- 🧠 Trained YOLO classification model.

\- 📊 SQLite-based prediction history.

\- 🤖 ESP32 and camera hardware integration.



\## 🏗️ System Architecture



```text

&#x20;         User / Farmer

&#x20;               |

&#x20;               v

&#x20;      Streamlit Frontend

&#x20;               |

&#x20;               v

&#x20;         FastAPI Backend

&#x20;               |

&#x20;         +-----+------+

&#x20;         |            |

&#x20;         v            v

&#x20;      YOLO Model   SQLite

&#x20;         |

&#x20;         v

&#x20;   Disease Prediction



&#x20;      ESP32 Camera

&#x20;           |

&#x20;           v

&#x20;      Image Capture

&#x20;           |

&#x20;           v

&#x20;      Prediction API

```



\## 🛠️ Technology Stack



| Category | Technologies |

|---|---|

| Programming | Python |

| AI / Deep Learning | YOLO Classification |

| Frontend | Streamlit |

| Backend | FastAPI |

| Image Processing | OpenCV, Pillow |

| Database | SQLite |

| Hardware | ESP32, Camera |

| Version Control | Git, GitHub |



\## 🧠 AI Model



The project uses a trained YOLO classification model for plant disease identification.



\- Model format: PyTorch (`.pt`)

\- Model file: `best.pt`

\- Number of classes: 38 (reported during development)

\- Task: Image classification



\## 📂 Project Structure



```text

plant-vision-ai/

│

├── app.py

├── prediction\_history.py

├── history\_viewer.py

├── requirements.txt

├── best.pt

│

├── backend/

│   ├── main.py

│   └── model\_service.py

│

├── disease\_database/

│

├── hardware/

│   └── esp32/

│

├── ml/

├── tests/

├── docs/

│   ├── screenshots/

│   ├── videos/

│   ├── hardware/

│   └── architecture/

│

├── .gitignore

└── README.md

```



\## ⚙️ Installation and Setup



\### 1. Clone the repository



```bash

git clone https://github.com/mutyalanivedh/plant-vision-ai.git

cd plant-vision-ai

```



\### 2. Create a virtual environment



```bash

python -m venv venv

```



\### 3. Activate the environment



Windows:



```bash

venv\\Scripts\\activate

```



\### 4. Install dependencies



```bash

pip install -r requirements.txt

```



\### 5. Start the FastAPI backend



```bash

uvicorn backend.main:app --reload

```



\### 6. Start the Streamlit frontend



Open another terminal:



```bash

streamlit run app.py

```



Open the application at `http://localhost:8501`.



\## 📸 Application Demonstration



Screenshots and video demonstrations will be added here.



\## 🤖 Hardware Integration



The project incorporates ESP32-based camera hardware for image acquisition and smart agricultural applications.



Hardware source code, component specifications, photographs and circuit documentation will be maintained in the `hardware/` directory.



\## 🚀 Future Enhancements



\- Public cloud deployment.

\- Improved field-image prediction performance.

\- Mobile-friendly agricultural assistance.

\- Multilingual support.

\- Expanded hardware automation.

\- Additional plant disease coverage.



\## 🎯 Project Vision



To make agriculture smarter and more accessible by combining Artificial Intelligence, Computer Vision and IoT for early plant disease identification.



\*\*Smart Farming | Early Detection | Healthier Crops 🌱\*\*



\---



Developed as an AI and IoT-based agricultural technology project.

