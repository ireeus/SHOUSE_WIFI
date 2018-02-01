//#include <SparkFunESP8266Client.h>
#include <SparkFunESP8266Server.h>
#include <SparkFunESP8266WiFi.h>

#include <SoftwareSerial.h>

//#include <SPI.h>
//#include <Ethernet.h>
//#include <WiFiClient.h>
///#include <HttpClient.h>
//#include <Bridge.h>

//////////////////////////////////////////////////////

// Arduino ID also used as an user to login to website
char MyID[] = "aaaa";

// PHP server with mysql data insertion
char server[] = "82.28.186.219/";
char host[] = "Host: 82.28.186.219/";

//WiFi network .
const char mySSID[] = "";
const char myPSK[] = "";

//////////////////////////////////////////////////////

ESP8266Client client = ESP8266Client(80);
float tempC1;                           //Temperature sensor 1
float tempC2;                           //Temperature sensor 1
int tempPin1 = A0;                      //Temperature sensor 1
int tempPin2 = A1;
int ledPin = 13;
int fan1 = 5;
const char* SSID = mySSID;


void connect() {
  if (client.connect(server, 80)) {
    Serial.print("Make a HTTP request ... ");
    client.print("GET /sh/recive_data.php");
    client.print("?un=");
    client.print(MyID);                               //Post The Serial Number
    client.print("&ti=");
    client.print(tempC1);                           //Post The Temperature
    client.print("&to=");
    client.print(tempC2);
    client.println(" HTTP/1.0");
    client.println("HOST: 82.28.186.219");
    client.println();
    Serial.println("ok");
  }
  else {                                              //If Arduino Can't Connect To The Server
    Serial.println("TRANSMITION FAILED");             //Serial Debug Output To Confirm Connection Failure
  }
}

void setup ()
{
  Serial.begin(9600);
  // Serial Monitor is used to control the demo and view
  // debug information.
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(fan1, OUTPUT);
  if (esp8266.begin()) // Initialize the ESP8266 and check it's return status
    Serial.println("ESP8266 ready to go!"); // Communication and setup successful
  else
    Serial.println("Unable to communicate with the ESP8266 :(");
  int retVal;

  // Wait for connection

  retVal = esp8266.connect(mySSID, myPSK);
  if (retVal < 0)
  {
    Serial.print(F("Error connecting: "));
    Serial.println(retVal);
  } else {
    Serial.print(F("Connected "));
    Serial.println(retVal);
    // Print your WiFi shield's IP address
    IPAddress ip = esp8266.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }
  //signal
  unsigned long before = millis();
  //  int32_t rssi = getRSSI(SSID);
  unsigned long after = millis();
  // Serial.print("Signal strength: ");
  //  Serial.print(rssi);
  //Serial.println("dBm");

  //Serial.print("Took ");
  //Serial.print(after - before);
  // Serial.println("ms");
  //delay(1000);

  connect();
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }                   //Routine To Post To The Server
  int retVal;
  retVal = esp8266.connect(mySSID, myPSK);

  tempC1 = analogRead(tempPin1);
  Serial.print("------------------------------------");
  Serial.println();
  Serial.print("sens_1: ");
  Serial.print(tempC1, 1);
  tempC1 = (5.0 * tempC1 * 100.0) / 1024.0;
  Serial.print("    |");

  tempC2 = analogRead(tempPin2);
  Serial.print("  ");
  Serial.print(" sens_2: ");
  Serial.print(tempC2, 1);
  tempC2 = (5.0 * tempC2 * 100.0) / 1024.0;

  Serial.println();
  Serial.print("Temp_1: ");
  Serial.print(tempC1, 1); // one decimal place
  Serial.print("°C  |   Temp_2: ");
  Serial.print(tempC2, 1);
  Serial.print("°C");
  //Serial.print("Unable to connect");
  // printing data posted to the server
  Serial.println();
  Serial.print(server);
  Serial.print("recive_data.php?");
  Serial.print("un=");                         //Post The Serial Number
  Serial.print(MyID);
  Serial.print("&ti=");                           //Post The Temperature
  Serial.print(tempC1);
  Serial.print("&to=");                           //Post The Humidty
  Serial.print(tempC2);
  Serial.println();

  if (retVal > 0)
  { Serial.print("Connected");
    Serial.println();
  } else {
    Serial.print("Disconnected");
    Serial.println();
  }

  if (tempC1 == 0 || tempC2 == 0) // if 1 or 2 is >25C

  {
    Serial.print("too cold");
  }
  Serial.println();

  if (tempC1 > 27 || tempC2 > 23) // if 1 or 2 is >25C
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(fan1, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(fan1, HIGH);
  }

  //  HttpClient client;
  connect();
  delay(2000);
}
