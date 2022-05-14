//LED針腳定義
#define LED1_PIN D1

int flash = 0;
void setup() {
    //console baud rate
    Serial.begin(115200);

    //LED 輸出模式
    pinMode(LED1_PIN,OUTPUT);

}

void loop() {

  if(flash){
    flash = 0;
    digitalWrite(LED1_PIN,LOW);
  }
  else{
    flash = 1;
    digitalWrite(LED1_PIN,HIGH);
  }
  
	delay(1000); //每秒閃爍
}
