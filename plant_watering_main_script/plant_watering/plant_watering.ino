//////////////////////////////////////////////////////////////////
/// ARDUINO SOIL MOISTURE

// https://github.com/earyzhe/plant_watering_system_arduino

////////////////////////////////////////////////////////////////////
#define SYSTEMID "nano-01"

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
bool isSleeping = false;

// Time in milliseconds
const unsigned long  loopDelayNormalSecs = 600;
//const unsigned long  loopDelayNormalSecs = 1;
const int loopDelayPumpmilli = 100;

Module modules[MODULE_COUNT] = {
  Module(A0, 0, '1', 70, 2, 622, 323, 40, false, "scindapsus"),
  Module(A1, 0, '2', 70, 3, 640, 323, 40, false, "scindapsus"),    /// Checked sensor values 2/6/2019 Plant two - Hanging plant
  Module(A2, 0, '3', 70, 4, 622, 312, 40, false, "scindapsus"),
  Module(A3, 0, '4', 90, 5, 664, 339, 60, false, "tray"),    // Big plant bed
  Module(A4, 0, '5', 70, 6, 672, 165, 40, false, "bonsai"),     // Checked sensor values 8/5/2019 bonsai
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
     modules[i].isPumping = false;

    delay(100);
  }
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
  Serial.println("Finished setup");
}

void loop()
{
  checkPlants();
  if (Serial.available() > 0) {
    recvWithStartEndMarkers();
  }
}

void handleMessage(char* message) {
  Serial.print("In handle message with the  message ");
  Serial.println( message);
  if (message == "SLEEP") {
    isSleeping = true;
  }
  if (message == "WAKE") {
    isSleeping = false;
  }
}

void checkPlants() {
  bool needsPump = false;

  for (int i = 0; i < MODULE_COUNT; i++)
  {
    Module currentModule = modules[i];
    int totalChar = strlen(systemId);
    // Project /
    Serial.print("<plant_system");
    //Tags /
    Serial.print(",thing=");
    Serial.print(SYSTEMID);

    Serial.print(",plnt_typ=");
    Serial.print(currentModule.plantType);
    Serial.print(",id=");
    Serial.print(currentModule.id);

    Serial.print(" ");

    //Fields
    Serial.print("serv_pin=");
    Serial.print(currentModule.servoPin);

    Serial.print(",read_pin=");
    Serial.print(currentModule.readPin);

    Serial.print(",sensor_reading=");
    Serial.print(analogRead(currentModule.readPin));

    Serial.print(",moi_set_hi=");
    Serial.print(currentModule.moistureSettingHigh);

    Serial.print(",moi_set_lo=");
    Serial.print(currentModule.moistureSettingLow);

    Serial.print(",sens_lo_val=");
    Serial.print(currentModule.sensorLowerValue);

    Serial.print(",sens_hi_val=");
    Serial.print(currentModule.sensorUpperValue);
    delay(1);

    currentModule.currentPercentage = convertToPercent(analogRead(currentModule.readPin), currentModule);

    Serial.print(",moi_lvl=");
    Serial.print(currentModule.currentPercentage);

    if (currentModule.currentPercentage < currentModule.moistureSettingLow && !isSleeping)
    {
      currentModule.isPumping = true;
      needsPump = true;
      digitalWrite(currentModule.servoPin, LOW);

      //Opening servo
      Serial.print(",dead_zone=0");
    }
    if (currentModule.currentPercentage >= currentModule.moistureSettingLow && currentModule.currentPercentage <= currentModule.moistureSettingHigh)
    {
      if (!currentModule.isPumping)
      {
        digitalWrite(currentModule.servoPin, HIGH);
      }

      //the deadzone
      Serial.print(",ded_zn=1");
    }
    if (currentModule.currentPercentage > currentModule.moistureSettingHigh)
    {
      currentModule.isPumping = false;
      digitalWrite(currentModule.servoPin, HIGH);

      //Opening servo
      Serial.print(",ded_zn=0");
    }

    byte servoPinState = digitalRead(currentModule.servoPin);
    if (servoPinState == LOW)
    {
      Serial.print(",servo=1");
    }
    else
    {
      Serial.print(",servo=0");
    }

    byte pumpPinState = digitalRead(pumpPin);

    if (pumpPinState == LOW)
    {
      Serial.println(",pmp=0>");
    }
    else
    {
      Serial.println(",pmp=1>");
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

char* recvWithStartEndMarkers()
{
  const byte numChars = 255;
  char receivedChars[numChars];
  boolean newData = false;
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
  Serial.print("In recieved chars with the  message ");
  Serial.println(receivedChars);
  if (strcmp(receivedChars,"SLEEP")== 0) {
    isSleeping = true;
    Serial.println("The module is ALSEEP" );
  }
  if (strcmp(receivedChars,"WAKE")==0) {
    isSleeping = false;
    Serial.println("The module is AWAKE");
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
