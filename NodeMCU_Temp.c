#include <SPI.h>
#include "RF24.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>

const char* ssid = "NameOfWiFi";
const char* password = "Password";

int ledPin = 5; // D1
WiFiServer server(80);  // open port 80 for web server

unsigned long t = 1;
unsigned long f = 1;
unsigned long got_code;



bool radioNumber = 0;

RF24 radio(15,2);   // Setup SPI pins on NodeMCU

byte addresses[][6] = {"1Node","2Node"};

bool role = 0;
long Temp1 = 0;
long Temp2 = 0;
long Hum =0;

void checkWeb()
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    
    // Wait until the client sends some data
    Serial.println("new client");
    while(!client.available()){
        delay(1);
    }
    
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
    
    // Match the request
    
    int value = LOW;
    if (request.indexOf("/Refresh") != -1)  {
        //digitalWrite(ledPin, HIGH);
        value = HIGH;
    }
    if (request.indexOf("/LED=OFF") != -1)  {
        digitalWrite(ledPin, LOW);
        value = LOW;
    }
    
    
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("&#x26F1 &nbsp; Faris's Home Weather Station :) &nbsp; &#x26F1<br><br>");
    client.print("Temperature: ");
    ////////////////////////////////// Sunny symbol:&#9728
    ////////////////////////////////// Raining symbol:&#9748
    ////////////////////////////////// Cloudy symbol:&#9925
    ////////////////////////////////// Freezing symbol:&#10052
    if(value == HIGH) {
        client.print(Temp1);
        client.print("&nbsp;&deg;F<br>");
        client.print("Temperature: ");
        client.print(Temp2);
        client.print("&nbsp;&deg;C<br>");
        client.print("Humidity: ");
        client.print(Hum);
        client.print("%");
        value == LOW;
    } else {
        client.print(Temp1);
        client.print("&nbsp;&deg;F<br>");
        client.print("Temperature: ");
        client.print(Temp2);
        client.print("&nbsp;&deg;C<br>");
        client.print("Humidity: ");
        client.print(Hum);
        client.print("%");
    }
    client.println("<br><br>");
    client.println("<a href=\"/Refresh\"\"><button>Refresh </button></a>");
    client.println("<a href=\"http://www.google.com/\"><button> Google </button></a><br />");
    client.println("</html>");
    
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");
}

//////////////////Temp Sensor///////////////////////////
#define DHTPIN 5
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
//////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);     // Begin serial with baud rate of 9600
    dht.begin();
    delay(10);
    
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    
    //Start the server
    server.begin();
    Serial.println("Server started");
    
    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    Serial.println("Starting radio...");
    radio.begin();
    
    radio.setPALevel(RF24_PA_MAX);    // Set communication power level
    radio.setDataRate(RF24_250KBPS);  // Set Data rate ti 250 KBPS
    
    // Open a writing and reading pipe on each radio, with opposite addresses
    if(radioNumber){
        radio.openWritingPipe(addresses[1]);
        radio.openReadingPipe(1,addresses[0]);
    }else{// for reference, incase you choose NodeMCU to be #1 instead of #0
        radio.openWritingPipe(addresses[0]);
        radio.openReadingPipe(1,addresses[1]);
    }
    
    // Start the radio listening for data
    radio.startListening();
}

void loop()
{
    // Wait a few seconds between measurements.
    delay(2000);
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t1 = dht.readTemperature();
    t = (unsigned long)t1;
    // Read temperature as Fahrenheit
    float f1 = dht.readTemperature(true);
    f = (unsigned long)f1;
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    
    if(radio.available()){
        // Variable for the received timestamp
        while (radio.available()) {                                   // While there is data ready
            radio.read( &got_code, sizeof(unsigned long) );             // Get the payload
        }
        
        radio.stopListening();                                     // First, stop listening so we can talk
        if(got_code == 8266320)
        {
            radio.write( &f, sizeof(unsigned long) );              // Send the final one back.                                            // Now, resume listening so we catch the next packets.
            Serial.print(F("Sent response "));
            Serial.println(f);  
        }
        radio.startListening();
    }
    Temp1 = f;  // Updating variables used in Html web page
    Temp2 = t;
    Hum = h;
    checkWeb();
    
} // Loop
