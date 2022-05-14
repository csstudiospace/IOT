//Sensor-VCC -> DUT-3V pin
#include <DHT.h>

int pinDHT11 = D3;  //D3 Pin腳 讀取DATA
DHT dht(pinDHT11, DHT11);  //dht11 class

void setup() {
  //console baud rate
  Serial.begin(115200);

  //DHT11 初始化
  dht.begin();
}

void loop() {

  //收集DHT11的溫、溼度值
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  printf("溼度 = %d %%\n", (int)humidity);
  printf("溫度 = %d C\n", (int)temperature);

  delay(2000);  //每2秒顯示一次
}
