/* 
    @desc: this is MQTT example, which publish/subscribe DUT's topics
    @step: 1. connect to WIFI
           2. establish mqtt connection, it's publisher and subscriber
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (1024)

//ESP8266 名稱
#define DUTNAME "iLED"

//LEDx2
#define LED1_PIN D0
#define LED2_PIN D1

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
char recv_topic[]="cs/"DUTNAME"/input";

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
  char *strsft;
  memset(str, 0, sizeof(str));
  memcpy(str, payload, length);

  int val = -1;
  strsft = strstr(str,"led1=");
  if(strsft != NULL){
    sscanf(strsft,"led1=%d", &val);
    digitalWrite(LED1_PIN,val);
    printf("Set LED1 to %s\n", (val==1)?"Enable":"Disable");
  }

  strsft = strstr(str,"led2=");
  if(strsft != NULL){
    sscanf(strsft,"led2=%d", &val);
    digitalWrite(LED2_PIN,val);
    printf("Set LED2 to %s\n", (val==1)?"Enable":"Disable");
  }
}

void setup() {
  //console baud rate
  Serial.begin(115200);

  //LED 輸出模式
  pinMode(LED1_PIN,OUTPUT);
  pinMode(LED2_PIN,OUTPUT);
  
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

    //取得IP資訊
    IPAddress cur_own_ip;
    cur_own_ip = WiFi.localIP();
  
    snprintf (msg, MSG_BUFFER_SIZE, "{\"rescmd\":\"18\",\"ret\":\"OK\",\"value\":{\"N\":\"%s\",\"GPS\":0,\"IP\":\"%s\",\"Mac\":\"%s\",\"Lat\":0,\"Lng\":0,\"Key\":\"N/A\"}}", DUTNAME, cur_own_ip.toString().c_str(), dut_mac_str);
    printf("Publish message: %s\n",msg);
    client.publish(send_topic, msg, true);
  }
}
