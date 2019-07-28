#include <WiFi.h>
#include <PubSubClient.h>


// Update these with values suitable for your network.
// Monkey Park
//const char* ssid = "MonkeyPark";
//const char* password = "MonkeyPark";

//const char* ssid = "BrightBox-9sbpsc";
//const char* password = "tag-dread-tame";

// Dads House
const char* ssid = "(Don't' mention the war)";
const char* password = "56939862460419967485";
const char* mqtt_server = "ec2-34-217-213-209.us-west-2.compute.amazonaws.com";
#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_SERIAL_PUBLISH_TEST "/test"
#define MQTT_SERIAL_RECEIVER_TEST "/test"
#define MQTT_SERIAL_PUBLISH_CH "/icircuit/ESP32/serialdata/tx"
#define MQTT_SERIAL_RECEIVER_CH "/icircuit/ESP32/serialdata/rx"

WiFiClient wifiClient;

PubSubClient client(wifiClient);

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish(MQTT_SERIAL_PUBLISH_TEST, "weather,location=us-midwest temperature=82");
      // ... and resubscribe
      client.subscribe(MQTT_SERIAL_RECEIVER_TEST);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:");  
    Serial.write(payload, length);
    Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
}

void loop() {
   client.loop();
   if (Serial.available() > 0) {
     char mun[501];
     memset(mun,0, 501);
     Serial.readBytesUntil( '\n',mun,500);
     publishSerialData(mun);
   } else {

     client.publish(MQTT_SERIAL_PUBLISH_TEST, "Loop is working");
     delay(5000);
   }
 }
 
