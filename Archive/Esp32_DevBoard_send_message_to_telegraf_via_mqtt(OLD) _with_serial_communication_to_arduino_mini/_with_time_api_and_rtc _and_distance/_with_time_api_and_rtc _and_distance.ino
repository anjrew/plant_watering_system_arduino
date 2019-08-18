#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

// For Temparature
#include "DHTesp.h"
//#define DHTTYPE DHT22
uint8_t DHTPin = 4;
DHTesp dht;
float Temperature;
float Humidity;

// For RTC
#include <Wire.h> // for I2C with RTC module
#include "RTClib.h"
RTC_DS3231 rtc;
int currentHour = 12;
uint8_t LED_ONE = 2;
uint8_t LED_TWO = 16;

// FOR DISTANCE
#define TRIG_PIN = 26;
#define ECHO_PIN = 27;
 

// Update these with values suitable for your network.
// Monkey Park
//const char* ssid = "MonkeyPark";
//const char* password = "MonkeyPark";

// Dads House
//const char* ssid = "BrightBox-9sbpsc";
//const char* password = "tag-dread-tame";

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
#define MQTT_SERIAL_PUBLISH_PLACE "places/berlin/oderstrasse/andrew"

// Wifi
WiFiClient wifiClient;
const char *ssid = "(Don't mention the war)";
const char *password = "56939862460419967485";

// HTTP
HTTPClient http;
char *timeApiEndP = "http://worldtimeapi.org/api/ip";

//MQTT
PubSubClient client(wifiClient);

int loops = 0;
const int checkLoops = 2000000;
//const int checkLoops = 80000;
const byte numChars = 255;
char receivedChars[numChars];
boolean newData = false;

void setup()
{
    setupSerialPorts();
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    reconnect();
    setupRtc();
    setupEnviromentreading();
    setupDistancePins();
}

void loop()
{   
    if (loops % 1000){
        distanceLoop();
    }

    client.loop();
    if (loops > checkLoops)
    {
        loops = 0;
        readCpuTemp();
        readSystemStats();
        readEnviroment();
        // recordOutsideWeather();
        timeLoop();
        ledFlash();
    }

    loops++;
    readSoftwareSerial2();
}

void distanceLoop(){
   long duration, distanceCm, distanceIn;
 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;
 
  if (distanceCm <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distanceIn);
    Serial.print("in, ");
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
  }
  delay(1000);
}

void setupDistancePins() {
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}

