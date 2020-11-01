#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "MQ135.h"


int sensorValue;
int digitalValue;
int LedPin = D5;
MQ135 gasSensor = MQ135(A0);
int ppm = gasSensor.getPPM();

#define FIREBASE_HOST "androidtest-8bb18.firebaseio.com"
#define FIREBASE_AUTH "jkUPqh4NBTPTm17J571NieSjaNbL2OhDwcPaZG56"

#define WIFI_SSID "AviRox"
#define WIFI_PASSWORD "test"

const char* ssid = "AviRox";
const char* password = "test";

void setup()
{
  //pinMode(D5, OUTPUT);
  pinMode( A0, INPUT);

  pinMode(LedPin, OUTPUT);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //begin a connection to wifi
  WiFi.begin(ssid, password);
  //keep trying to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
  //establish connection to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

Serial.begin(9600); // sets the serial port to 9600

}


void loop()

{

float rzero = gasSensor.getRZero();
float ppm = gasSensor.getPPM();



Serial.println(ppm, DEC); // prints the value read
//Serial.println(rzero, DEC);

yield();
if(ppm<100){//good
digitalWrite(LedPin, HIGH);
Firebase.setInt( "sensors/airquality", 0);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm >= 100 && ppm < 300)//not good
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "sensors/airquality", 1);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm >= 300 && ppm < 500)//bad
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "sensors/airquality", 2);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm >= 500)//get out
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "sensors/airquality", 3);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

delay(1000); // wait 100ms for next reading

}
