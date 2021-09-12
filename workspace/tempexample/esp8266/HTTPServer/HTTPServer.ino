#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define STASSID "cswork_2.4G"
#define STAPSK  "88888888"

const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void)
{
  //console baud rate
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  printf("\n");

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  printf("\n");
  printf("Connected to %s, IP:%s\n", ssid, WiFi.localIP().toString().c_str());

  //定義　HTTP Server　路由
  //首頁
  server.on("/", handleRoot);

  //inline 網頁
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  //當 HTTP Server 路由找不到時，進入 Not Found
  server.onNotFound(handleNotFound);
  //HTTP Server 啟動
  server.begin();
  printf("HTTP server started\n");
}

void loop(void) {
  //檢查是否有連線進入
  server.handleClient();
}