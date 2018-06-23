#include <SPI.h>
#include "RF24.h"
#include <stdint.h>
#include <math.h>

uint16_t requestTemp=0;
bool radioNumber = 1;   // Set Tranceiver as #1 (Arduino)
unsigned long Temp_r = 0;
int DS_pin = 6;
int STCP_pin = 5;
int SHCP_pin = 4;
bool pin1,pin2,pin3,pin4,pin5,pin6,pin7,pin8,pin9,pin10,pin11,pin12;
uint8_t CorF = 1;
int rnd = 0;
double rndD = 0;

RF24 radio(9,10);   // Set Arduino communication pins between Transceiver and Arduino
byte addresses[][6] = {"1Node","2Node"};

void Get_Temp()      // Get Temperature
{
    if(radio.available())   // If a signal received from sender, read buffer
    {
        radio.read(&Temp_r, sizeof(unsigned long));
    }
    radio.stopListening();
    Serial.println(F("Now sending"));
    Serial.println(8266320);
    unsigned long start_time = 8266320;
    radio.write(&start_time, sizeof(unsigned long)); // Send request temperature signal
    radio.startListening();
}

void num(int input)   // Display *input* on 7-Segment LED
{
    pin1=LOW;
    pin2=LOW;
    pin3=LOW;
    pin4=LOW;
    pin5=LOW;
    pin6=LOW;
    pin7=LOW;
    pin8=LOW;
    pin9=LOW;
    pin10=LOW;
    pin11=LOW;
    pin12=LOW;
    if(input == 0)        // Display 0
    {
        pin3=HIGH;
        pin5=HIGH;
    }
    else if(input == 1)   // Display 1
    {
        pin1=HIGH;
        pin2=HIGH;
        pin3=HIGH;
        pin5=HIGH;
        pin10=HIGH;
        pin11=HIGH;
    }
    else if(input == 2)   // Display 2
    {
        pin10=HIGH;
        pin4=HIGH;
        pin3=HIGH;
    }
    else if(input == 3)   // Display 3
    {
        pin10=HIGH;
        pin1=HIGH;
        pin3=HIGH;
    }
    else if(input == 4)   // Display 4
    {
        pin11=HIGH;
        pin1=HIGH;
        pin2=HIGH;
        pin3=HIGH;
    }
    else if(input == 5)   // Display 5
    {
        pin7=HIGH;
        pin1=HIGH;
        pin3=HIGH;
    }
    else if(input == 6)   // Display 6
    {
        pin7=HIGH;
        pin3=HIGH;
    }
    else if(input == 7)   // Display 7
    {
        pin1=HIGH;
        pin2=HIGH;
        pin3=HIGH;
        pin5=HIGH;
        pin10=HIGH;
    }
    else if(input == 8)   // Display 8
    {
        pin3=HIGH;
    }
    else if(input == 9)   // Display 9
    {
        pin1=HIGH;
        pin3=HIGH;
    }
}

void setup() {
    Serial.begin (9600);          // setup serial with baudrate of 9600
    pinMode(DS_pin,OUTPUT);       // Setup pins for 8-bit shif register
    pinMode(STCP_pin,OUTPUT);
    pinMode(SHCP_pin,OUTPUT);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,INPUT);
    writereg();
    radio.begin();                // Start RF module
    
    radio.setPALevel(RF24_PA_MAX);    // Set communication power level
    radio.setDataRate(RF24_250KBPS);  // Set Data rate ti 250 KBPS
    
    // Open a writing and reading pipe on each radio, with opposite addresses
    if(radioNumber){
        radio.openWritingPipe(addresses[1]);
        radio.openReadingPipe(1,addresses[0]);
    }else{  // for reference, incase you choose Arduino to be #0 instead of #1
        radio.openWritingPipe(addresses[0]);
        radio.openReadingPipe(1,addresses[1]);
    }
    
    // Start the radio listening for data
    radio.startListening();
}

boolean registers[8];   // 8-bit Register's individual bits
// Mapping register's values to 7-Seg Pins

/////////////////////
//registers[0]=pin6;
//registers[1]=pin8;
//registers[2]=pin9;
//registers[3]=pin12;
//registers[4]=pin7;
//registers[5]=pin5;
//registers[6]=pin10;
//registers[7]=pin11;
/////////////////////

void reglow()           // Reset all pins in register to zero
{
    for(int i=0;i<8;i++)
    {
        registers[i] = LOW;
    }
}

void writereg() // After Register's bits have been set, update them on the Register
{
    digitalWrite(STCP_pin, LOW);      // Unlock writing to Reg
    for (int i = 7; i>=0; i--)
    {
        digitalWrite(SHCP_pin, LOW);
        digitalWrite(DS_pin, registers[i]);
        digitalWrite(SHCP_pin, HIGH);
    }
    digitalWrite(STCP_pin, HIGH);     // Lock writing to Reg
}
void Alloff()           // Turn all LED's off (7-Seg.)
{
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    registers[0]=LOW;
    registers[1]=LOW;
    registers[2]=LOW;
    registers[3]=LOW;
    registers[4]=HIGH;
    registers[5]=HIGH;
    registers[6]=HIGH;
    registers[7]=HIGH;
    writereg();
}
void D1(int digit)      // Write digit 1 of the 7-Seg
{
    reglow();
    num(digit);
    digitalWrite(7, pin1);
    digitalWrite(8, pin2);
    digitalWrite(2, pin3);
    digitalWrite(3, pin4);
    registers[3]=HIGH;
    registers[4]=pin7;
    registers[5]=pin5;
    registers[6]=pin10;
    registers[7]=pin11;
    writereg();
}

