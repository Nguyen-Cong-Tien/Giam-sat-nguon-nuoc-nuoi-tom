#include "unistd.h"
#include "FirebaseESP8266.h"//Thêm thư viện esp8266 firebase
#include "FirebaseFS.h"
//#include "Utils.h"
#include "ESP8266WiFi.h"
#include "OneWire.h"//thêm thư viện onewire
#include "DallasTemperature.h"//thêm thư viện DallasTemperature

#define FIREBASE_HOST "test1-a63da-default-rtdb.firebaseio.com" // *Api Firebase
#define FIREBASE_AUTH "84Ghij9ox2yRfXeJEThDGIA0yOmkK63a7rbgHkEY" //*
const String tenwifi = "Nguyen Tien"; //*
const String mkwifi = "0123456789";
const int oneWireBus = D2;     
const int potPin=D1;
float ph;
float Value=0;
//thiết lập oneWire để giao tiếp với mọi thiết bị OneWire
OneWire oneWire(oneWireBus);

// chuyển dữ liệu từ Onewire tới cảm biến nhiệt độ Dallas 
DallasTemperature sensors(&oneWire);

FirebaseData firebaseData;
//float t = 30.56;
void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Ket noi den wifi ");
  Serial.println(tenwifi); 
  WiFi.begin(tenwifi,mkwifi);
  //WiFi.begin(tenwifi);//Trong trường hợp dùng wifi không mật khẩu thì dùng hàm này
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi da duoc ket noi");
  Serial.println("dia chi IP: "); 
  Serial.println(WiFi.localIP());//Xuất ip esp
  Serial.begin(115200);
  // chạy cảm biến DS18B20
  sensors.begin();
  //chạy cảm biến ph
  pinMode(potPin,INPUT);
  delay(1000);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
   sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  if (Firebase.setFloat(firebaseData, "/nhietdonuoc", temperatureC))
  {
    Serial.println("PASSED");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println();
  }
  Serial.print(temperatureF);
  Serial.println("ºF");

   Value= analogRead(potPin);//đọc dữ liệu từ cảm biến ph
    Serial.print(Value);//value là do cảm biến đọc được
    Serial.print(" | ");
    float voltage=Value*(3.3/4095.0);//chuyển đổi sang độ ph
    ph=(3.3*voltage);
    Serial.println(ph);
    if (Firebase.setFloat(firebaseData, "/doph", ph))
  {
    Serial.println("PASSED");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println();
  }
  delay(500);
  
}
