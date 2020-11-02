#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
 /*
 MQ-7 Gas Sensor
 */

int sensorValue;
int digitalValue;
int LedPin = D5;
int DangerPin = D2;

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
float ppm = analogRead(A0);
int digval = digitalRead(D0);

Serial.println(ppm, DEC); // prints the value read
Serial.println(digval, DEC);

yield();

if(ppm <= 70)//normal
{
digitalWrite(LedPin, LOW);
digitalWrite(DangerPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/cmonoxide", 1);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm >= 70 && ppm < 80)//bad
{
digitalWrite(LedPin, HIGH);
digitalWrite(DangerPin, LOW);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/cmonoxide", 2);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm >= 80 && ppm <100)//this is when it starts to cause side effects like naseau, fatigue, etc. -- can still be life threatening as well
{
digitalWrite(LedPin, HIGH);
digitalWrite(DangerPin, HIGH);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/cmonoxide", 3);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

yield();
if(ppm >= 100)//get out - this is when long exposure can be life threatening
{
digitalWrite(LedPin, HIGH);
digitalWrite(DangerPin, HIGH);
Firebase.setInt( "Wubgq8kiNgYNC8hOZcYS991ll4n2/cmonoxide", 4);
  if(Firebase.failed()){
    Serial.print("setting /int failed: ");
    Serial.println(Firebase.error());
    return;
  }
}

delay(1000); // wait 100ms for next reading

}
