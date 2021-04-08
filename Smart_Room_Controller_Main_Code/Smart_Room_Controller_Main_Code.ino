/*
 *  Project:      JoyStick Test
 *  Description:  Code for testing Joystick
 *  Authors:      Kareem Crum
 *  Date:         07-April-2021
 */

// RTC (Real Time Clock) #include's
#include <TimeLib.h>

// Ethernet and Wemo and Hue #include's
#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>
#include <hue.h>
EthernetClient client;
 
// Neopixel #include's
#include <Adafruit_NeoPixel.h>
#include <colors.h>

// BME and OLED #include's
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

// Servo and Joystick and Encoder #include's
#include <math.h>
#include <PWMServo.h>
#include <OneButton.h>
#include <Encoder.h>
Encoder myEnc(7,8);
PWMServo myServo;
OneButton button1 (2, true, true);

//Ethernet and Wemo and Hue variables
//Servo and Joystick and Encoder variables
float y;
bool buttonState;
int w;
bool buttonState2;
int z;
int value = 0;
int move;
int position;
int np;
int limit;
int remember;
int myHue [] = {1, 2, 3, 4};

//Neopixel variables
const int PIXELPIN = 17;
const int PIXELCOUNT = 4;
int butt;

//BME and OLED variables and stuff
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

char N = 164;

//setting up BME
Adafruit_BME280 bme;
float tempC;

//converting values
float tempF;

   Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);
  /* Argument 1 = PIXELCOUNT
   * Argument 2 = GPIO PIXELPIN
   * Argument 3 = NEO_GRB+NEO_KHZ800
   * 
   */
   
void setup() {
  // put your setup code here, to run once:
  pixel.begin();
  pixel.show();
  myServo.attach(5);
  button1.attachClick(click1);
  button1.attachLongPressStart(longPressStart1);
  button1.setClickTicks(250);
  button1.setPressTicks(2000);
  
  pinMode(A1, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
  move = 0;  
  w = 1;
    
  bool status;
  status = bme.begin(0x76);

  // void Setup for RTC
  setSyncProvider(getTeensy3Time);

  Serial.begin(115200);
  while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(100);
  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }

  //void setup for BME and OLED
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...


   // Draw 'stylized' characters

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

//  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
  
  //void setup for Ethernet
  Ethernet.begin(mac);
  delay(200);          //ensure Serial Monitor is up and running           
  printIP();
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());

}

void loop() {
  // put your main code here, to run repeatedly:
  button1.tick();
  y = myServo.read();
  myServo.read();
  rtc();

  value = analogRead(A1);
  Serial.printf("| Y: %i\n", value);

  js();
  
  position = myEnc.read();
  if(position<=0) {
     (position=0);
     myEnc.write(0); 
  }
    
  if(position>=95) {
      (position=95);
      myEnc.write(95); 
  }
 
  np = map(position,0,95,0,3);
     
  if(move !=np) {
      move = np;
  }
  
  remember = position;
  Serial.printf("position= %i\n", np);
  myEnc.write(remember);
  
  tempC = bme.readTemperature();
  tempF = map(tempC,0,100,32,212);
  printValues();
  testdrawstyles();

  pixel.clear();
  
  pixel.fill(black, 0, 4);
  pixel.fill(purple, 0, w);
  pixel.setBrightness(55);

  if(tempF >72){
    pixel.fill(black, 0, 4);
    pixel.fill(red, 0, w);
    pixel.setBrightness(55);
    Serial.println("red");
  }
  
  else if(tempF < 69){
    pixel.fill(black, 0, 4);
    pixel.fill(blue, 0, w);
    pixel.setBrightness(55);
    Serial.println("blue");
  }
        
  pixel.show();
  
}

void printValues() {
  
    Serial.printf("Temperature = %f *F\n", tempF);
    Serial.printf("Time = %i:%i:%i\n", hour(), minute(), second() );

}

void testdrawstyles(void) {
  
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.printf("%f\n", tempF); 


  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.printf("%i:%i:%i\n", hour(), minute(), second());

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.printf("on %i\n", buttonState);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.printf("%i selected\n", w);
  
  display.display();
}

void click1() {
  
  w++;
  if(w > 4) {
    w = 1;
  }

  z = w *45;
  (myServo.write(z));  
  Serial.printf("z = %i\n",z);

}

void longPressStart1() {
  
  buttonState = !buttonState;
  Serial.println(buttonState);
  
} 

void js(){
  
  if(buttonState){
    if(value == 1023){
      switchON(w-1);
    }
    if(value <= 20){
      switchOFF(w-1);
    }
   Serial.printf("WEMO is on %i\n", w);
  }
  if(!buttonState){
    if(value == 1023){
       setHue(myHue[w-1],true,HueBlue, 255, 255);
    }
    if(value <= 20){
        setHue(myHue[w-1],false,0,0,0);
    }
   Serial.printf("Hue is on %i\n", w);
  }
}

void printIP() {

  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);
  
}

void rtc(){
  
 if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t != 0) {
      Teensy3Clock.set(t); // set the RTC
      setTime(t);
    }
  }
  digitalClockDisplay();  
}

void digitalClockDisplay() {
  
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}

void printDigits(int digits){
  
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
