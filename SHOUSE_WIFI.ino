 /*
  Web client

 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
//////////////////////////////////////////////////////

// Arduino ID also used as an user to login to website
char MyID[] = "bbbb";                       //id of the station
char Host[] = "HOST: 81.110.5.170"; 

int sensorPin1 = 0;       
int sensorPin2 = 1;   
const int ledPin =  13; 

//pinMode(ledPin, OUTPUT);
                        
///////////////////////////////////////////////////////////////////////////////////////////

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(81,110,5,177);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Configure pins for Adafruit ATWINC1500 Breakout
WiFi.setPins(8,7,4);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
 
  Serial.println("Connected to wifi");
  printWiFiStatus();
    Serial.println("--------------------------------");
    Serial.println("");


}
void connect() {
 //getting the voltage reading from the temperature sensor
 int reading1 = analogRead(sensorPin1);  
 int reading2 = analogRead(sensorPin2);
 
 // converting that reading to voltage
 float voltage1 = reading1 * 5.0;
 voltage1 /= 1024.0; 
  float voltage2 = reading2 * 5.0;
 voltage2 /= 1024.0; 
 
 // print out the voltage
 //Serial.print(voltage1); Serial.println(" volts1");
 //Serial.print(voltage2); Serial.println(" volts2");
 
 // now print out the temperature
 float tempC1 = (voltage1 - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
 float tempC2 = (voltage2 - 0.5) * 100 ;                                              //to degrees ((voltage - 500mV) times 100)
 //Serial.print(tempC1); Serial.println(" *C");
 //Serial.print(tempC2); Serial.println(" *C");



  if (client.connect(server, 80)) {
    client.print("GET /sh/recive_data.php");
    client.print("?un=");
    client.print(MyID);                               //Post The Serial Number
    client.print("&ti=");
    client.print(tempC2);                           //Post The Temperature
    client.print("&to=");
    client.print(tempC1);
    client.println(" HTTP/1.0");
    client.println(Host);
    client.println();

  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  //status request
  if (client.connect(server, 80)) {
    client.print("GET /sh/status_request.php");
    client.print("?user=");
    client.print(MyID);   
    client.println(" HTTP/1.0");
    client.println(Host);
    client.println();

  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }



}



void loop() {
if (client.available()) {
    char c = client.read();
    Serial.print(c);
    
digitalWrite(ledPin, atoi(c)); 


    // Serial.println("");
  }

  // while(true);
   
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();
    delay(1000);
    connect();
  }
  
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
