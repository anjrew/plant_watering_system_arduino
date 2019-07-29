//////////////////////////////////////////////////////////////////
/// ARDUINO SOIL MOISTURE

// https://github.com/earyzhe/plant_watering_system_arduino

////////////////////////////////////////////////////////////////////

class Module
{
public:
    Module(int, int, char, int, int, int, int, int, bool, String);
    int readPin;
    int currentPercentage;
    String id;
    int moistureSettingHigh;
    int servoPin;
    int sensorLowerValue;
    int sensorUpperValue;
    int moistureSettingLow;
    bool isPumping;
    String plantType;
};

/// Constructor for each module
Module::Module(int a, int b, char c, int d, int e, int f, int g, int h, bool i, String j)
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

int pumpPin = 12;
String systemId = "plntsys";
int loopDelaymilli = 10000;
int loopDelay = loopDelaymilli;

Module modules[MODULE_COUNT] = {
    Module(A0, 0, '1', 70, 2, 622, 323, 40, false, "unknown"),
    Module(A1, 0, '2', 70, 3, 640, 323, 40, false, "unknown"), // Checked sensor values 2/6/2019 Plant two - Hanging plant
    Module(A2, 0, '3', 70, 4, 622, 323, 40, false, "unknown"),
    Module(A3, 0, '4', 90, 5, 664, 339, 60, false, "various"), // Checked sensor values 8/5/2019 Mint - The low threshold ishigher as the mint was looking bad at 30%
    Module(A4, 0, '5', 70, 6, 672, 342, 40, false, "bonsai"), // Checked sensor values 8/5/2019 RoseMary
    Module(A5, 0, '6', 60, 7, 700, 372, 30, false, "cactus"), // Checked sensor values 2/6/2019 Cactus
    Module(A6, 0, '7', 70, 8, 597, 287, 40, false, "peace_lily"), // Checked sensor values 8/5/2019 Peace Lily

    //        Module(A7,0,'8', 50, 9, 882, 734),
};
//        Module(A6,0,'7', 50, 8, 550, 274),

void setup()
{
    Serial.begin(9600);
    // Initialise pins
    for (int i = 2; i < (MODULE_COUNT + 3); i++)
    {
        Serial.print("Pin ");
        Serial.print(i);
        pinMode(i, OUTPUT);
        Serial.print(" is set to OUTPUT\n");
        delay(500);
    }
    pinMode(pumpPin, OUTPUT);
    for (int i = 2; i < (MODULE_COUNT + 3); i++)
    {
        modules[i].isPumping = false;
    }
}

void loop()
{

    bool needsPump = false;
    for (int i = 0; i < MODULE_COUNT; i++)
    {

        Module currentModule = modules[i];
        String plantDataString = "";

        // Project
        plantDataString += systemId + String(",");

        //Tags
        plantDataString += String("city=") + String("Berlin") + String(",");
        plantDataString += String("location=") + String("oderstrasse") + String(",");
        plantDataString += String("room=") + String("andrews") + String(",");
        plantDataString += String("plant_type=") + String(currentModule.plantType);

        plantDataString += String(" ");
        
        //Fields
        plantDataString += String("plant_id=") + String(currentModule.id) + String(",");
        plantDataString += String("servo_pin=") + String(currentModule.servoPin) + String(",");
        plantDataString += String("read_pin=") + String(currentModule.readPin) + String(",");
        plantDataString += String("moi_setting_high=") + String(currentModule.moistureSettingHigh) + String(",");
        plantDataString += String("moi_setting_low=") + String(currentModule.moistureSettingLow) + String(",");
        plantDataString += String("sensor_low_value=") + String(currentModule.sensorLowerValue) + String(",");
        plantDataString += String("sensor_high_value=") + String(currentModule.sensorUpperValue) + String(",");
        currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);
        plantDataString += String("moisture_level=") + String(currentModule.currentPercentage) + String(",");

        if (currentModule.currentPercentage < currentModule.moistureSettingLow)
        {
            currentModule.isPumping = true;
            needsPump = true;
            digitalWrite(currentModule.servoPin, LOW);

            //Opening servo
            plantDataString += String("in_dead_zone=false");
        }
        if (currentModule.currentPercentage >= currentModule.moistureSettingLow && currentModule.currentPercentage <= currentModule.moistureSettingHigh)
        {
            if (!currentModule.isPumping)
            {
                digitalWrite(currentModule.servoPin, HIGH);
            }

            //the deadzone
            plantDataString += String("in_dead_zone=true");
        }
        if (currentModule.currentPercentage > currentModule.moistureSettingHigh)
        {
            currentModule.isPumping = false;
            digitalWrite(currentModule.servoPin, HIGH);

            //Opening servo
            plantDataString += String("in_dead_zone=false");
        }

        byte servoPinState = digitalRead(currentModule.servoPin);
        if (servoPinState == LOW)
        {
            plantDataString += String(",servo_open=true");
        }
        else
        {
            plantDataString += String(",servo_open=false");
        }

        byte pumpPinState = digitalRead(pumpPin);
        if (pumpPinState == LOW)
        {
            plantDataString += String(",") + String("pump_status=false");
        }
        else
        {
            plantDataString += String(",") + String("pump_status=true");
        }

        Serial.println(plantDataString);
    }

    if (needsPump)
    {
        digitalWrite(pumpPin, HIGH);
        loopDelay = 0;
    }
    else
    {
        digitalWrite(pumpPin, LOW);
        loopDelay = loopDelaymilli;
    }
    delay(loopDelay);
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
