#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>

// network credentials
const char* ssid = "Xiaomi_FF90";
const char* password = "rjrs5200";

//  the IP address of the computer receiving the stream
IPAddress server(192, 168, 1, 100);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // Reduce JPEG quality to reduce bandwidth
  config.jpeg_quality = 10;
  config.frame_size = FRAMESIZE_QVGA;
  config.fb_count = 2;

  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Wait for camera to settle
  delay(2000);
}

void loop() {
  // Capture image from camera
  camera_fb_t* fb = esp_camera_fb_get();

  // Send image to server
  if (client.connect(server, 8080)) {
    // Send HTTP headers
    client.println("POST /image.jpg HTTP/1.1");
    client.println("Host: 192.168.1.100:8080");
    client.println("Content-Type: image/jpeg");
    client.print("Content-Length: ");
    client.println(fb->len);
    client.println();

    // Send image data
    client.write(fb->buf, fb->len);
  }

  // Release image buffer
  esp_camera_fb_return(fb);

  // Wait for next frame
  delay(1000);
}
