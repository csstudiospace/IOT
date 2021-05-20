//Sensor-VCC -> DUT-Vin pin

const int TRIGGER = D4;  //D4
const int ECHO = D5;     //D5

// defines variables
long duration;
float distance;

void setup() {
	//console baud rate
	Serial.begin(115200);

	pinMode(TRIGGER, OUTPUT); // 定義 trigger 為送出模式
	pinMode(ECHO, INPUT);     // 定義 echo 為接受模式
}

void loop() {
	digitalWrite(TRIGGER, LOW);  
	delayMicroseconds(2);  
	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);                          //Trigger 需要維持10us的高電位才會發送聲波
	digitalWrite(TRIGGER, LOW);
	duration = pulseIn(ECHO, HIGH);                 //等待收到Trigger的訊號。
	printf("===========================\n");
	                                                //正確的公式包含溫度： Speed of sound m/s = 331.4 + (0.606 * Temp) + (0.0124 * Humidity)
	distance = (float)34000/1000000 * duration / 2; //計算CM的距離： 聲速(34000CM/1000000us) * 測量時間 = 聲音走的距離, 聲音走的距離/2 = 測量距離 
	printf("Duration:%ld us, Distance:%.1f CM\n", duration, distance);
	
	delay(1000);                                    //每秒偵測物體距離
}