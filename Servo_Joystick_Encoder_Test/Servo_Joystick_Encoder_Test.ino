/*
 * Project: Servo
 * Description: Servo Fun
 * Author: Kareem Crum
 * Date: 30-MAR-2021
*/
#include <math.h>

const float A = 90;
const float B = 90;
float t;
float y;
float v;

#include <PWMServo.h>

PWMServo myServo;

#include <OneButton.h>

OneButton button1 (2, true, true);

// make button do stuff
bool buttonState;
int i;
int value = 0;

#include <Encoder.h>
Encoder myEnc(7,8);
int move;
int position;
int np;
int limit;
int remember;
 
void setup() {
  // put your setup code here, to run once:
  myServo.attach(5);
  button1.attachClick(click1);
  button1.setClickTicks(250);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  move = 0;
}

void loop() 
{
  // put your main code here, to run repeatedly:
    button1.tick();
    t= millis()/5000.0;
    v= 1;
    y=A*sin(2*M_PI*v*t)+B;
    myServo.read();
   Serial.printf("%f\n",y);
 
   if(buttonState){
    (myServo.write(180));
    myServo.read();
    Serial.printf("Servo is on %f\n",y);
  }
  
  if(!buttonState) {
    myServo.write(0);
    myServo.read();
    Serial.printf("Servo is on %f\n", !y);
  }
  
    value = analogRead(A0);
    Serial.print("X:");
    Serial.print(value, DEC);
  
    value = analogRead(A1);
    Serial.print("| Y:");
    Serial.print(value, DEC);
  
    value = digitalRead(2);
    Serial.print("| Button:");
    Serial.println(value, DEC);
  
    delay(100);
    
    position = myEnc.read();
    if(position<=0) {
      (position=0);
      myEnc.write(0); 
    }
    
    if(position>=95) {
      (position=95);
      myEnc.write(95); 
    }
 
    np = map(position,0,95,0,16);
    
    if(move !=np) {
      move = np;
    }
    
    remember = position;
    Serial.printf("position= %i\n", np);
    myEnc.write(remember);
        
}

void click1() {
  buttonState = !buttonState;
  Serial.println(buttonState);

}
