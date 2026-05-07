#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

// ===== Cấu hình WiFi =====
const char* ssid = "Doremon";
const char* password = "15011996";

IPAddress broadcastIP(192, 168, 1, 255);
IPAddress local_IP(192, 168, 1, 97);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// ===== Cấu hình bảo mật web =====
const char* webUser = "admin";
const char* webPass = "7fAEUzV6oCgg57!";

// ===== Cấu hình OTA =====
const char* otaHostname = "admin";
const char* otaPassword = "7fAEUzV6oCgg57!-ota";

// ===== Danh sách máy cần bật =====
struct TargetPC {
  const char* name;
  byte mac[6];
};

TargetPC targets[] = {
  {"hung-desktop", {0x3C, 0x7C, 0x3F, 0x80, 0x69, 0x5B}}
};
const int targetCount = sizeof(targets) / sizeof(TargetPC);
const int port = 80;
WiFiUDP udp;
ESP8266WebServer server(port);

// Biến quản lý thời gian cho việc kiểm tra kết nối
unsigned long lastWifiCheck = 0;
const unsigned long wifiCheckInterval = 30000; // 30 giây kiểm tra 1 lần

void sendWOL(byte *mac) {
  const int wolPort = 9;
  byte packet[102];
  for (int i = 0; i < 6; i++) packet[i] = 0xFF;
  for (int i = 1; i <= 16; i++) memcpy(&packet[i * 6], mac, 6);
  
  udp.beginPacket(broadcastIP, wolPort);
  udp.write(packet, sizeof(packet));
  udp.endPacket();
  Serial.println(F("WOL packet sent."));
}

void handleRoot() {
  if (!server.authenticate(webUser, webPass)) {
    return server.requestAuthentication();
  }

  // Sử dụng F() để đẩy chuỗi tĩnh vào Flash, tiết kiệm RAM tránh treo
  String html = F("<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>WOL</title>");
  html += F("<style>body{font-family:sans-serif;text-align:center;background:#f0f2f5;padding:20px;}button{margin:10px;padding:15px 25px;font-size:1.5rem;border:none;border-radius:8px;background:#0078D7;color:white;cursor:pointer;width:100%;max-width:400px;}</style></head><body>");
  html += F("<h2>Wake-on-LAN Dashboard</h2>");

  for (int i = 0; i < targetCount; i++) {
    html += F("<form action='/wake' method='get'>");
    html += F("<input type='hidden' name='id' value='");
    html += String(i);
    html += F("'><button type='submit'>Bật máy: ");
    html += String(targets[i].name);
    html += F("</button></form>");
  }

  html += F("<p style='font-size:0.9rem;color:#666;'>Uptime: ");
  html += String(millis() / 60000);
  html += F(" mins | IP: ");
  html += WiFi.localIP().toString();
  html += F("</p><p>Giờ hệ thống: <span id='time'></span></p>");

  html += F("<script>function u(){const n=new Date();const l=new Date(n.getTime()+(7*3600000));document.getElementById('time').textContent=l.toISOString().replace('T',' ').split('.')[0]}setInterval(u,1000);u();</script>");
  html += F("</body></html>");

  server.send(200, "text/html", html);
}

void handleWake() {
  if (!server.authenticate(webUser, webPass)) return server.requestAuthentication();

  String idStr = server.arg("id");
  int id = idStr.toInt();
  if (idStr != "" && id >= 0 && id < targetCount) {
    sendWOL(targets[id].mac);
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(404, "text/plain", "Not Found");
  }
}

void handleRestart() {
  if (!server.authenticate(webUser, webPass)) return server.requestAuthentication();
  server.send(200, "text/plain", "Restarting ESP...");
  delay(1000);
  ESP.restart();
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi connection lost. Reconnecting..."));
    WiFi.begin(ssid, password);
    
    // Thử đợi tối đa 10 giây để kết nối lại
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 20) {
      delay(500);
      count++;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Tắt chế độ tiết kiệm điện WiFi (Rất quan trọng cho Web Server)
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("\nConnected!"));

  udp.begin(9);

  server.on("/", handleRoot);
  server.on("/wake", handleWake);
  server.on("/restart", handleRestart);
  server.begin();

  // Cấu hình OTA
  ArduinoOTA.setHostname(otaHostname);
  ArduinoOTA.setPassword(otaPassword);
  ArduinoOTA.begin();

  // Kích hoạt Watchdog Timer (WDT) phần cứng
  ESP.wdtEnable(WDTO_8S); 
  Serial.println(F("System Ready."));
}

void loop() {
  // Reset timer của WDT để báo cho chip biết code vẫn đang chạy bình thường
  ESP.wdtFeed(); 
  
  ArduinoOTA.handle();
  server.handleClient();

  // Kiểm tra WiFi định kỳ
  unsigned long currentMillis = millis();
  if (currentMillis - lastWifiCheck >= wifiCheckInterval) {
    lastWifiCheck = currentMillis;
    checkWiFi();
  }

  // Tự khởi động lại sau mỗi 1 ngày (86400 s) để giải phóng RAM triệt để
  if (currentMillis > 86400000) {
    ESP.restart();
  }
}