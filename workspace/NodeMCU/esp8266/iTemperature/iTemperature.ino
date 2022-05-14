/* 
    @desc: this is MQTT example, which publish/subscribe DUT's topics
    @step: 1. connect to WIFI
           2. establish mqtt connection, it's publisher and subscriber
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>

#define MSG_BUFFER_SIZE (1024)

//ESP8266 名稱
#define DUTNAME "iTemperature"

//DHT11
int pinDHT11 = D3;  //D3 Pin腳 讀取DATA
SimpleDHT11 dht11;  //dht11 class

//回報間隔
const int report_interval = 2000;

//mac string
char dut_mac_str[17+1] = {0};

//WIFI網路的 SSID， 密碼
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_key";

//Third-party MQTT Broker Domain Name ＆ Port
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

//MQTT topic & message
char send_topic[]="cs/"DUTNAME"/output";
char msg[MSG_BUFFER_SIZE];


WiFiClient espClient;
PubSubClient client(espClient);

//count time
unsigned long lastMsg = 0;

void setup_wifi() {

  delay(10);

  //Start to connect to WIFI
  printf("Connecting to %s\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    printf(".");
  }

  printf("WiFi connected, IP:%s\n", WiFi.localIP().toString().c_str());
  //取得Mac資訊
  byte dut_mac[6];
  WiFi.macAddress(dut_mac);
  sprintf( dut_mac_str, "%02X:%02X:%02X:%02X:%02X:%02X", dut_mac[0], dut_mac[1], dut_mac[2], dut_mac[3], dut_mac[4], dut_mac[5]);
}

void mqttconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    printf("Attempting MQTT connection...\n");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    printf("client id:%s\n", clientId.c_str());
    if (client.connect(clientId.c_str())) {
      printf("MQTT connected\n");
    } else {
      printf("failed, rc=%d\n", client.state());

      //Wait 5 seconds before retrying
      printf(" try again in 5 seconds\n");
      delay(5000);
    }
  }
}

void setup() {
  //console baud rate
  Serial.begin(115200);

  //啟動WiFi連線
  setup_wifi();

  //設定 MQTT Broker ＆ Port
  client.setServer(mqtt_server, mqtt_port);
}


void loop() {

  if (!client.connected()) {
    //啟動 MQTT 連線
    mqttconnect();
  }
  client.loop();

  //每2秒回報1次
  unsigned long now = millis();
  if (now - lastMsg > report_interval) {
    lastMsg = now;

    //取得IP資訊
    IPAddress cur_own_ip;
    cur_own_ip = WiFi.localIP();

    //取得DHT11 溫𣳫度
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      //讀取失敗
      printf("Read DHT11 failed, err=%d\n", err);
      temperature = 0;
      humidity = 0;
    }
    
    printf("溼度 = %d %%\n", (int)humidity);
    printf("溫度 = %d C\n", (int)temperature);

  
    snprintf (msg, MSG_BUFFER_SIZE, "{\"rescmd\":\"18\",\"ret\":\"OK\",\"value\":{\"N\":\"%s\",\"hum\":\"%d\",\"temp\":\"%d\",\"GPS\":0,\"IP\":\"%s\",\"Mac\":\"%s\",\"Lat\":0,\"Lng\":0,\"Key\":\"N/A\"}}", DUTNAME, (int)humidity, (int)temperature, cur_own_ip.toString().c_str(), dut_mac_str);
    printf("Publish message: %s\n",msg);
    client.publish(send_topic, msg, true);
  }
}
