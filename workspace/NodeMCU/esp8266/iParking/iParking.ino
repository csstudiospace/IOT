/* 
    @desc: this is MQTT example, which publish/subscribe DUT's topics
    @step: 1. connect to WIFI
           2. establish mqtt connection, it's publisher and subscriber
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (1024)

//ESP8266 名稱
#define DUTNAME "iParking"

//LED
const int RED_LED   = D0;  //D0
const int GREEN_LED = D1;  //D1

//距離感測器(HC-SR04)
const int TRIGGER   = D4;  //D4
const int ECHO      = D5;  //D5

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

//Distance threshold
int VALID_DIS = 30;

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

  //LED 輸出模式
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);

  //聲納trig 輸出模式
  pinMode(TRIGGER,OUTPUT);
  
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

  //每1秒回報1次
  unsigned long now = millis();
  if (now - lastMsg > report_interval) {
    lastMsg = now;

    //取得IP資訊
    IPAddress cur_own_ip;
    cur_own_ip = WiFi.localIP();

    //收集HC-SR04的距離值
    long duration;
    float distance;
    digitalWrite(TRIGGER,LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER,LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = (float)34000/1000000 * duration /2;

    //是否亮紅燈
    int led = 0;
    //當距離低於30CM時，亮紅燈（代表有車停）。
    //反之亮綠燈(代表沒車停)。
    if(distance == 0){
        led = 2;
    }
    else if(distance < VALID_DIS){
        led = 1;
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
    }
    else{
        led = 0;
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
    }

    snprintf (msg, MSG_BUFFER_SIZE, "{\"rescmd\":\"18\",\"ret\":\"OK\",\"value\":{\"N\":\"%s\",\"dis\":\"%.2f\",\"led\":\"%d\",\"GPS\":0,\"IP\":\"%s\",\"Mac\":\"%s\",\"Lat\":0,\"Lng\":0,\"Key\":\"N/A\"}}", DUTNAME, distance, led, cur_own_ip.toString().c_str(), dut_mac_str);
    printf("Publish message: %s\n",msg);
    client.publish(send_topic, msg, true);
  }
}