void setupLeds(){
    pinMode(LED_TWO, OUTPUT);     // Initialize the LED_BUILTIN pin as an outputß
    pinMode(LED_ONE, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void setupEnviromentreading()
{
    dht.setup(DHTPin, DHTesp::DHT22);
}

void setupSerialPorts()
{
    Serial.begin(baudRate);
    Serial2.begin(baudRate);
    Serial2.setRxBufferSize(1024);
    delay(100);
    Serial2.println("Hello, world?");
    delay(100);
    Serial.setTimeout(1000);
}

void setupRtc()
{

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }
    if (rtc.lostPower())
    {
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

void ledFlash(){
    digitalWrite(LED_ONE, LOW);
    delay(500);
    digitalWrite(LED_ONE, HIGH);
    digitalWrite(LED_TWO, LOW);
    delay(500);
    digitalWrite(LED_TWO, HIGH);
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {

        Serial.print("Attempting MQTT connection...");

        // Create a random client ID
        String clientId;
        clientId.reserve(30);
        clientId = "ESP32Client-01";

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

void readSoftwareSerial2()
{
    if (Serial2.available())
    {
        noInterrupts();
        recvWithStartEndMarkers();
        showNewData();
        interrupts();
    }
}

void recvWithStartEndMarkers()
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial2.available() > 0 && newData == false)
    {
        rc = Serial2.read();

        if (recvInProgress == true)
        {
            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker)
        {
            recvInProgress = true;
        }
    }
}

void showNewData()
{
    if (newData == true)
    {
        Serial.println(receivedChars);
        client.publish(MQTT_SERIAL_PUBLISH_PLANTS, receivedChars);
        newData = false;
    }
}

void recordOutsideWeather()
{
    // void recordOutsideWeather(char[] lat, char[] longti){
    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

        http.begin("api.openweathermap.org/data/2.5/weather?lat=35&lon=139"); //Specify the URL
        int httpCode = http.GET();                                            //Make the request

        if (httpCode > 0)
        { //Check for the returning code

            String payload;
            payload.reserve(1000);
            payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
        }

        else
        {
            Serial.println("Error on HTTP request");
        }

        http.end(); //Free the resources
    }
};

void readEnviroment()
{
    TempAndHumidity lastValues = dht.getTempAndHumidity();
    float temperature = lastValues.temperature;
    float humidity = lastValues.humidity;
    char finalString[140];
    char influxString[120] = "places,thing-id=andrews-esp32-nodemcu,city=berlin,location=oderstrasse,room=andrews,sensor=dht22 temp_c=";
    char tempString[10];
    snprintf(tempString, 10, "%f", temperature);
    char humidString[10];
    snprintf(humidString, 10, "%f", humidity);
    strcpy(finalString, influxString);
    strcat(finalString, tempString);
    strcat(finalString, ",humidity=");
    strcat(finalString, humidString);
    Serial.println(finalString);
    client.publish(MQTT_SERIAL_PUBLISH_PLACE, finalString);
    delay(10);
}

void readCpuTemp()
{

    double tempInC = (temprature_sens_read() - 32) / 1.8;

    char finalString[110];
    char influxString[100] = "things,thing-id=andrews-esp32-nodemcu,city=berlin,location=oderstrasse,room=andrews cpu_temp_c=";
    char tempString[10];
    snprintf(tempString, 10, "%f", tempInC);

    strcpy(finalString, influxString);
    strcat(finalString, tempString);
    Serial.println(finalString);
    client.publish(MQTT_SERIAL_PUBLISH_CPU, finalString);
    delay(10);
}

void resetHeap()
{
    heap_caps_dump_all();
}

void readSystemStats()
{
    char finalString[400];

    strcpy(finalString, "things,thing-id=andrews-esp32-nodemcu,city=berlin,location=oderstrasse,room=andrews free_heap_size=");

    char b[20];
    snprintf(b, 20, "%ld", xPortGetFreeHeapSize());
    strcat(finalString, b);

    strcat(finalString, ",minimun_ever_free_heap_size=");
    char c[20];
    snprintf(c, 20, "%ld", xPortGetMinimumEverFreeHeapSize());
    strcat(finalString, c);

    strcat(finalString, ",heap_cap_intergrity=");
    char d[20];
    snprintf(d, 20, "%ld", heap_caps_check_integrity_all(true));
    strcat(finalString, d);

    strcat(finalString, ",heap_caps_free_size_8=");
    char e[20];
    snprintf(e, 20, "%ld", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    strcat(finalString, e);

    strcat(finalString, ",heap_caps_free_size_32=");
    char f[20];
    snprintf(f, 20, "%ld", heap_caps_get_free_size(MALLOC_CAP_32BIT));
    strcat(finalString, f);

    strcat(finalString, ",dRAM=");
    char g[20];
    double dram = static_cast<double>(heap_caps_get_free_size(MALLOC_CAP_8BIT));
    snprintf(g, 20, "%ld", dram);
    strcat(finalString, g);

    strcat(finalString, ",dRAM_iRAM=");
    char h[20];
    double dramIram = static_cast<double>(heap_caps_get_free_size(MALLOC_CAP_32BIT));
    snprintf(h, 20, "%ld", dramIram);
    strcat(finalString, h);

    char i[20];
    strcat(finalString, ",iRAM=");
    double iram = dramIram - dram;
    snprintf(i, 20, "%ld", iram);
    strcat(finalString, i);

    strcat(finalString, ",heap_caps_largest_free_block_8=");
    char j[20];
    snprintf(j, 20, "%ld", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    strcat(finalString, j);

    strcat(finalString, ",heap_caps_largest_free_block_32=");
    char k[20];
    snprintf(k, 20, "%ld", heap_caps_get_largest_free_block(MALLOC_CAP_32BIT));
    strcat(finalString, k);

    strcat(finalString, ",smlst_free_mem_stack=");
    char l[20];
    snprintf(l, 20, "%ld", uxTaskGetStackHighWaterMark(NULL));
    strcat(finalString, l);

    Serial.println(finalString);
    client.publish(MQTT_SERIAL_PUBLISH_CPU, finalString);
    delay(10);
}

void timeLoop()
{
    Serial.print(rtc.now().hour(), DEC); 
    Serial.print(':');   
    Serial.print(rtc.now().minute(), DEC);
    Serial.print(':');
    Serial.print(rtc.now().second(), DEC);

    int newHour = rtc.now().hour();

    if (newHour != currentHour)
    {
        if (newHour < 24)
        {
            currentHour = rtc.now().hour();
            calibrateTime();
            Serial2.println(newHour);
            if (newHour > 21 || newHour < 9)
            {
                Serial2.println("<TURN_OFF>");
            }
            else
            {
                Serial2.println("<TURN_ON>");
            }
        }
    }
}

void calibrateTime()
{

    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

        http.begin(timeApiEndP);   //Specify the URL
        int httpCode = http.GET(); //Make the request

        if (httpCode > 0)
        { //Check for the returning code

            String payload;
            payload.reserve(300);
            payload = http.getString();
            if (httpCode == 200)
            {
                String newDateTime;
                newDateTime.reserve(300);
                newDateTime = extractDateTime(payload);
                Serial.println(newDateTime);

                int newYear = newDateTime.substring(0, 4).toInt();
                int newMonth = newDateTime.substring(5, 7).toInt();
                int newDay = newDateTime.substring(8, 10).toInt();
                int newHour = newDateTime.substring(11, 13).toInt();
                int newMinutes = newDateTime.substring(14, 16).toInt();
                int newSeconds = newDateTime.substring(17, 19).toInt();
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
            }
            else
            {
                // Send Error to Mailgun
                Serial.println(payload);
            }
        }

        else
        {
            Serial.println("Error on HTTP request");
        }
        http.end(); //Free the resources
    }

    delay(10);
}

String extractDateTime(String string)
{
    int startIndex = string.lastIndexOf("\"datetime\":\"");
    int endIndex = string.indexOf(",\"client_ip\"");
    return string.substring(startIndex + 12, endIndex - 1);
}
