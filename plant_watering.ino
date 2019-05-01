////////////////////////////////////////////////////////
                   /// ARDUINO SOIL MOISTURE        
                                    
// https://github.com/earyzhe/plant_watering_system_arduino 
     
/////////////////////////////////////////////////////////

class Module{
    public:
      Module(int, int, char, int);
      int pin;
      int value;
      String id;
      int moistureSetting;
};

Module::Module (int a, int b, char c, int d) {
  pin = a;
  value = b;
  id = c;
  moistureSetting= d;
}

int moduleCount = 7;
Module modules[8] = {
    
        Module(A0,0,'1', 100),
        Module(A1,0,'2', 50),
        Module(A2,0,'3', 50),
        Module(A3,0,'4', 50),
        Module(A4,0,'5', 50),
        Module(A5,0,'6', 50),
        Module(A6,0,'7', 50),
        Module(A7,0,'8', 50),    
    } ;


void setup() {
    Serial.begin(9600);
}

void loop() {

//    for (int i = 0; i <= moduleCount; i++) {
      
        Module currentModule = modules[0];
        printId(currentModule.id);
        Serial.println(analogRead(currentModule.pin));
        currentModule.value = convertToPercent(analogRead(currentModule.pin));

        if (currentModule.value < currentModule.moistureSetting){

          Serial.print("Watering...\n");
         }
        
        printValueToSerial(currentModule.value);
        printSetting(currentModule.moistureSetting);
//    }
//    delay(1000);
}

int convertToPercent(int sensorValue){
    int percentValue = 0;
    percentValue = map(sensorValue, 623, 323, 0, 100);
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
