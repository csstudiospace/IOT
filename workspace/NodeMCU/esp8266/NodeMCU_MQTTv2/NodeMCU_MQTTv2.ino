/* 
    @desc: this is MQTT example, which publish/subscribe DUT's topics
    @step: 1. connect to WIFI
           2. establish mqtt connection, it's publisher and subscriber
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (100)

//ESP8266 名稱
#define DUTNAME "MyESP8266"
//數值，自動累加
int value = 0;

//LEDx2
#define LED1_PIN D1
#define LED2_PIN D2

//距離感測器(HC-SR04)
#define SONA_TRIG_SENSOR_PIN D4
#define SONA_ECHO_SENSOR_PIN D5
//震動感測器(SW420)
#define VIBRATION_SENSOR_PIN D6

//WIFI網路的 SSID， 密碼
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

//Third-party MQTT Broker Domain Name ＆ Port
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

//MQTT topic & message
char send_topic[]="cs/dutTest_29/GetInfo";
char recv_topic[]="cs/dutTest_29/SetInfo";
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
      client.subscribe(recv_topic);
    } else {
      printf("failed, rc=%d\n", client.state());

      //Wait 5 seconds before retrying
      printf(" try again in 5 seconds\n");
      delay(5000);
    }
  }
}

void recvcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  char str[64] = {0};
  memset(str, 0, sizeof(str));
  memcpy(str, payload, length);

  int val = -1;
  if(strstr(str,"led1=") != NULL){
    sscanf(str,"led1=%d", &val);
    digitalWrite(LED1_PIN,val);
    printf("Set LED1 to %s\n", (val==1)?"Enable":"Disable");
  }
  else if(strstr(str,"led2=") != NULL){
    sscanf(str,"led2=%d", &val);
    digitalWrite(LED2_PIN,val);
    printf("Set LED2 to %s\n", (val==1)?"Enable":"Disable");
  }
  else{
    printf("unknown str:%s\n", str);
  }
  
}

void setup() {
  //console baud rate
  Serial.begin(115200);

    //LED 輸出模式
  pinMode(LED1_PIN,OUTPUT);
  pinMode(LED2_PIN,OUTPUT);

  //聲納trig 輸出模式
  pinMode(SONA_TRIG_SENSOR_PIN,OUTPUT);


  //啟動WiFi連線
  setup_wifi();

  //設定 MQTT Broker ＆ Port
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(recvcallback);
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

    //Test value
    ++value;

    //震動偵測
    int vibration_val = digitalRead(VIBRATION_SENSOR_PIN);

    //收集HC-SR04的距離值
    long duration;
    float distance;
    digitalWrite(SONA_TRIG_SENSOR_PIN,LOW);
    delayMicroseconds(2);
    digitalWrite(SONA_TRIG_SENSOR_PIN,HIGH);
    delayMicroseconds(10);
    digitalWrite(SONA_TRIG_SENSOR_PIN,LOW);
    duration = pulseIn(SONA_ECHO_SENSOR_PIN, HIGH);
    distance = (float)34000/1000000 * duration /2;
    
    snprintf (msg, MSG_BUFFER_SIZE, "{\"name\":\"%s\",\"value\":%d,\"vib\":\"%d\",\"dis\":\"%.2f\"}", DUTNAME, value, vibration_val, distance);
    printf("Publish message \"%s\" to [%s]\n",msg, send_topic);
    client.publish(send_topic, msg, true);
  }
}
