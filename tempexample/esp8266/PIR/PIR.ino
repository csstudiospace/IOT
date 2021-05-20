//Sensor-VCC -> DUT-Vin pin
int PIRSensor = D2;

void setup() {
  //console baud rate
  Serial.begin(115200);
  pinMode(PIRSensor, INPUT); //定義 D2 為輸入模式，讀取 PIR 訊號
}
void loop() {
  //每秒讀取 PIR，當HIGH時代表有人； 反之，代表無人
  long state = digitalRead(PIRSensor);
    if(state == HIGH) {
      printf("Person Detected!\n");
    }
    else {
      printf("No Person!\n");
    }

    delay(1000);
}
