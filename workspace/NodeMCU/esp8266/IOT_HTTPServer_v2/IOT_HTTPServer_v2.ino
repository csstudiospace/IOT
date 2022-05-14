#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <DHT.h>

#define STASSID "cswork2.4"
#define STAPSK  "22224444"

#define LED1_PIN D1
#define LED2_PIN D2

#define TEMPERATURE_SENSOR_PIN D3
#define SONA_TRIG_SENSOR_PIN D4
#define SONA_ECHO_SENSOR_PIN D5
#define VIBRATION_SENSOR_PIN D6

const char* ssid = STASSID;
const char* password = STAPSK;

DHT dht(TEMPERATURE_SENSOR_PIN, DHT11);
ESP8266WebServer webserver(80);


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i = 0; i < webserver.args(); i++) {
    message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
  }
  webserver.send(404, "text/plain", message);
}

void handleHome() {
  String message = "<html>";
  message += "<head>";
  message += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";
  message += "</head>";
  message += "<body>";

  message += "<h1> Hello from ESP8266</h1>";
  //定義LED 網頁介面與行為
  message += "<h3>";
  message += "LED1:<button state='0' name='led1' type='button' onclick='btclick(this);'>ON</button>";
  message += "LED2:<button state='0' name='led2' type='button' onclick='btclick(this);'>ON</button>";
  message += "</h3>";
  message += "<script>";
  message += "function btclick(ele){";
  message += "  if(ele.getAttribute('state') == 0){";
  message += "    ele.innerText = 'OFF';";
  message += "    ele.setAttribute('state', 1);";
  message += "  }";
  message += "  else{";
  message += "    ele.innerText = 'ON';";
  message += "    ele.setAttribute('state', 0);";
  message += "  }";
  message += "var xmlHttp = new XMLHttpRequest();";
  message += "var ledurl = ele.getAttribute('name')+\"=\"+ele.getAttribute('state');";
  message += "xmlHttp.open('GET', 'api/setdata?'+ledurl, false );";
  message += "xmlHttp.send( null );";
  message += "}";
  message += "</script>";
  
  //定義SW420 網頁介面
  message += "<h2> 震動：<span id='vib'></span></h2>";
  
  //定義DHT11 網頁介面
  message += "<h2> 溫度：<span id='temp'></span></h2>";
  message += "<h2> 溼度：<span id='hum'></span></h2>";
  
  //定義HC-SR04 網頁介面
  message += "<h2> 距離：<span id='dis'></span></h2>";

  message += "</body>";

  //當網頁開啟時，取得所有值並填入震動、溫度、聲洚距離的值。
  message += "<script>";
  message += "document.addEventListener('DOMContentLoaded', function(){";
  message += "var xmlHttp = new XMLHttpRequest();";
  message += "xmlHttp.open(\"GET\", \"api/getdata\", false );";
  message += "xmlHttp.send( null );";
  message += "var obj = JSON.parse(xmlHttp.responseText);";
  message += "console.log(obj);";
  message += "document.getElementById(\"vib\").innerHTML = obj.vibration;";
  message += "document.getElementById(\"temp\").innerHTML = obj.temp;";
  message += "document.getElementById(\"hum\").innerHTML = obj.hum;";
  message += "document.getElementById(\"dis\").innerHTML = obj.sona;";
  message +="});";
  message += "</script>";
  message += "</html>";

  //回傳網頁內容
  webserver.send(200, "text/html", message);
}

void handleGetData() {
  char jsonmsg[100]={0};
  //收集SW420的震動值
  int vibration_val = digitalRead(VIBRATION_SENSOR_PIN);

  //收集DHT11的溫、溼度值
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

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
 
  strcpy(jsonmsg,"{");
  sprintf(jsonmsg+strlen(jsonmsg),"\"vibration\":\"%d\",", vibration_val);
  sprintf(jsonmsg+strlen(jsonmsg),"\"temp\":\"%d\",", (int)temperature);
  sprintf(jsonmsg+strlen(jsonmsg),"\"hum\":\"%d\",", (int)humidity);
  sprintf(jsonmsg+strlen(jsonmsg),"\"sona\":\"%.2f\"", distance);
  sprintf(jsonmsg+strlen(jsonmsg),"}");
  
  webserver.send(200, "application/json", jsonmsg);
}

void handleSetData() {
  int i;
  char bodydata[1024] = {0};
  
  //解析傳送過來的data
  sprintf((char *)bodydata, "%s", webserver.arg("plain").c_str());
  printf("method:%d, url:%s, args:%d\n", webserver.method(), webserver.uri().c_str(), webserver.args());
  for(i=0;i<webserver.args();i++){
    printf("i:%d, argName:%s, arg:%s\n", i, webserver.argName(i).c_str(), webserver.arg(i).c_str());
    
    //收到led的請求並設定
    if(strcmp("led1", webserver.argName(i).c_str())==0){
      if(strcmp("0", webserver.arg(i).c_str())==0){ //led 1 關
        printf("LED OFF\n");
        digitalWrite(LED1_PIN,LOW);
      }
      else if(strcmp("1", webserver.arg(i).c_str())==0){ //led 1 開
        printf("LED ON\n");
        digitalWrite(LED1_PIN,HIGH);
      }
      else{} //led1 do nothing
    }
    else if(strcmp("led2", webserver.argName(i).c_str())==0){
      if(strcmp("0", webserver.arg(i).c_str())==0){ //led 2 關
        printf("LED OFF\n");
        digitalWrite(LED2_PIN,LOW);
      }
      else if(strcmp("1", webserver.arg(i).c_str())==0){ //led 2 開
        printf("LED ON\n");
        digitalWrite(LED2_PIN,HIGH);
      }
      else{} //led2 do nothing
    }
    else{}//do nothing
  }
  printf("===========\n");

  //回傳設定led 成功
  char jsonmsg[100]={0};
  strcpy(jsonmsg,"{");
  sprintf(jsonmsg+strlen(jsonmsg),"\"ret\":\"ok\"");
  sprintf(jsonmsg+strlen(jsonmsg),"}");
  webserver.send(200, "application/json", jsonmsg);
}

void setup(void)
{
  //console baud rate
  Serial.begin(115200);

  //LED 輸出模式
  pinMode(LED1_PIN,OUTPUT);
  pinMode(LED2_PIN,OUTPUT);

  //聲納trig 輸出模式
  pinMode(SONA_TRIG_SENSOR_PIN,OUTPUT);

  //DHT11 初始化
  dht.begin();

  //WiFi連線
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  printf("\n");

  //Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  printf("\n");
  printf("Connected to %s, IP:%s\n", ssid, WiFi.localIP().toString().c_str());

  //定義　HTTP Server　路由
  //首頁
  webserver.on("/", handleHome);

  //api 取得所有值
  webserver.on("/api/getdata", handleGetData);

  //api 設定led
  webserver.on("/api/setdata", handleSetData);

  //當 HTTP Server 路由找不到時，進入 Not Found
  webserver.onNotFound(handleNotFound);
  //HTTP Server 啟動
  webserver.begin();
  printf("HTTP server started\n");
}

void loop(void) {
  //檢查是否有連線進入
  webserver.handleClient();
}
