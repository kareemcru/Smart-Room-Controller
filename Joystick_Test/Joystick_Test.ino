/*
 *  Project:      JoyStick Test
 *  Description:  Code for testing Joystick
 *  Authors:      Kareem Crum
 *  Date:         07-April-2021
 */
 
int value = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(7, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(A0);
  Serial.print("X:");
  Serial.print(value, DEC);

  value = analogRead(A1);
  Serial.print("| Y:");
  Serial.print(value, DEC);

  value = digitalRead(7);
  Serial.print("| Button:");
  Serial.println(value, DEC);

  delay(100);

}
