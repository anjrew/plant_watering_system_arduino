//////////////////////////////////////////////
  //        ARDUINO SOIL MOISTURE        //
 //                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

class Module{
    public:
      Module(int, int, char, int);
      int pin;
      int value;
      String id;
      int moistureSetting;
};

Module::Module (int a, int b, char c, int) {
  pin = a;
  value = b;
  id = c;
}

int moduleCount = 8;
Module modules[8] = {
    
        Module(A0,0,'1', 50),
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

    for (int i = 0; i <= moduleCount; i++) {
      
        Module currentModule = modules[i];
        Serial.print("\nCurrent plant ");
        Serial.println(currentModule.id);
        currentModule.value = analogRead(currentModule.pin);
        printValueToSerial(currentModule.value);

        delay(1000);
    }
    delay(1000);
}

int convertToPercent(int sensorValue){
    int percentValue = 0;
    percentValue = map(sensorValue, 1023, 465, 0, 100);
    return percentValue;
}

void printValueToSerial(int value){

    int percent = convertToPercent(value);
    Serial.print("Analog Value: ");
    Serial.print(value);
    Serial.print("\nPercent: ");
    Serial.print(percent);
    Serial.println("%");
}
