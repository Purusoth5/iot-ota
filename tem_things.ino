#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTTYPE DHT11
DHT dht(D5, DHT11);

String apiKey = "5SPL3TKJQAWF42W7";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "purusoth123";     // replace with your wifi ssid and wpa2 key
const char *pass =  "purusoth128";
const char *server = "api.thingspeak.com";

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(1000);
   dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);
  pinMode(A0,INPUT);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void loop()
{
  int soil=analogRead(A0);
WiFiClient client;
 float h = dht.readHumidity();
  float t = dht.readTemperature();
      
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  const int httpPort = 80; //Port 80 is commonly used for www
 //---------------------------------------------------------------------
 //Connect to host, host(web site) is define at top
 if(!client.connect(server, httpPort)){
   Serial.println("Connection Failed");
   delay(300);
   return; //Keep retrying until we get connected
 }
if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
 
    String postStr = apiKey;
    postStr += "&field2=";
    postStr += String(t);
    postStr += "&field3=";
    postStr += String(h);
    postStr += "&field1=";
    postStr += String(soil);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
delay(1000);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");


//Wait for server to respond with timeout of 5 Seconds
 int timeout=0;
 while((!client.available()) && (timeout < 1000))     //Wait 5 seconds for data
 {
   delay(10);  //Use this with time out
   timeout++;
 }
//---------------------------------------------------------------------
 //If data is available before time out read it.
 if(timeout < 500)
 {
     while(client.available()){
        Serial.println(client.readString()); //Response from ThingSpeak
     }
 }
 else
 {
     Serial.println("Request timeout..");
 }
 delay(1000);  //Read Web Page every 20 seconds
  }
  
 

  Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(20000);
}
