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
    const String plantType;
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

const int pumpPin = 12;
const char systemId = "plant_system";
const float baudRate = 115200;
const int loopDelaymilli = 10000;
int loopDelay = loopDelaymilli;

Module modules[MODULE_COUNT] = {
    Module(A0, 0, '1', 70, 2, 622, 323, 40, false, "unknown"),
    Module(A1, 0, '2', 70, 3, 640, 323, 40, false, "unknown"), // Checked sensor values 2/6/2019 Plant two - Hanging plant
    Module(A2, 0, '3', 70, 4, 622, 323, 40, false, "unknown"),
    Module(A3, 0, '4', 90, 5, 664, 339, 60, false, "various"),    // Checked sensor values 8/5/2019 Mint - The low threshold ishigher as the mint was looking bad at 30%
    Module(A4, 0, '5', 70, 6, 672, 342, 40, false, "bonsai"),     // Checked sensor values 8/5/2019 RoseMary
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
        delay(500);
    }
    pinMode(pumpPin, OUTPUT);
    for (int i = 2; i < (MODULE_COUNT + 3); i++)
    {
        modules[i].isPumping = false;
    }
    delay(1000);
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

        //Tags /
        char city = ",city=Berlin";
        char location = ",location=oderstrasse";
        char room = ",room=andrews";
        totalChar += strlen(city) + strlen(location) + strlen(room);

        char typeKey = ",plant_type=";
        String typeValue = currentModule.plantType;
        char typeBuf[typeValue.length() + 1];
        typeValue.toCharArray(typeBuf, typeValue.length() + 1);
        totalChar += typeValue.length() + strlen(typeKey);

        char seporatorSpace = " ";
        totalChar += strlen(seporatorSpace);

        //Fields
        char plantIdKey = ",plant_id=";
        String plantIdValue = currentModule.id;
        char idBuf[plantIdValue.length() + 1];
        plantIdValue.toCharArray(idBuf, plantIdValue.length() + 1);
        totalChar += strlen(plantIdKey) + plantIdValue.length();

        char servoPinKey = ",servo_pin=";
        char servoPinValue = currentModule.servoPin;
        totalChar += strlen(servoPinKey) + strlen(servoPinValue);

        char readPinKey = ",read_pin=";
        char readPinValue = currentModule.readPin;
        totalChar += strlen(readPinKey) + strlen(readPinValue);

        char settingHighKey = ",moi_setting_high=";
        char settingHighValue = currentModule.moistureSettingHigh;
        totalChar += strlen(settingHighKey) + strlen(settingHighValue);

        char settingLowKey = ",moi_setting_low=";
        char settingLowValue = currentModule.moistureSettingLow;
        totalChar += strlen(settingLowKey) + strlen(settingLowValue);

        char plantDataStringKey = ",sensor_low_value=";
        char plantDataStringValue = currentModule.sensorUpperValue;
        totalChar += strlen(plantDataStringKey) + strlen(plantDataStringValue);

        currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);

        char levelKey = ",moisture_level=";
        char levelValue = currentModule.currentPercentage;
        totalChar += strlen(levelKey) + strlen(levelValue);

        char deadzone = '\0';

        if (currentModule.currentPercentage < currentModule.moistureSettingLow)
        {
            currentModule.isPumping = true;
            needsPump = true;
            digitalWrite(currentModule.servoPin, LOW);

            //Opening servo
            deadzone = ",in_dead_zone=f";
        }
        if (currentModule.currentPercentage >= currentModule.moistureSettingLow && currentModule.currentPercentage <= currentModule.moistureSettingHigh)
        {
            if (!currentModule.isPumping)
            {
                digitalWrite(currentModule.servoPin, HIGH);
            }

            //the deadzone
            deadzone = ",in_dead_zone=t";
        }
        if (currentModule.currentPercentage > currentModule.moistureSettingHigh)
        {
            currentModule.isPumping = false;
            digitalWrite(currentModule.servoPin, HIGH);

            //Opening servo
            deadzone = ",in_dead_zone=f";
        }
        totalChar += strlen(deadzone);

        char servoOpen = '\0';

        byte servoPinState = digitalRead(currentModule.servoPin);
        if (servoPinState == LOW)
        {
            servoOpen = ",servo_open=t";
        }
        else
        {
            servoOpen = ",servo_open=f";
        }
        totalChar += strlen(servoOpen);

        char pumpOpen = '\0';

        byte pumpPinState = digitalRead(pumpPin);
        if (pumpPinState == LOW)
        {
            pumpOpen = ",pump_open=f";
        }
        else
        {
            pumpOpen = ",pump_open=t";
        }

        totalChar += strlen(pumpOpen);
        Serial.println("Buffer length in");
        Serial.println(city);
        Serial.println(totalChar);

        char buf[totalChar + 20];
        strcpy(buf,systemId);

        // Tags
        strcat(buf,city);
        strcat(buf,location);
        strcat(buf,room);
        strcat(buf,typeKey);
        strcat(buf,typeBuf);
        strcat(buf,seporatorSpace);
        //Fields
        strcat(buf,plantIdKey);
        strcat(buf,idBuf);
        strcat(buf,servoPinKey);
        strcat(buf,servoPinValue);
        strcat(buf,readPinKey);
        strcat(buf,readPinValue);
        strcat(buf,settingHighKey);
        strcat(buf,settingHighValue);
        strcat(buf,settingLowKey);
        strcat(buf,settingLowValue);
        strcat(buf,plantDataStringKey);
        strcat(buf,plantDataStringValue);
        strcat(buf,levelKey);
        strcat(buf,levelValue);
        strcat(buf,deadzone);
        strcat(buf,servoOpen);
        strcat(buf,pumpOpen);
        Serial.println("Buffer length out");
        Serial.println(strlen(buf));
        Serial.println(buf);
        delay(100);
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

char *stringToChar(String toChange)
{
    //        strcpy (buf, "But does it get goat's blood out?");
    //        return buf;
    //        int str_len = strlen(toChange)() + 1;
    //        char * buf = (char *) malloc (str_len);
    //        toChange.toCharArray(char_array, str_len);
    //        return char_array1;
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
