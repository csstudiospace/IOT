/* 
    @desc: this is MQTT example, which publish/subscribe DUT's topics
    @step: 1. connect to WIFI
           2. establish mqtt connection, it's publisher and subscriber
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (50)

//ESP8266 名稱
#define DUTNAME "MyESP8266"
//數值，自動累加
int value = 0;

//WIFI網路的 SSID， 密碼
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

//Third-party MQTT Broker Domain Name ＆ Port
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

//MQTT topic & message
char send_topic[]="cs/dutTest/Info";
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
  //printf("WiFi connected, IP:%s\n", WiFi.localIP().c_str());
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
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    ++value;
    
    snprintf (msg, MSG_BUFFER_SIZE, "{\"name\":\"%s\",\"value\":%d}", DUTNAME,value);
    printf("Publish message: %s\n",msg);
    client.publish(send_topic, msg, true);
  }
}
