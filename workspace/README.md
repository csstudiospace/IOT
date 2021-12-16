# This is a IOT project

- ### 不知安裝SDK的話，先參考 NodeMCU_HelloWorld PDF檔 NodeMCU_HelloWorld.pdf

- ### 打開 Arduino 的命令列
```
    (1) 打開 Arduino IDE
    (2) 在上方工具列，點擊工具
    (3) 在序列埠，選擇ESP8266的Com
    (4) 在上方工具列，點擊工具
    (5) 點擊序列埠覽控視窗
```

- ### 燒錄 ESP8266 DHT11
```
    (1) 打開 Arduino IDE
    (2) 載入各項功能的範例檔 .ino， 例： 載入DHT11 DHT11.ino
    (3) 修改WiFi SSID和密碼
    (4) 修改感測器的針腳位置
    (5) 點擊上傳
    (6) 觀看結果，從 Arduino 的命令列
```

- ### 燒錄 ESP8266 IOT_HTTPServer_2
```
    (1) 打開 Arduino IDE
    (2) 載入各項功能的範例檔 .ino， 例： 載入ESP8266 IOT Server V2版本 IOT_HTTPServer_v2.ino
    (3) 修改WiFi SSID和密碼
    (4) 修改感測器的針腳位置
    (5) 點擊上傳
    (6) 觀看結果，從ESP8266的網頁或中控台前台
```

- ### 燒錄 ESP8266 NodeMCU_MQTTv2
```
    (1) 打開 Arduino IDE
    (2) 載入各項功能的範例檔 .ino， 例： 載入ESP8266 IOT MQTT V2版本 NodeMCU_MQTTv2.ino
    (3) 修改WiFi SSID和密碼
    (4) 修改感測器的針腳位置
    (5) 點擊上傳
    (6) 觀看結果，從ESP8266的網頁或中控台前台
```

- ### 啟動 Http 中控台，與 IOT_HTTPServer_2 共同使用
```
    cd /workspace/node-v12.18.3-win-x86
    ./node-red -u .node-red .node-red/controlcenter.json

    (1) 進入中控台後台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880
    (2) 進入中控台前台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880/ui
```

- ### 啟動 MQTT 中控台，與 NodeMCU_MQTT 共同使用
MQTT url 固定為： cs/dutTest/Info<br>
過濾名稱為分類：MyESP8266_1, MyESP8266_2,...,MyESP8266_30
```
    cd /workspace/node-v12.18.3-win-x86
    ./node-red -u .node-red .node-red/mqtt_multiple.json

    (1) 進入中控台後台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880
    (2) 進入中控台前台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880/ui
```

- ### 啟動 MQTT 中控台，與 NodeMCU_MQTTv2 共同使用
過濾MQTT url為分類：cs/dutTest_0/GetInfo, cs/dutTest_1/GetInfo, cs/dutTest_2/GetInfo,...,cs/dutTest_N/GetInfo
```
    cd /workspace/node-v12.18.3-win-x86
    ./node-red -u .node-red .node-red/mqttv2.json

    (1) 進入中控台後台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880
    (2) 進入中控台前台 -> 打開瀏覽器，網址列輸入 http://127.0.0.1:1880/ui
```

