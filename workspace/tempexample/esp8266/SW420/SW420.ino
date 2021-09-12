//Sensor-VCC -> DUT-3.3v~5v pin
#define VIBRATION_SENSOR_PIN D6
 
int vibration_detected = 0;
 
void setup() {
    //console baud rate
    Serial.begin(115200);
}

void loop() {
	vibration_detected = digitalRead(VIBRATION_SENSOR_PIN);
	Serial.println(vibration_detected);
	delay(100); //每 100 ms 偵測震動
}