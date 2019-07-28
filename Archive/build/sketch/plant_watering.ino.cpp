#include <Arduino.h>
#line 1 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
#line 1 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
//////////////////////////////////////////////////////////////////
                   /// ARDUINO SOIL MOISTURE        
                                    
// https://github.com/earyzhe/plant_watering_system_arduino 
     
////////////////////////////////////////////////////////////////////

class Module{
    public:
      Module(int, int, char, int, int);
      int pin;
      int value;
      String id;
      int moistureSetting;
      int servoPin;
};

/// Constructor for each module
Module::Module (int a, int b, char c, int d, int e) {
  pin = a;
  value = b;
  id = c;
  moistureSetting = d;
  servoPin = e;
}

int moduleCount = 7;
int pumpPin = 13;

Module modules[8] = {
        Module(A0,0,'1', 100, 2),
        Module(A1,0,'2', 50, 3),
        Module(A2,0,'3', 50, 4),
        Module(A3,0,'4', 50, 5),
        Module(A4,0,'5', 50, 6),
        Module(A5,0,'6', 50, 7),
        Module(A6,0,'7', 50, 8),
        Module(A7,0,'8', 50, 9),    
    } ;


#line 42 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
void setup();
#line 51 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
void loop();
#line 76 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
int convertToPercent(int sensorValue);
#line 82 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
void printSetting(int setting);
#line 90 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
void printId(String id);
#line 97 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
void printValueToSerial(int value);
#line 42 "/Users/earyzhe/dev/Arduino/plant_watering/plant_watering.ino"
void setup() {
    Serial.begin(9600);
    // Initialise pins
    for (int i = 2; i <= 10; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(pumpPin,OUTPUT);
}

void loop() {

//    for (int i = 0; i <= moduleCount; i++) {
      
        Module currentModule = modules[0];
        printId(currentModule.id);
        Serial.println(analogRead(currentModule.pin));
        currentModule.value = convertToPercent(analogRead(currentModule.pin));

        if (currentModule.value < currentModule.moistureSetting){
            digitalWrite(currentModule.servoPin, HIGH);
            digitalWrite(pumpPin, HIGH);
            Serial.print("Watering...\n");
        }
        else{
            digitalWrite(currentModule.servoPin, LOW);
            digitalWrite(pumpPin, LOW);
        }
        
        printValueToSerial(currentModule.value);
        printSetting(currentModule.moistureSetting);
//    }
//    delay(1000);
}

int convertToPercent(int sensorValue){
    int percentValue = 0;
    percentValue = map(sensorValue, 622, 323, 0, 100);
    return percentValue;
}

void printSetting(int setting){
        delay(1000);
        Serial.print("Setting: ");
        delay(1000);
        Serial.print(setting);
        Serial.println("%");
 }

 void printId(String id){
        Serial.print("\nPlant ");
        delay(1000);
        Serial.println(id);
        delay(1000);
  } 

void printValueToSerial(int value){

    Serial.print("Moisture Percent: ");
    delay(1000);
    Serial.print(value);
    delay(100);
    Serial.println("%");
    delay(1000);
}

