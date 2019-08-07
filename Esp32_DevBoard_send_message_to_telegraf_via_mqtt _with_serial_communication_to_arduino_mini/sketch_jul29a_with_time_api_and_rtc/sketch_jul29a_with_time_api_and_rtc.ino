#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

// For RTC
#include <Wire.h>  // for I2C with RTC module
#include "RTClib.h"
RTC_DS3231 rtc;
int currentHour = 12;

// Update these with values suitable for your network.
// Monkey Park
//const char* ssid = "MonkeyPark";
//const char* password = "MonkeyPark";

// Dads House
//const char* ssid = "BrightBox-9sbpsc";
//const char* password = "tag-dread-tame";

// Oder 47


const int baudRate = 115200;

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

const char *mqtt_server = "postman.cloudmqtt.com";
#define mqtt_port 11968
#define MQTT_USER "xxtdtmwf"
#define MQTT_PASSWORD "c-0_VSx4qaOv"
#define MQTT_SERIAL_PUBLISH_TEST "test"
#define MQTT_SERIAL_PUBLISH_PLANTS "plants/berlin/oderstrasse/andrew"
#define MQTT_SERIAL_PUBLISH_CPU "things/esp32"

// Wifi
WiFiClient wifiClient;
const char *ssid = "(Don't mention the war)";
const char *password = "56939862460419967485";

// HTTP
HTTPClient http;
char* timeApiEndP = "http://worldtimeapi.org/api/ip";

//MQTT
PubSubClient client(wifiClient);

int loops = 0;
const int checkLoops = 800000;

void setup()
{
  setupSerialPorts();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
  setupRtc ();
}


void loop()
{
  client.loop();
  if (loops > checkLoops) {
    loops = 0;
    readCpuTemp();
    readSystemStats();
    timeLoop();
  }
  loops++;
  readSoftwareSerial2();
}


void setupSerialPorts() {
  Serial.begin(baudRate);
  Serial2.begin(baudRate);
  Serial2.println("Hello, world?");
  delay(100);
  Serial.setTimeout(1000);
}

void setupRtc () {

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date &amp; time, for example to set
    // January 21, 2014 at 3am you would call:
    //     rtc.adjust(DateTime(2019, 7, 21, 1, 30, 0));
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));
}


void setup_wifi()
{
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {

    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("connected");

      //Once connected, publish an announcement...
      client.publish(MQTT_SERIAL_PUBLISH_TEST, "ESP32 client");

    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}

void readSoftwareSerial2() {
  
  if (Serial2.available()) {
    noInterrupts();
    char output[255];
    Serial2.readBytesUntil('\r', output, 255);
    while(Serial2.read() >= 0)
    Serial.println(output);
    client.publish(MQTT_SERIAL_PUBLISH_PLANTS, output);
    delay(10);
    interrupts();
  }
}


void readCpuTemp() {
  
  double tempInC = (temprature_sens_read() - 32) / 1.8;

  char finalString[110];
  char influxString[100] = "things,thing-id=andrews-esp32-nodemcu,city=berlin,location=oderstrasse,room=andrews cpu_temp_c=";
  char tempString[10];
  snprintf(tempString, 10, "%f", tempInC);
  
  strcpy(finalString,influxString);
  strcat(finalString,tempString);
  Serial.println(finalString);
  client.publish(MQTT_SERIAL_PUBLISH_CPU, finalString);
  delay(10);
}

void readSystemStats(){
  char finalString[180];
  char influxString[100] = "things,thing-id=andrews-esp32-nodemcu,city=berlin,location=oderstrasse,room=andrews free_heap_size=";
  
  strcpy(finalString,influxString);

  char b[20];
  snprintf(b, 10, "%ld", xPortGetFreeHeapSize());
  strcat(finalString,b);

  strcat(finalString,",minimun_ever_free_heap_size=");
  
  char c[20];
  snprintf(c, 10, "%ld", xPortGetMinimumEverFreeHeapSize());

  strcat(finalString,c);
    
  Serial.println(finalString);
  client.publish(MQTT_SERIAL_PUBLISH_CPU, finalString);
  delay(10);
  }

void timeLoop () {
  int newHour = rtc.now().hour();

  if (newHour != currentHour) {
    if (newHour < 24) {
      currentHour = rtc.now().hour();
      calibrateTime();
      Serial2.println(newHour);
      if (newHour > 21 || newHour < 9) {
        Serial2.println("TURN_OFF");
      } else {
        Serial2.println("TURN_ON");
      }
    }
  }
}

void calibrateTime() {

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    http.begin(timeApiEndP); //Specify the URL
    int httpCode = http.GET();//Make the request

    if (httpCode > 0) { //Check for the returning code

      String payload = http.getString();
      if (httpCode == 200) {
        String newDateTime = extractDateTime(payload);
        Serial.println(newDateTime);

        int newYear = newDateTime.substring(0 , 4).toInt();
        int newMonth = newDateTime.substring(5 , 7).toInt();
        int newDay = newDateTime.substring(8 , 10).toInt();
        int newHour = newDateTime.substring(11 , 13).toInt();
        int newMinutes = newDateTime.substring(14 , 16).toInt();
        int newSeconds = newDateTime.substring(17 , 19).toInt();
        Serial.print(newYear);
        Serial.println(" newYear");
        Serial.print(newMonth);
        Serial.println(" newMonth");
        Serial.print(newDay);
        Serial.println(" newDay");
        Serial.print(newHour);
        Serial.println(" newHour");
        Serial.print(newMinutes);
        Serial.println(" newMinutes");
        Serial.print(newSeconds);
        Serial.println(" newSeconds");

        rtc.adjust(DateTime(newYear, newMonth, newDay, newHour, newMinutes, newSeconds));

      } else {
        // Send Error to Mailgun
        Serial.println(payload);
      }
    }

    else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }

  delay(10);
}

void getYear(String string) {

}

String extractDateTime(String string) {
  int startIndex = string.lastIndexOf("\"datetime\":\"");
  int endIndex = string.indexOf(",\"client_ip\"");
  return string.substring(startIndex + 12 , endIndex - 1);
}
