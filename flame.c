#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//Use test1@gmail.com -- Wubgq8kiNgYNC8hOZcYS991ll4n2

// Set these to run example.
#define FIREBASE_HOST "androidtest-8bb18.firebaseio.com" //This is the HOST ID from FIREBASE
#define FIREBASE_AUTH "jkUPqh4NBTPTm17J571NieSjaNbL2OhDwcPaZG56" //This is the AUTHORIZATION KEY taken from FIREBASE
//Change line with your WiFi router name and password
#define WIFI_SSID "wifi_ssid"
#define WIFI_PASSWORD "wifi_password"

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";


#define FLAME D0 // connect DO pin of sensor to this pin
#define ALARM D7 // pin 8 for Alarm


void setup() {
  Serial.begin(9600);

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

  pinMode(FLAME, INPUT);//define FLAME input pin
  pinMode(ALARM, OUTPUT);//define ALARM output pin
}

void loop() {
  int fire = digitalRead(FLAME);// read FLAME sensor
  yield();
  if( fire == HIGH)
  {
    //update boolean variable in FIREBASE
        Firebase.setBool("Wubgq8kiNgYNC8hOZcYS991ll4n2/flame", false);
        if (Firebase.failed()) {
            Serial.print("setting /false failed:");
            Serial.println(Firebase.error());
            return;
        }
    digitalWrite(ALARM,LOW);// set the buzzer ON
    Serial.println("Peace!!");
  }
  else{
    //update boolean variable in FIREBASE
        Firebase.setBool("Wubgq8kiNgYNC8hOZcYS991ll4n2/flame", true);
        if (Firebase.failed()) {
            Serial.print("setting /false failed:");
            Serial.println(Firebase.error());
            return;
        }
    digitalWrite(ALARM,HIGH); // Set the buzzer OFF
    Serial.println("Fire!!");
  }
  delay(200);
}
