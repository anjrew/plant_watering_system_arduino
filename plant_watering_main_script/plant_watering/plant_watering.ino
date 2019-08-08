//////////////////////////////////////////////////////////////////
/// ARDUINO SOIL MOISTURE

// https://github.com/earyzhe/plant_watering_system_arduino

////////////////////////////////////////////////////////////////////

class Module
{
public:
    Module(int, int, char, int, int, int, int, int, bool, char*);
    int readPin;
    int currentPercentage;
    char id;
    int moistureSettingHigh;
    int servoPin;
    int sensorLowerValue;
    int sensorUpperValue;
    int moistureSettingLow;
    bool isPumping;
    char* plantType;
};

/// Constructor for each module
Module::Module(int a, int b, char c, int d, int e, int f, int g, int h, bool i, char* j )
{
    readPin = a;
    currentPercentage = b;
    id = c;
    moistureSettingHigh = d;
    servoPin = e;
    sensorLowerValue = f;
    sensorUpperValue = g;
    moistureSettingLow = h;
    isPumping = i;
    plantType = j;
}

#define MODULE_COUNT 7

const int pumpPin = 12;
const char systemId = "plant_system";
const float baudRate = 115200;

// Time in milliseconds
//const unsigned long  loopDelayNormalSecs = 600;
const unsigned long  loopDelayNormalSecs = 6;
const int loopDelayPumpmilli = 100;

Module modules[MODULE_COUNT] = {
    Module(A0, 0, '1', 70, 2, 622, 323, 40, false, "unknown"),
    Module(A1, 0, '2', 70, 3, 640, 323, 40, false, "unknown"),    /// Checked sensor values 2/6/2019 Plant two - Hanging plant
    Module(A2, 0, '3', 70, 4, 622, 318, 40, false, "unknown"),
    Module(A3, 0, '4', 90, 5, 664, 339, 60, false, "various"),    // Big plant bed
    Module(A4, 0, '5', 70, 6, 672, 308, 40, false, "bonsai"),     // Checked sensor values 8/5/2019 bonsai
    Module(A5, 0, '6', 60, 7, 700, 372, 30, false, "cactus"),     // Checked sensor values 2/6/2019 Cactus
    Module(A6, 0, '7', 70, 8, 597, 287, 40, false, "peace_lily"), // Checked sensor values 8/5/2019 Peace Lily
    //        Module(A7,0,'8', 50, 9, 882, 734),
};

void setup()
{
    Serial.begin(baudRate);
    // Initialise pins
    for (int i = 2; i < (MODULE_COUNT + 3); i++)
    {
        Serial.print("Pin ");
        Serial.print(i);
        pinMode(i, OUTPUT);
        Serial.print(" is set to OUTPUT\n");
        digitalWrite(i, HIGH);
        delay(100);
    }
    pinMode(pumpPin, OUTPUT);
    digitalWrite(pumpPin, LOW);
    for (int i = 2; i < (MODULE_COUNT + 3); i++)
    {
        modules[i].isPumping = false;
    }
    Serial.println("Finished setup");
}

void loop()
{

    bool needsPump = false;

    for (int i = 0; i < MODULE_COUNT; i++)
    {
        Module currentModule = modules[i];
        int totalChar = strlen(systemId);
        // Project /
        Serial.print("plant_system");
        //Tags /
        Serial.print(",city=Berlin");
        Serial.print(",location=oderstrasse");
        Serial.print(",room=andrews");
        Serial.print(",plant_type=");
        Serial.print(currentModule.plantType);
        Serial.print(",plant_id=");
        Serial.print(currentModule.id);
        
        Serial.print(" ");

        //Fields
        Serial.print("servo_pin=");
        Serial.print(currentModule.servoPin);

        Serial.print(",read_pin=");
        Serial.print(currentModule.readPin);

        Serial.print(",sensor_reading=");
        Serial.print(analogRead(currentModule.readPin));

        Serial.print(",moi_setting_high=");
        Serial.print(currentModule.moistureSettingHigh);

        Serial.print(",moi_setting_low=");
        Serial.print(currentModule.moistureSettingLow);

        Serial.print(",sensor_low_value=");
        Serial.print(currentModule.sensorUpperValue);
        delay(1);

        currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);

        Serial.print(",moisture_level=");
        Serial.print(currentModule.currentPercentage);
        
        if (currentModule.currentPercentage < currentModule.moistureSettingLow)
        {
            currentModule.isPumping = true;
            needsPump = true;
            digitalWrite(currentModule.servoPin, LOW);

            //Opening servo
            Serial.print(",in_dead_zone=f");
        }
        if (currentModule.currentPercentage >= currentModule.moistureSettingLow && currentModule.currentPercentage <= currentModule.moistureSettingHigh)
        {
            if (!currentModule.isPumping)
            {
                digitalWrite(currentModule.servoPin, HIGH);
            }

            //the deadzone
            Serial.print(",in_dead_zone=t");
        }
        if (currentModule.currentPercentage > currentModule.moistureSettingHigh)
        {
            currentModule.isPumping = false;
            digitalWrite(currentModule.servoPin, HIGH);

            //Opening servo
            Serial.print(",in_dead_zone=f");
        }

        byte servoPinState = digitalRead(currentModule.servoPin);
        if (servoPinState == LOW)
        {
            Serial.print(",servo_open=t");
        }
        else
        {
            Serial.print(",servo_open=f");
        } 

        byte pumpPinState = digitalRead(pumpPin);
        
        if (pumpPinState == LOW)
        {
            Serial.println(",pump_open=f");
        }
        else
        {
            Serial.println(",pump_open=t");
        }
        delay(10);
    }

    if (needsPump)
    {
        digitalWrite(pumpPin, HIGH);
        delay(loopDelayPumpmilli);
    }
    else
    {
        digitalWrite(pumpPin, LOW);
        int p = 0;
        while (p < loopDelayNormalSecs) {
        delay(1000);
        p++;
      }

    }
}

int convertToPercent(int sensorValue, Module module)
{
    int percentValue = 0;
    percentValue = map(sensorValue, module.sensorLowerValue, module.sensorUpperValue, 0, 100);
    return percentValue;
}

void printSetting(int setting)
{
    Serial.print("Setting: ");
    Serial.print(setting);
    Serial.println("%");
}

void printId(String id)
{
    Serial.print(" - Plant ");
    Serial.println(id);
}

void printValueToSerial(int currentPercentage)
{
    Serial.print(" - Moisture Percent: ");
    Serial.print(currentPercentage);
    Serial.println("%");
}