void D2(int digit)      // Write digit 2 of the 7-Seg
{
    reglow();
    num(digit);
    digitalWrite(7, pin1);
    digitalWrite(8, pin2);
    digitalWrite(2, pin3);
    digitalWrite(3, pin4);
    registers[2]=HIGH;
    registers[4]=pin7;
    registers[5]=pin5;
    registers[6]=pin10;
    registers[7]=pin11;
    writereg();
}
void D3(int digit)      // Write digit 3 of the 7-Seg
{
    reglow();
    num(digit);
    digitalWrite(7, pin1);
    digitalWrite(8, pin2);
    digitalWrite(2, pin3);
    digitalWrite(3, pin4);
    registers[1]=HIGH;
    registers[4]=pin7;
    registers[5]=pin5;
    registers[6]=pin10;
    registers[7]=pin11;
    writereg();
}
void D4(int digit)      // Write digit 4 of the 7-Seg (Celsius or Fahrenheit)
{
    reglow();
    if(digit == 0)          // Celsius
    {
        digitalWrite(7, LOW);   // LED E
        digitalWrite(8, LOW);   // LED D
        digitalWrite(2, HIGH);  // LED .
        digitalWrite(3, HIGH);  // LED C
        registers[0]=HIGH;      // LED Comm 4 (Anode for D4)
        registers[4]=HIGH;      // LED B
        registers[5]=HIGH;      // LED G
        writereg();
    }
    else                    // Fahrenheit
    {
        digitalWrite(7, LOW);   // LED E
        digitalWrite(8, HIGH);  // LED D
        digitalWrite(2, HIGH);  // LED .
        digitalWrite(3, HIGH);  // LED C
        registers[0]=HIGH;      // LED Comm 4 (Anode for D4)
        registers[4]=HIGH;      // LED B
        writereg();
    }
}

unsigned GetNumberOfDigits (unsigned i)     // Return how many digits in i
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}

void serialFlush(){         // Flush Serial
    while(Serial.available() > 0) {
        char t = Serial.read();
    }
}
void display_Digits(int x,int u)        // Display one digit plus temperature type
{
    Alloff();
    D1(x);
    delayMicroseconds(200);
    
    Alloff();
    D4(u);
    delayMicroseconds(200);
    Alloff();
}
void display_Digits(int x, int y, int u)   // Display two digits plus temperature type
{
    Alloff();
    D1(x);
    delayMicroseconds(200);
    Alloff();
    D2(y);
    delayMicroseconds(200);
    
    Alloff();
    D4(u);
    delayMicroseconds(200);
    Alloff();
}
void display_Digits(int x, int y, int z, int u) // Display three digits plus temperature type
{
    Alloff();
    D1(x);
    delayMicroseconds(200);
    Alloff();
    D2(y);
    delayMicroseconds(200);
    Alloff();
    D3(z);
    delayMicroseconds(200);
    
    Alloff();
    D4(u);
    delayMicroseconds(200);
    Alloff();
}
int conv(unsigned x)        // convert x to an integer
{
    if (x <= 32767)
        return static_cast<int>(x);
    
    if (x >= -32767)
        return static_cast<int>(x + 32767) - 32767;
    
    return x;
}
void loop()
{
    if(requestTemp==3000)   // Save power by requesting temperature in time intervals
    {
        Get_Temp();
        requestTemp=0;
    }
    if(analogRead(1) > 1000)// If A1 pin is reads "HIGH" switch Celsius <-> Fahrenheit
    {
        if(CorF==0)      // If previously was Celsius
            CorF++;      // switch to Fahrenheit
        else             // If previously was Fahrenheit
            CorF--;      // switch to Celsius
        delay(1000);     // give time for user to lift finger off the button
    }
    if(CorF == 0)        // if Celsius
    {
        Temp_r = round(((double(Temp_r-32))*0.5556));
    }
    else if(CorF == 1);  // if Fahrenheit (Default)
    
    if(Serial.available() > 0)      // Flush serial if not empty
        serialFlush();
    rnd = conv(Temp_r);     // convert received temperature into an integer
    if(rnd > 999 || rnd < 0)
    {
        Serial.println("Out of Range  0 <-> 999");
        delay(3000);
    }
    else
    {
        rndD = double(rnd);
        switch (GetNumberOfDigits(rnd)) // Depending on how many digits in the temperature, use different functions to display it.
        {
            case 1:
                display_Digits(rndD,CorF);
                break;
            case 2:
                display_Digits(floor(rndD/10),round((((rndD/10)-floor(rndD/10))*10)),CorF);
                break;
            case 3:
                display_Digits(floor(rndD/100),floor((((rndD/100)-floor(rndD/100))*10)),(((rndD/10)-floor(rndD/10))*10),CorF);
                break;
            default:
                break;
        }
    }
    requestTemp++;
}
