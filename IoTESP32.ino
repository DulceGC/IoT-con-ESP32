//curso de IoT 2019

#include <WiFi.h>
#include <aREST.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//def
#define DHTPIN 23
//#define DHTTYPE DHT11
#define MQ 34

//init
DHT dht(DHTPIN, DHT11);

//aREST
aREST rest = aREST();

//WiFi
char ssid [] = "snapoffice_01"; //modem de wifi
char pass [] = "SnapAguascalientes"; //contraseña de tu modem

WiFiServer server(80);

float mq;
float humidity;
float temperature;

void setup(){
  Serial.begin(115200);
  dht.begin();

//init variables API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("pollution",&mq);
  
 //Name ID
  rest.set_id("1");
  rest.set_name("sensor_TTGO");

  //conect to wifi
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");

  //start server
  server.begin();
  Serial.println("Server started");

  //IP
  Serial.println("IP:");
  Serial.print(WiFi.localIP());
}
void loop(){
    //wait 1s
    delay(1000);
    mq = analogRead(MQ); //MQ135
    humidity = dht.readHumidity(); //RH %0 - 100 (Punto de rocio)
    temperature = dht.readTemperature(); //0 - 100 *c

    //REST calls
    WiFiClient client = server.available();
    if (!client) {
      return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}


 
