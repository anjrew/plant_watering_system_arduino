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

    for (int i = 0; i < MODULE_COUNT; i++) {

        Serial.print('\n');
      
        Module currentModule = modules[i];
        printId(currentModule.id);
        currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);

        Serial.print(" - ");


        if (currentModule.currentPercentage < currentModule.moistureSettingLow){
            currentModule.isPumping = true;
            digitalWrite(currentModule.servoPin, LOW);
            Serial.print("Opening Servo : ");
            Serial.print(currentModule.servoPin);
            needsPump = true;
            Serial.print(" pin is Watering... and pumping- ");
        }
        if (currentModule.currentPercentage >= currentModule.moistureSettingLow && currentModule.currentPercentage <= currentModule.moistureSettingHigh){
            Serial.print("Holding as is in the dead zone: ");
            if (!currentModule.isPumping){
                 digitalWrite(currentModule.servoPin, HIGH);
             }
        }
          
       if (currentModule.currentPercentage > currentModule.moistureSettingHigh){
            currentModule.isPumping = false;
            Serial.print("Closing  Servo : ");
            Serial.print(currentModule.servoPin);
            digitalWrite(currentModule.servoPin, HIGH);
            Serial.print(" pin is Wet enough - and Stoping pumping ");
        }
        
        printValueToSerial(currentModule.currentPercentage);
        Serial.print("High  Setting : ");
        printSetting(currentModule.moistureSettingHigh);
        Serial.print(" Low  Setting : ");
        printSetting(currentModule.moistureSettingLow );

        if (currentModule.isPumping){
          Serial.println("Current module is pumping");
        }

        byte pinState = digitalRead(currentModule.servoPin);
        if (pinState == LOW) {
           Serial.println("The current servo pin is open");
        } else {
           Serial.println("The servo pin is closed");
         }
    }

    if (needsPump){
        Serial.print(" Turning Pump on : ");
        digitalWrite(pumpPin, HIGH);
    }
    else{
        Serial.print(" Turning Pump off : ");
        digitalWrite(pumpPin, LOW);
    }

    delay(1000);
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

