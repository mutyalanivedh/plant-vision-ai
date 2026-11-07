#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// ==================================================
// Wi-Fi settings
// ==================================================
const char* ssid = "OPPO A6 Pro 5G t6ne";
const char* password = "87654321";

// ==================================================
// AI Thinker ESP32-CAM pin configuration
// ==================================================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WebServer server(80);

// ==================================================
// Send a complete JPEG response
// ==================================================
void handleJpg() {
  Serial.println("JPEG request received");

  camera_fb_t* fb = esp_camera_fb_get();

  if (fb == nullptr) {
    Serial.println("ERROR: Camera capture failed");
    server.send(503, "text/plain", "Camera capture failed");
    return;
  }

  Serial.printf("Captured JPEG: %u bytes\n", (unsigned int)fb->len);

  WiFiClient client = server.client();
  client.setNoDelay(true);
  client.setTimeout(5);

  // Send HTTP headers directly, including the exact JPEG size.
  client.printf(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/jpeg\r\n"
    "Content-Length: %u\r\n"
    "Cache-Control: no-store\r\n"
    "Connection: close\r\n"
    "\r\n",
    (unsigned int)fb->len
  );

  size_t totalSent = 0;
  unsigned int zeroWrites = 0;
  const size_t chunkSize = 1024;

  while (totalSent < fb->len && client.connected()) {
    size_t remaining = fb->len - totalSent;
    size_t amount = remaining < chunkSize ? remaining : chunkSize;

    size_t written = client.write(fb->buf + totalSent, amount);

    if (written > 0) {
      totalSent += written;
      zeroWrites = 0;
    } else {
      zeroWrites++;
      if (zeroWrites >= 100) {
        Serial.println("ERROR: JPEG transfer stalled");
        break;
      }
      delay(10);
    }

    delay(1);
  }

  Serial.printf(
    "JPEG transfer: %u / %u bytes\n",
    (unsigned int)totalSent,
    (unsigned int)fb->len
  );

  if (totalSent == fb->len) {
    Serial.println("JPEG transfer complete");
  } else {
    Serial.println("WARNING: JPEG transfer incomplete");
  }

  // Release the camera buffer only after sending.
  esp_camera_fb_return(fb);

  delay(2);
  client.stop();
}

// ==================================================
// Camera webpage
// ==================================================
void handleRoot() {
  const char page[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Plant Disease Rover Camera</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 20px;
      background: #f3f5f3;
    }
    img {
      width: 100%;
      max-width: 640px;
      height: auto;
      background: #ddd;
    }
    a {
      display: inline-block;
      margin: 12px;
      padding: 12px;
      background: #176b3a;
      color: white;
      text-decoration: none;
      border-radius: 6px;
    }
    #status { color: #444; }
  </style>
</head>
<body>
  <h2>Plant Disease Rover Camera</h2>
  <p id="status">Connecting to camera...</p>
  <img id="camera" alt="Camera image">
  <br>
  <a href="/capture" target="_blank">Capture single image</a>
  <p>Automatic refresh pauses until each image finishes loading.</p>

  <script>
    const img = document.getElementById('camera');
    const statusText = document.getElementById('status');

    function loadImage() {
      img.onload = function() {
        statusText.textContent = 'Camera connected';
        setTimeout(loadImage, 1500);
      };

      img.onerror = function() {
        statusText.textContent = 'Capture failed; retrying...';
        setTimeout(loadImage, 3000);
      };

      img.src = '/jpg?t=' + Date.now();
    }

    loadImage();
  </script>
</body>
</html>
)HTML";

  server.send(200, "text/html", page);
}

// ==================================================
// Initialize camera
// ==================================================
bool initializeCamera() {
  camera_config_t config = {};

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 25;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_DRAM;

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed: 0x%x\n", err);
    return false;
  }

  Serial.println("Camera initialized successfully at VGA");
  return true;
}

// ==================================================
// Setup
// ==================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Plant Disease Rover - Camera Recovery Firmware");

  if (!initializeCamera()) {
    Serial.println("Check camera model and connections.");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 30000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection failed. Restart the board.");
    return;
  }

  Serial.println("Wi-Fi connected");
  Serial.print("Camera IP address: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/jpg", HTTP_GET, handleJpg);
  server.on("/capture", HTTP_GET, handleJpg);

  server.onNotFound([]() {
    server.send(404, "text/plain", "Endpoint not found");
  });

  server.begin();

  Serial.println("Web server started");
  Serial.println("Endpoints:");
  Serial.println("  /        - Camera webpage");
  Serial.println("  /jpg     - JPEG image");
  Serial.println("  /capture - Single JPEG capture");
}

// ==================================================
// Main loop
// ==================================================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected; reconnecting...");
    WiFi.reconnect();
    delay(100);
  }

  server.handleClient();
  delay(2);
}