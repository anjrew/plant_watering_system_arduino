/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
char mystr[5];
int incomingByte = 0;   // for incoming serial data
char b;
String str;




void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  //Read
//  mySerial.readBytes(mystr,5); //Read the serial data and store in var
//  mySerial.println(mystr); //Print data on Serial Monitor

  while(mySerial.available()) {
            char b = mySerial.read();
            Serial.print(b);
            delay(1);
     }

  incomingByte = mySerial.read();
  
  // say what you got:
  Serial.print("I received in raw: ");
  Serial.println(incomingByte);
  
  Serial.print("I received as DEC: ");
  Serial.println(incomingByte, DEC);
  
  Serial.print("I received as HEX: ");
  Serial.println(incomingByte, HEX);

  Serial.print("I received as OCT: ");
  Serial.println(incomingByte, OCT);

  Serial.print("I received as BIN: ");
  Serial.println(incomingByte, BIN);

  
  str=String(incomingByte);
  Serial.println(str);
//  str.toCharArray(b,2);   
  
  
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
