This is a IOT project

- 打開 Arduino 的命令列 -
    (1) 打開 Arduino IDE
	(2) 在上方工具列，點擊工具
	(3) 在序列埠，選擇ESP8266的Com
	(4) 在上方工具列，點擊工具
	(5) 點擊序列埠覽控視窗

- 燒錄 ESP8266 DHT11 -
    (1) 打開 Arduino IDE
	(2) 載入各項功能的範例檔 .ino， 例： 載入DHT11 DHT11.ino
	(3) 修改WiFi SSID和密碼
	(4) 修改感測器的針腳位置
	(5) 點擊上傳
	(6) 觀看結果，從 Arduino 的命令列

- 燒錄 ESP8266 IOT_HTTPServer_2 -
    (1) 打開 Arduino IDE
	(2) 載入各項功能的範例檔 .ino， 例： 載入ESP8266 IOT Server V2版本 IOT_HTTPServer_v2.ino
	(3) 修改WiFi SSID和密碼
	(4) 修改感測器的針腳位置
	(5) 點擊上傳
	(6) 觀看結果，從ESP8266的網頁或中控台前台

- 啟動中控台 -
	cd /workspace/node-v12.18.3-win-x86
	./node-red -u .node-red .node-red/controlcenter.json
	
	(1) 進入中控台後台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880
    (2) 進入中控台前台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880/ui

