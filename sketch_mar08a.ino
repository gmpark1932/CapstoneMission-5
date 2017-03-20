#include <OneWire.h> 
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ONE_WIRE_BUS D4   //  <------------------------

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

const char* host = "api.thingspeak.com";
String ApiKey = "3RZ0UFQDK1K47R9E";
String path = "/update?key=" + ApiKey + "&field1=";

/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/

const char* ssid = "olleh_WiFi_B0E3";
const char* pass = "0000004223";
 
void setup(void) 
{ 
 // start serial port 
 Serial.begin(115200); 
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 
 WiFi.begin(ssid,pass);
 //Wait for connection
 while(WiFi.status() != WL_CONNECTED){
  delay(100);
  Serial.print(".");
 }

  Serial.println("");
  Serial.print("Conneted to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 // Start up the library 
 sensors.begin();
 
} 

float getTemperature() {
  float temp;
  do {
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

void send_temperature(){
  char temperatureString[6];
  float temperature = getTemperature();

  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
  
}

void loop(void) 
{ 
  send_temperature();

  delay(500);
} 

