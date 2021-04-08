/*
 * Project: EthernetTest
 * Description: Ensure Ethernet Port wired correctly
 * Author: Brian Rashap
 * Date: 29-MAR-2021
 */

 /*
  * Ethernet Port Wiring
  * 3.3V to Teensy 3.3V
  * GND to Teensy GND
  * MISO to Teensy DI (Pin 12)
  * MOSI to Teensy DO (Pin 11)
  * SCLK to Teensy SCK (Pin 13)
  * SCNn to Teensy Pin 10 (for Chip Select)
  * RSTn to Teensy Pin 9
  * INTn and NC (No Connection) not connected
  */

#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>
#include <hue.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <colors.h>

Encoder myEnc(7,8);
EthernetClient client;
bool status;

#include <OneButton.h>
OneButton button1 (2, true, true);
bool buttonState;
int w;
int myHue [] = {1, 2, 3, 4};

//check buttonState
int move;
int position;
int np;
int limit;
int remember;
int value = 0;

//Neopixel variables
 const int PIXELPIN = 17;
 const int PIXELCOUNT = 4;

    Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);
  /* Argument 1 = PIXELCOUNT
   * Argument 2 = GPIO PIXELPIN
   * Argument 3 = NEO_GRB+NEO_KHZ800
   * 
   */
    
void setup() {
  
  pixel.begin();
  pixel.show();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  pinMode(A1, INPUT);

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  move = 0;
  button1.attachClick(click1);
  button1.attachLongPressStart(longPressStart1);
  button1.setClickTicks (250);
  button1.setPressTicks(2000);

  //Open Serial Communication and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Starting Program");

  //Start ethernet connection
  status = Ethernet.begin(mac);
  if (!status) {
    Serial.printf("failed to configure Ethernet using DHCP \n");
    //no point in continueing 
    while(1);
    }
  //print your local IP address
  Serial.print("My IP address:");
  for (byte thisbyte = 0; thisbyte < 4; thisbyte++) {
    //print value of each byte of the IP address
    Serial.print(Ethernet.localIP()[thisbyte], DEC);
    if (thisbyte < 3) Serial.print(".");
    }
  Serial.println();
}

void loop() { 
  
  button1.tick();

    
  value = analogRead(A1);
  Serial.printf("| Y: %i\n", value);

  js();
  
  delay(100);
  
  }
  
void click1() {
  
  w++;
  if(w >3){
    w = 0;
  } 
}

void longPressStart1() {
  
  buttonState = !buttonState;
  Serial.println(buttonState);
  
} 

void js(){
  if(buttonState){
    if(value == 1023){
      switchON(w);
    }
    if(value <= 20){
      switchOFF(w);
    }
   Serial.printf("WEMO is on %i\n", w);
  }
  if(!buttonState){
    if(value == 1023){
       setHue(myHue[w],true,HueBlue, 255, 255);
    }
    if(value <= 20){
        setHue(myHue[w],false,0,0,0);
    }
   Serial.printf("Hue is on %i\n", w);
  }
}
