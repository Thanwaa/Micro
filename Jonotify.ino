void Line_Notify(String message) ;
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#define b 08

 
const char* ssid = "Teach";
const char* password = "30122540";

void Line_Notify(String message) ;
#define LINE_TOKEN "R8cL4RerwvT9VhPn8K2hCN0XLhZfUcluXZAgtP3qT7M" 
SoftwareSerial NodeSerial(D2, D3);
String message = "%E0%B8%88%E0%B8%B1%E0%B8%81%E0%B8%A3%E0%B8%A2%E0%B8%B2%E0%B8%99%E0%B8%82%E0%B8%AD%E0%B8%87%E0%B8%84%E0%B8%B8%E0%B8%93%E0%B9%84%E0%B8%A1%E0%B9%88%E0%B8%9B%E0%B8%A5%E0%B8%AD%E0%B8%94%E0%B8%A0%E0%B8%B1%E0%B8%A2";
void setup () {
 
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(b, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}



void loop() {
  while (NodeSerial.available() > 0) {
float val = NodeSerial.parseFloat();
if (NodeSerial.read() == '\n') {
if (val == 10) {
Line_Notify(message);
} 
Serial.println(val);
}
}
delay(100);

  Serial.println();
  HTTPClient http;
  http.begin("https://projectteach.herokuapp.com/text.txt", "08:3B:71:72:02:43:6E:CA:ED:42:86:93:BA:7E:DF:81:C4:BC:62:30");
  int httpCode = http.GET();
  if (httpCode == 200) {
    String content = http.getString();
    Serial.println(content);

  int Value3012 = -1;
  int ValueError = -1;
  
  ValueError = content.indexOf("error");
  Value3012 = content.indexOf("3012");

  if (ValueError > 0) {
  delay(500);
  }
  else {
    Serial.print("");
  

        if (Value3012  == -1) 
    {
      delay(500);
      Serial.print("\n 3012\n");
    }
  
    
        if (Value3012 != -1) 
    {
        digitalWrite(b , 1);
        delay(500);
        Serial.print("\n 3012\n");
        
      }
      
     
      Line_Notify(message);
  }
  }else {
    Serial.println("Fail. error code " + String(httpCode));
  }
    delay(1000);
}


void Line_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(1000);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
