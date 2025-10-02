/*
 Web https://randomnerdtutorials.com/
  Wuthichai Thitapakorn
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "index.h"


const char* ssid = "SSID_WIFI";
const char* password = "PASSWORD";
//const char* ssid = "Jeerapa_muay_2.4G";
//const char* password = "0896917667";

#define DHTPIN 5 //PIN ที่ เชื่อมต่อกับ sensor DHT GPIO5 D1
#define ledPin1 14 // GPIO14  D5  
#define ledPin2 12 // GPIO12  D6
#define relay1  0  // GPIO0   D3
#define relay2  4  // GPIO4   D4
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

//อุณหภูมิและความชื้น จะอับเดทใน loop()function
float t = 0.0;
float h = 0.0;

String st = "";       // ส่งข้อความเพื่อแสดงสถานะการทำงานของรีเลย์
float temp_set = 27.5;
String tp = "";

AsyncWebServer server(80);  

//"unsigned long" สำหรับตัวแปรที่มีเวลา
// ค่าจะใหญ่เกินไปอย่างรวดเร็วสำหรับ int ที่จะจัดเก็บ
unsigned long previousMillis = 0;    // จะจัดเก็บครั้งสุดท้ายที่อัปเดต DHT

// อัปเดตการอ่าน DHT ทุก 5 วินาที
const long interval = 5000;  

// Replaces placeholder with DHT values แทนที่ค่า placeholder ด้วยค่า DHT
// กระบวบการในการส่งข้อมูลเพื่อไปแสดงผลบนหน้าเว็บเพจ
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h); 
  } 
  else if(var == "STATUS"){     //แสดงสถานะการทำงานของรีเลย์ ON / OFF
    return st;
  }
  else if(var == "TEMPSET"){  //ค่าอุณหภูมิที่ตั้งไว้
    return String(temp_set);
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  
  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,HIGH);
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  
  Serial.begin(115200);
  dht.begin();
    
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
  
 /*------------------------------------------------------------------*/
  // Route for root / web page ส่งข้อมูลแบบ Asynchornouse
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  
  /*------------------------------------------------------------------*/
   server.on("/action_page", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send_P(200, "text/html",index_html, processor);
    //ส่งค่าแบบ GET โดยรับค่าจากตัวแปร value_setting ไปเก็บที่ tp กำหนดให้ค่า temp_set เท่ากับ tp
    tp = request->getParam("value_setting")->value(); 
    temp_set = tp.toFloat(); //แปลงค่า tp เป็น Float ใช้ .toFloat()
    Serial.println(tp);
  });

  /*-----------------------------ค่าอุณหภูมิ------------------------------*/
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });

  /*-----------------------------ค่าความชื้น-----------------------------*/
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });
  /*--------------------แสดงค่าสถานะการเปิด/ปิดของรีเลย์--------------------*/
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", st.c_str());
  });
  /*--------------------ส่งค่าอุณหภูมิไปแสดงผลที่หน้าเว็บเพจ------------------*/
    server.on("/temp_set", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send_P(200, "text/planin", String(temp_set).c_str());
  });
  /*------------------------------------------------------------------*/  
  // Start server
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {  
    previousMillis = currentMillis; // บันทึกครั้งสุดท้ายที่คุณอัปเดตค่า DHT   
    float newT = dht.readTemperature(); // อ่านอุณหภูมิเป็นเซลเซียส (ค่าเริ่มต้น)
    // อ่านอุณหภูมิเป็นฟาเรนไฮต์ (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // หากการอ่านอุณหภูมิล้มเหลวอย่าเปลี่ยนค่า t
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.println(t);
    }
/* start check the temperature to control Relay */

    if(t>temp_set){
      digitalWrite(ledPin1,HIGH);
      digitalWrite(ledPin2,LOW); 
      digitalWrite(relay2,LOW);
      //st = "On"; 
      st = "<button type='button' class='btn btn-success'>ON</button>";
    }
    
    else{
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,HIGH);
      digitalWrite(relay2,HIGH);
      //st = "Off";
      st = "<button type='button' class='btn btn-danger'>OFF</button>";
    }
  
/* end control relay */   
    // Read Humidity
    float newH = dht.readHumidity();      
    // หากการอ่านค่าความชื้นล้มเหลวอย่าเปลี่ยนค่า h
    if (isnan(newH)) { //Not a Number 
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.println(h);
    }
  }
}

