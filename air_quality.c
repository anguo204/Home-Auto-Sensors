#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "MQ135.h"

int sensorValue;
int digitalValue;
int LedPin = D5;
MQ135 gasSensor = MQ135(A0);
int ppm = gasSensor.getPPM();

#define FIREBASE_HOST "androidtest-8bb18.firebaseio.com" //HOST ID taken from FIREBASE
#define FIREBASE_AUTH "jkUPqh4NBTPTm17J571NieSjaNbL2OhDwcPaZG56" //AUTHORIZATION KEY taken from FIREBASE

#define WIFI_SSID "wifi_ssid"
#define WIFI_PASSWORD "wifi_password"

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

void setup()
{
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

yield();
if(ppm<=50){//good
digitalWrite(LedPin, HIGH);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/airQ", 0);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm > 50 && ppm <= 100)//moderate
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/airQ", 1);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm > 100 && ppm <= 150)//unhealthy for sensitive groups
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/airQ", 2);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm > 150 && ppm <= 200)//unhealthy
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/airQ", 3);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm > 200 && ppm <= 300)//very unhealthy
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/airQ", 4);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm > 300 && ppm <= 500)//hazardous
{
digitalWrite(LedPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/airQ", 5);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

delay(1000); // wait 1s for next reading

}
