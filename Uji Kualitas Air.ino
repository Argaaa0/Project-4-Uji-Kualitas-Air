#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define sensorPin A0 // Analog pin to which Turbidity Sensor is connected
#define INTERVAL 1000 // Interval in milliseconds untuk membaca sensor dan mengirimkan data

int read_ADC;
int ntu;
const char *ssid = "Galaxy"; // Ganti dengan SSID WiFi Anda
const char *password = "12345678"; // Ganti dengan password WiFi Anda
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTML());
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  read_ADC = analogRead(sensorPin);
  if (read_ADC > 208) read_ADC = 208;
  ntu = map(read_ADC, 0, 208, 300, 0);
  Serial.println(ntu);
  server.handleClient();
  delay(INTERVAL);
}

String getHTML() {
  String status;
  String color;
  
  // Determine turbidity status
  if (ntu <= 50) {
    status = "Jernih";
    color = "lightblue";
  } else if (ntu > 50 && ntu <= 100) {
    status = "Keruh";
    color = "yellow";
  } else {
    status = "Kotor";
    color = "red";
  }

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>Turbidity Sensor Data</title></head><body>";
  html += "<h1>Turbidity Sensor Data</h1>";
  html += "<p>NTU: " + String(ntu) + "</p>";
  html += "<div style='width: 100px; height: 20px; background-color: " + color + ";'></div>"; // Balok berubah warna
  html += "<p>Status: " + status + "</p>";
  html += "</body></html>";
  return html;
}
