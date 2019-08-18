  #include <WiFi.h>
  #include <PubSubClient.h>
  #include <HardwareSerial.h>

  // For RTC
  #include <Wire.h>  // for I2C with RTC module
  #include "RTClib.h"
  RTC_DS3231 rtc;
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  int currentHour = 0;
  
  // Update these with values suitable for your network.
  // Monkey Park
  //const char* ssid = "MonkeyPark";
  //const char* password = "MonkeyPark";
  
  // Dads House
  //const char* ssid = "BrightBox-9sbpsc";
  //const char* password = "tag-dread-tame";
  
  // Oder 47
  const char *ssid = "(Don't mention the war)";
  const char *password = "56939862460419967485";

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
  
  WiFiClient wifiClient;
  PubSubClient client(wifiClient);
  
  int loops = 0;
  const int tempCheckLoops = 800000;

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
      if (loops > tempCheckLoops){
        loops = 0;
          readCpuTemp();
        }
      loops++;
      readSoftwareSerial2();
      timeLoop();
  }
  

  void setupSerialPorts(){
      Serial.begin(baudRate);
      Serial.setTimeout(500); 
      Serial2.begin(baudRate);
      Serial2.println("Hello, world?"); 
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
  
  void readSoftwareSerial2(){ 
    if (Serial2.available()) {
        String output = Serial2.readStringUntil('\n');
        Serial.println(output);
        int str_len = output.length() + 1; 
        char char_array[str_len];
        output.toCharArray(char_array, str_len);
        client.publish(MQTT_SERIAL_PUBLISH_PLANTS, char_array);
        delay(10);
    }
  }
  
  
  void readCpuTemp() {
     double tempInC = (temprature_sens_read() - 32) / 1.8;
     String tempString = String(tempInC);
    // Convert raw temperature in F to Celsius degrees
    String influxString = "things,thing-id=andrews-esp32-nodemcu,city=berlin,location=oderstrasse,room=andrews cpu_temp_c=";
    
    influxString += tempString;
    int str_len = influxString.length() + 1; 
    char char_array[str_len];
    influxString.toCharArray(char_array, str_len);
    Serial.println(influxString);
    client.publish(MQTT_SERIAL_PUBLISH_CPU, char_array);
    delay(10);
  }

void timeLoop () {
  DateTime now = rtc.now();;
  if (now.hour() != currentHour) {
    if (currentHour < 24) {
      currentHour = now.hour();

      if (now.hour() > 21 || now.hour() < 9) {
        Serial.print("The current hour is ");
        Serial.println(currentHour);
        Serial2.println("TURN_OFF");
      }
    }
  }
}


  
