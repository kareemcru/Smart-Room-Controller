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
#include<wemo.h>
#include <hue.h>


EthernetClient client;
bool status;

#include <OneButton.h>
OneButton button1 (2, true, true);
bool buttonState;
int i;
//check buttonState
int move;
int position;

void setup() {
  
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  move = 0;
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleClick1);
  button1.setClickTicks (250);

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
  
  }
  
void click1() {
  
  buttonState = !buttonState;
  Serial.println(buttonState);
    
  if(buttonState){
      setHue(2,true,HueBlue, 255, 255);
      switchON(i);
      Serial.printf("WEMO is on %i\n", i);
  }
  else{
    switchOFF(i);
  }
    if(!buttonState){
      setHue(2,false,0,0,0);
  }
  
}

void doubleClick1() {
  
  i++;
  if(i > 3) {
    i = 0;
  }
  Serial.printf("value of i is %i\n",i);
  
}  
