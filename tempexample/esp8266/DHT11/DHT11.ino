//Sensor-VCC -> DUT-3V pin
#include <SimpleDHT.h>

int pinDHT11 = D4;  //D4 Pin腳 讀取DATA
SimpleDHT11 dht11;  //dht11 class

void setup() {
  //console baud rate
  Serial.begin(115200);
}

void loop() {

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //讀取失敗
    printf("Read DHT11 failed, err=%d\n", err);
    delay(1000);
    return;
  }
  
  printf("溼度 = %d %%\n", (int)humidity);
  printf("溫度 = %d C\n", (int)temperature);

  delay(2000);  //每2秒顯示一次
}