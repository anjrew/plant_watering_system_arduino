//////////////////////////////////////////////////////////////////
                   /// ARDUINO SOIL MOISTURE        
                                    
// https://github.com/earyzhe/plant_watering_system_arduino 
     
////////////////////////////////////////////////////////////////////

class Module{
    public:
      Module(int, int, char, int, int, int, int, int, bool);
      int readPin;
      int currentPercentage;
      String id;
      int moistureSettingHigh;
      int servoPin;
      int sensorLowerValue;
      int sensorUpperValue;
      int moistureSettingLow;
      bool isPumping;
};

/// Constructor for each module
Module::Module (int a, int b, char c, int d, int e, int f, int g, int h, bool i) {
    readPin = a;
    currentPercentage = b;
    id = c;
    moistureSettingHigh = d;
    servoPin = e;
    sensorLowerValue = f;
    sensorUpperValue = g;
    moistureSettingLow = h;
    isPumping = i;
}

#define MODULE_COUNT 7

int pumpPin = 12;
String systemId = "plntsys47odstrbed"
int loopDelaymilli = 10000;

Module modules[MODULE_COUNT] = {
        Module(A0, 0, '1', 70, 2, 622, 323, 40, false),
        Module(A1, 0, '2', 70, 3, 640, 323, 40, false), // Checked sensor values 2/6/2019 Plant two - Hanging plant 
        Module(A2, 0, '3', 70, 4, 622, 323, 40, false),
        Module(A3, 0, '4', 90, 5, 664, 339, 60, false), // Checked sensor values 8/5/2019 Mint - The low threshold ishigher as the mint was looking bad at 30%
        Module(A4, 0, '5', 70, 6, 672, 342, 40, false), // Checked sensor values 8/5/2019 RoseMary
        Module(A5, 0, '6', 70, 7, 700, 372, 40, false), // Checked sensor values 2/6/2019 Cactus
        Module(A6, 0, '7', 70, 8, 597, 287, 40, false), // Checked sensor values 8/5/2019 Peace Lily

//        Module(A7,0,'8', 50, 9, 882, 734), 
    } ;
   //        Module(A6,0,'7', 50, 8, 550, 274),



void setup() {
    Serial.begin(9600);
    // Initialise pins
    for (int i = 2; i < (MODULE_COUNT + 3); i++) {
        Serial.print("Pin ");
        Serial.print(i);
        pinMode(i, OUTPUT);
        Serial.print(" is set to OUTPUT\n" );
        delay(500);
    }
    pinMode(pumpPin,OUTPUT);
    for (int i = 2; i < (MODULE_COUNT + 3); i++) {
        modules[i].isPumping = false;
    }
}

void loop() {

    bool needsPump = false;
    String serialString = ''

    for (int i = 0; i < MODULE_COUNT; i++) {
      
        Module currentModule = modules[i];
        String plantDataString = ""

        plantDataString += currentModule.id + ",";

        currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);

        if (currentModule.currentPercentage < currentModule.moistureSettingLow){
            currentModule.isPumping = true;
            loopDelaymilli = 100;
            digitalWrite(currentModule.servoPin, LOW);

            Serial.print(currentModule.servoPin);
            //Opening servo
            plantDataString += "in_dead_zone=false"
        }
        if (currentModule.currentPercentage >= currentModule.moistureSettingLow && currentModule.currentPercentage <= currentModule.moistureSettingHigh){
            if (!currentModule.isPumping){
                digitalWrite(currentModule.servoPin, HIGH);
             }

            //the deadzone
            plantDataString += "in_dead_zone=true"
        }  
       if (currentModule.currentPercentage > currentModule.moistureSettingHigh){
            currentModule.isPumping = false;
            loopDelaymilli = 10000;
            digitalWrite(currentModule.servoPin, HIGH);

            //Opening servo
            plantDataString += "in_dead_zone=false"
        }
        
        plantDataString += "," + "moisture_level=" + currentModule.currentPercentage;

        byte pinState = digitalRead(currentModule.servoPin);
        if (pinState == LOW) {
            plantDataString += ",servo_open=true",
        } else {
            plantDataString += ",servo_open=false",
        }
        plantDataString += "\x"
        serialString += plantDataString
    }

    if (needsPump){
        serialString += "," + "pump_status=true";
        digitalWrite(pumpPin, HIGH);
    }
    else{
        serialString + "," + "pump_status=false";
        digitalWrite(pumpPin, LOW);
    }

    Serial.println(serialString)
    delay(loopDelaymilli);
}

int convertToPercent(int sensorValue, Module module){
    Serial.print("- Sensor value = ");
    Serial.print(sensorValue);
    int percentValue = 0;
    percentValue = map(sensorValue, module.sensorLowerValue, module.sensorUpperValue , 0, 100);
    return percentValue;
}

void printSetting(int setting){
       Serial.print("Setting: ");
       Serial.print(setting);
       Serial.println("%");
 }

void printId(String id){
    Serial.print(" - Plant ");
    Serial.println(id);  } 

void printValueToSerial(int currentPercentage){
    Serial.print(" - Moisture Percent: ");
    Serial.print(currentPercentage);
    Serial.println("%");
}

