#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

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
#define MQTT_SERIAL_PUBLISH "/test"
#define MQTT_SERIAL_RECEIVER "/test"

WiFiClient wifiClient;

PubSubClient client(wifiClient);

// SoftwareSerial mySerial(16, 17); // RX, TX
//HardwareSerial mySerial2(1); // RX,
//HardwareSerial Serial2(2);

char mystr[5];
int incomingByte = 0; // for incoming serial data
char b;
String str;

void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
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
            client.publish(MQTT_SERIAL_PUBLISH, "weather,location=us-midwest temperature=82");
            // ... and resubscribe
            client.subscribe(MQTT_SERIAL_RECEIVER);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:");
    Serial.write(payload, length);
    Serial.println();
}

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(500); // Set time out for
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    reconnect();

//    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial2.println("Hello, world?");
}

void publishSerialData(char *serialData)
{
    if (!client.connected())
    {
        reconnect();
    }
    client.publish(MQTT_SERIAL_PUBLISH, serialData);
}

void readSoftwareSerial2(){ // run over and over

  if (Serial2.available()) {
//    Serial.write(Serial2.2read());
      String output = Serial2.readStringUntil('\n');
      Serial.println(output);
  }
//  if (Serial.available()) {
//    Serial2.write(Serial.read());
//  }
}

void readCpuTemp() {
  Serial.print("Temperature: ");
  
  // Convert raw temperature in F to Celsius degrees
  Serial.print((temprature_sens_read() - 32) / 1.8);
  Serial.println(" C");
  delay(1000);
}

void loop()
{
    client.loop();
    if (Serial.available() > 0)
    {
        char mun[501];
        memset(mun, 0, 501);
        Serial.readBytesUntil('\n', mun, 500);
//        publishSerialData(mun);
    }
    else
    {

//        client.publish(MQTT_SERIAL_PUBLISH, "weather,location=us-eastside temperature=124");
        delay(5000);
    }
    readCpuTemp();
//    readSoftwareSerial1();
    readSoftwareSerial2();
}
