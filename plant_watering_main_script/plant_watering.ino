//////////////////////////////////////////////////////////////////
                   /// ARDUINO SOIL MOISTURE        
                                    
// https://github.com/earyzhe/plant_watering_system_arduino 
     
////////////////////////////////////////////////////////////////////

class Module{
    public:
      Module(int, int, char, int, int, int, int);
      int readPin;
      int currentPercentage;
      String id;
      int moistureSetting;
      int servoPin;
      int sensorLowerValue;
      int sensorUpperValue;
};

/// Constructor for each module
Module::Module (int a, int b, char c, int d, int e, int f, int g) {
    readPin = a;
    currentPercentage = b;
    id = c;
    moistureSetting = d;
    servoPin = e;
    sensorLowerValue = f;
    sensorUpperValue = g;

}

#define MODULE_COUNT 6

int pumpPin = 12;

Module modules[MODULE_COUNT] = {
        Module(A0,0,'1', 50, 2, 622, 323),
        Module(A1,0,'2', 50, 3, 622, 323),
        Module(A2,0,'3', 50, 4, 622, 323),
        Module(A3,0,'4', 50, 5, 664, 339), // Checked sensor values 8/5/2019 Mint
        Module(A4,0,'5', 50, 6, 672, 342), // Checked sensor values 8/5/2019 RoseMary
        Module(A5,0,'6', 50, 7, 612, 320),
//        Module(A7,0,'8', 50, 9, 882, 734), // Checked sensor values 8/5/2019 Peace Lily
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
}

void loop() {

    bool needsPump = false;

    for (int i = 0; i < MODULE_COUNT; i++) {

        Serial.print('\n');
      
        Module currentModule = modules[i];
        printId(currentModule.id);
        currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);

        Serial.print(" - ");


        if (currentModule.currentPercentage < currentModule.moistureSetting){
            digitalWrite(currentModule.servoPin, LOW);
            Serial.print(currentModule.servoPin);
            Serial.print(" pin is Watering... - ");
            needsPump = true;
        }
        else{
            Serial.print(currentModule.servoPin);
            Serial.print(" pin is Wet enough - ");
            digitalWrite(currentModule.servoPin, HIGH);
        }
        
        printValueToSerial(currentModule.currentPercentage);
        printSetting(currentModule.moistureSetting);
    }

    if (needsPump){
        digitalWrite(pumpPin, HIGH);
    }
    else{
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
