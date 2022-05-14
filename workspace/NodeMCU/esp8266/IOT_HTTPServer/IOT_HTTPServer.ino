#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define STASSID "cswork2.4"
#define STAPSK  "22224444"

const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);

//Sensor-VCC -> DUT-3V pin
#include <DHT.h>

int pinDHT11 = D3;  //D3 Pin腳 讀取DATA
DHT dht(pinDHT11, DHT11);  //dht11 class


void handleRoot() {
  char sendStr[1024];

  memset(sendStr, 0, sizeof(sendStr));

  //收集DHT11的溫、溼度值
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  sprintf( sendStr, "目前溫度 %d C, 溼度 %d %%", (int)temperature, (int)humidity);
  server.send(200, "text/plain", sendStr);
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

  //DHT11 初始化
  dht.begin();
  
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
