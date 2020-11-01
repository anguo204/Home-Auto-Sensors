#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "androidtest-8bb18.firebaseio.com"
#define FIREBASE_AUTH "jkUPqh4NBTPTm17J571NieSjaNbL2OhDwcPaZG56"
//Change line with your WiFi router name and password
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "iphone2898"

const char* ssid = "iPhone";
const char* password = "iphone2898";
const int DoorGpioPin = 5; // D1 of Node MCU
const int ledGpioPin =4;
int doorState=0;
int ledState=0;
#define closed 0
#define opened 1

void setup() {
  Serial.begin(115200);
  delay(10);
  // prepare GPIO2
  pinMode(DoorGpioPin, INPUT);
  pinMode(ledGpioPin, OUTPUT);
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
} //End of setup
/*============================================================================*/
void loop() {
    // While pin 12 is HIGH (not activated)
    yield(); // Do (almost) nothing -- yield to allow ESP8266 background functions
    if( digitalRead(DoorGpioPin)==HIGH && doorState==opened){ // Print button pressed message.
        Serial.println("Close");
        doorState=closed;
        ledState=1;//on
        digitalWrite(ledGpioPin, HIGH);

        //update boolean variable in FIREBASE
        Firebase.setBool("sensors/dooropen", false);
        if (Firebase.failed()) {
            Serial.print("setting /false failed:");
            Serial.println(Firebase.error());
            return;
        }
        Serial.println("doorOpen = false");
    }
    yield();
    if( digitalRead(DoorGpioPin)==LOW && doorState==closed){
        Serial.println("Open");
        doorState=opened;

        ledState=0; //off
        digitalWrite(ledGpioPin, LOW);

        //update boolean variable in FIREBASE
        Firebase.setBool("sensors/dooropen", true);
        if (Firebase.failed()) {
            Serial.print("setting /truth failed:");
            Serial.println(Firebase.error());
            return;
        }
        Serial.println("doorOpen = true");
    }
    yield(); // this is most important part of the code, as it tells the esp8266 keep running background wifi work,
            //without this, your code  will disconnect from wifi, after long run of code.
} //   END OF LOOP
