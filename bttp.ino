#define BLYNK_PRINT Serial
//#define BLYNK_TEMPLATE_ID "TMPLbdslqWKS"
//#define BLYNK_DEVICE_NAME "btvn"

#define BLYNK_TEMPLATE_ID           "TMPLyseJyUlV"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "47QZw0SD7wY8wbdAra0EDSyFX4LZY34t"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClient.h>
#include <string.h>
#include <BlynkSimpleEsp32.h>

// define blynk config
//#define BLYNK_AUTH_TOKEN "lBZ5u-S46umA39At9TXq0gDGHSS3wiBx"

#define wifiConnected 2
#define TEMP V0
#define HUMI V1

int value = 0;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SHY.2.4HZ";
char pass[] = "111111111";

//http config
String weatherApiKey = "ae587ba3b710e7a40c6f5a124c622f87";
float latitude = 16.047079;
float longtitude = 108.206230;
String weatherJsonBuffer;
String weatherUrl = "https://api.openweathermap.org/data/2.5/weather?lat=16.08333&lon=108.083328&appid=ae587ba3b710e7a40c6f5a124c622f87";

//function
String httpGETRequest(const char* Url);
void readValue();

//define global variable
int temp = 30;
int humi = 80;

BlynkTimer timer;

void setup()
{
    Serial.begin(115200);
    pinMode(wifiConnected, OUTPUT);
    delay(10);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    digitalWrite(wifiConnected, HIGH);
    Serial.println("WiFi connected");
    Blynk.begin(auth, ssid, pass);
    timer.setInterval(1000L, readValueFromApi);
}

void loop()
{
    Blynk.run();
    timer.run();
}

void readValueFromApi()
{
  weatherJsonBuffer = httpGETRequest(weatherUrl.c_str());
  JSONVar weatherJson = JSON.parse(weatherJsonBuffer);
  if(JSON.typeof_(weatherJson) == "undefined")
  {
    Serial.println("Parsing fail");
    return;
  }
  Serial.print("Temperature in Da nang: ");
  temp = weatherJson["main"]["temp"];
  temp = temp - 273;
  Serial.println(temp);
  Serial.print("Humidity in Da Nang: ");
  humi = weatherJson["main"]["humidity"];
  Serial.println(humi);
  Serial.println();
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, humi);
}

String httpGETRequest(const char* Url)
{
  HTTPClient http;
  http.begin(Url);
  String responseBody = "{}";
  int responseCode = http.GET();
  if(responseCode > 0)
  {
    responseBody = http.getString();
  }
  else
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}
