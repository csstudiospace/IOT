//Sensor-VCC -> DUT-3.3v~5v pin
#define VIBRATION_SENSOR_PIN D6

#define RED_LED D0
#define GREEN_LED D1

int vibration_detected = 0;

void setup() {
    //console baud rate
    Serial.begin(115200);

    pinMode(RED_LED, OUTPUT);   // 定義 紅燈 為送出模式
    pinMode(GREEN_LED, OUTPUT); // 定義 綠燈 為送出模式
}

void loop() {
	vibration_detected = digitalRead(VIBRATION_SENSOR_PIN);
	Serial.println(vibration_detected);

  if(vibration_detected == 1){
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
  }
  else{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
  
	delay(100); //每 100 ms 偵測震動
}
