/*
 *  Project:      OLED, BME, Neopixel Test
 *  Description:  Code for testing OLED, BME, Neopixels
 *  Authors:      Kareem Crum
 *  Date:         07-April-2021
 */
// Neopixel #include's
#include <Adafruit_NeoPixel.h>
#include <colors.h>
// BME and OLED #include's
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

//Neopixel variables
const int PIXELPIN = 17;
const int PIXELCOUNT = 3;
int butt;

//BME and OLED variables and stuff
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

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
  pixel.begin();
  pixel.show();
  Serial.begin(9600);

  bool status;
  status = bme.begin(0x76);
  
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


  
}

void loop() {
  for(butt = 0; butt <= 3; butt++) {
    pixel.setPixelColor(butt, 0, 0, 255);
    pixel.setBrightness(50);
  }
  pixel.show();
  tempC = bme.readTemperature();
  tempF = map(tempC,0,100,32,212);
  printValues();
  testdrawstyles();
}
void printValues() {

    Serial.printf("Temperature = %f\n", tempF);
    Serial.print(bme.readTemperature());
    Serial.println(" *F");

}

void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.printf("%f\n", tempF);
  
  display.display();
  delay(2000);
}
