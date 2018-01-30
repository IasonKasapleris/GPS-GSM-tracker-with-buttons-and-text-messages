// Kwdikas 1: To GPS an lavei tin leksi "Location" apo kinito tilefwno tote tou epistrefei tis syntetagmenes tou simeiou sto opoio vrisketai to GPS. 
// Epipleon an patisoume kapoio koumpi apothikeuetai ekeini ti stigmi i topothesia tou GPS. Stis 7 apothikeuseis mas stelnontai epi topou oles oi syntetagmenes pou apithikeutikan kata ti diadromi 
// Author: Iason-Stylianos Kasapleris
// A.M.: 03112051

#include <AltSoftSerial.h> //Include all relevant libraries, emulate an additional serial port, allowing you to communicate with another serial device
#include <TinyGPS++.h> //this library provides compact and easy-to-use methods for extracting position, date, time, altitude, speed, and course from consumer GPS devices.  
#include <GSM.h>  //this library enables an Arduino board to do most of the operations you can do with a GSM phone: place and receive voice calls, send and receive SMS, and connect to the internet over a GPRS network.
#include <stdio.h>
#define PINNUMBER "" // PIN Number for the SIM - leave blank unless your SIM has a pin, this is inserted between ""


static const uint32_t GPSBaud = 9600; //Baud rate for communication with the GPS, Adafruit GPS = 9600, your GPS may well be 4800, check the spec

TinyGPSPlus gps; // The TinyGPS++ object for interfacing with the GPS

AltSoftSerial ss; // The serial connection object to the GPS device

String yourPassword = "Location"; // Put the password here between the ""
String password; // Temporary variable used for comparison of passwords

GSM gsmAccess; // Initialise the library instances
GSM_SMS sms;

char senderNumber[20]="00306947050972"; // Array to hold the number a SMS is retreived from
// Array which holds the coordinates and the color of the button pressed.

int led1 = 13;
int led2 = 12;

const int GREEN_BUTTON = 4;
const int YELLOW_BUTTON  = 7; 
int yellow_val  = 0;
int green_val   = 0;
String txtmsg = "";

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  
  ss.begin(GPSBaud); // begin the GPS serial connection
 
  Serial.begin(9600); // begin Serial communication with the computer at 9600 baud rate

  Serial.println("Hello"); // Print to the computer
  Serial.println("Initialising...");
 
  boolean notConnected = true; // connection state

  while(notConnected) // until it connects
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY) // if it succeeds connecting
      notConnected = false; // connected
    else
    {
      Serial.println("Not connected"); // print to the computer
      delay(1000); //delay
    }
  }

  Serial.println("GSM initialized");      // print to the computer
  Serial.println("Waiting for messages");
}


void loop()
{
  while (ss.available() > 0) //while there is stuff in the buffer
    if (gps.encode(ss.read())) //if it can successfully decode it, do it. Else try again when more charachters are in the buffer
 
  if (sms.available()) // if a text has been recieved
  {
    Serial.println("Message received from:"); // print to the computer

    sms.remoteNumber(senderNumber, 20); // assign the sender number to the "senderNumber" variable
    Serial.println(senderNumber); // print the sender number to the computer
    
    password = ""; // flush the temporary variable

    char c;
    
    while(c=sms.read())
    {
      password += c; // append the sms to the "password" variable
    }

    Serial.println(password); // print the contents of the sms
    Serial.println("\nEND OF MESSAGE"); // print to the computer

    sms.flush(); // delete message from modem buffer
    Serial.println("MESSAGE DELETED"); // print to the computer

    if (password == yourPassword) // if the sms contains the correct password
    {
      Serial.println("\nPASSWORD VALID"); // print to the computer
      digitalWrite(led2, LOW);
      digitalWrite(led1, HIGH);
      sms.beginSMS(senderNumber); // begin an sms to the sender number
      sms.print(gps.location.lat(), 6); // append the lat to the sms
      sms.print(","); // append a comma
      sms.print(gps.location.lng(), 6); // append the lon to the sms
      sms.endSMS(); //send the sms
    }
    else {
      Serial.println("\nPASSWORD NOT VALID"); // print to the computer
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
    }
  }
  
  green_val = digitalRead(GREEN_BUTTON);
  if (green_val == HIGH) {
    delay(500);
   //sms.print(gps.location.lat(), 6); // append the lat to the sms
   //sms.print(","); // append a comma
   //sms.print(gps.location.lng(), 6); // append the lon to the sms
   //sms.print("\n");
    txtmsg += String(gps.location.lat(), 6);
    txtmsg += ",";
    txtmsg += String(gps.location.lng(), 6);
    txtmsg += "g\n";
    Serial.println(txtmsg);
    if (txtmsg.length() > 120) {
        sms.beginSMS(senderNumber);
        sms.print(txtmsg);
        sms.endSMS();
        txtmsg = "";
    }
  }
    Serial.println("outside the buttons");

  yellow_val = digitalRead(YELLOW_BUTTON); // read input value and store it
  if (yellow_val == HIGH) {
    delay(500);
   //sms.print(gps.location.lat(), 6); // append the lat to the sms
   //sms.print(","); // append a comma
   //sms.print(gps.location.lng(), 6); // append the lon to the sms
   //sms.print("\n");
    txtmsg += String(gps.location.lat(), 6);
    txtmsg += ",";
    txtmsg += String(gps.location.lng(), 6);
    txtmsg += "y\n";
    Serial.println(txtmsg);
    if (txtmsg.length() > 120) {
        sms.beginSMS(senderNumber);
        sms.print(txtmsg);
        sms.endSMS();
        txtmsg = "";
        }
  }
  
  delay(1000); // delay
}

