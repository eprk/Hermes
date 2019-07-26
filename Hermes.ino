/* WHAT'S NEW
 *  2018/10/18 Hermes 1.1 Added the new adaptive threshold.
 *  2018/10/23 Hermes 1.2 Changed the calibration loops
*/

// ---CONSTANTS
const int R = 11;
const int G = 12;
const int B = 13;
const int DTR = 2;
const int pinIN = A0;

unsigned long idleTime = 200; // the time (in ms) the system stays idle after triggering a DTR

double val,thr;
double thrRatio = 0.5;// MUST be between 0 and 1. The ratio between the threshold-white distance divided by the black-white distance (thr-whiteval/(blackval-whiteval))
// Conservative value (best one) = 0.5. old value was 0.75. High value: 0.999 (IT WORKS)

void setup() {
  // put your setup code here, to run once:
  unsigned long calibTime = 5000; // calibration time in ms
  unsigned long startTime,i;
  const int writeBtn = 3; // pin that is written HIGH for the button
  const int readBtn = 4; // pin that is read to check if button is pressed
  double whiteval,blackval;
  
  pinMode(DTR,OUTPUT);
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(writeBtn,OUTPUT);
  
  digitalWrite(DTR,LOW);
  digitalWrite(R,LOW);
  digitalWrite(G,LOW);
  digitalWrite(B,LOW);

  // Serial Monitor Check begin(uncomment included code for checking)
//  Serial.begin(9600);
//  Serial.println("<Arduino is ready>");
  // Serial Monitor Check end (uncomment included code for checking)

  // BLUE LED ON. CALIBRATION CAN START
  digitalWrite(B,HIGH);

  digitalWrite(writeBtn,HIGH);
  while(digitalRead(readBtn) == LOW){}
  digitalWrite(writeBtn,LOW);
  // --- BEGIN WHITE CALIBRATION
  whiteval = 5000; // largest possible value, in mV
  startTime = millis();
  while (millis()<startTime+calibTime){
  val = analogRead(pinIN)*4.9; // 4.9 is a conversion factor from 0-255 bits to 0-5000 mV
  if (val < whiteval){
    whiteval = val;
  }
  // Red LED blinks at 4 Hz (blinking purple)
  if ((millis()-startTime)%250<125){ // Red LED turns on
    digitalWrite(R,HIGH);
  }
  else { // Red LED turns off
    digitalWrite(R,LOW);
  }
  }
  // --- END WHITE CALIBRATION

  // RED AND BLUE LEDs ON. BLACK CALIBRATION CAN START
  digitalWrite(R,HIGH);
  digitalWrite(writeBtn,HIGH);
  while(digitalRead(readBtn) == LOW){}
  digitalWrite(writeBtn,LOW);
  // --- BEGIN BLACK CALIBRATION
  blackval = 5000;
  startTime = millis();
  while(millis() < startTime+calibTime){
  val = analogRead(pinIN)*4.9;
  if (val<blackval){
    blackval = val;
  }
  // Red LED blinks at 4 Hz (blinking purple)
  if ((millis()-startTime)%250<125){ // Red LED turns on
    digitalWrite(R,HIGH);
  }
  else { // Red LED turns off
    digitalWrite(R,LOW);
  }
  }
  // --- END BLACK CALIBRATION

  // The threshold is finally set:
  thr = thrRatio*(blackval-whiteval)+whiteval;
  // The ratio is a number between 0 and 1. It's the distance of the threshold from the white value, normalized over the distance between the white and black values.

  // The setup is over. Only the green LED stays on
  digitalWrite(R,LOW);
  digitalWrite(B,LOW);
  digitalWrite(G,HIGH);

  // Serial Monitor Check begin(uncomment included code for checking)
  Serial.print("whiteval = ");
  Serial.println(whiteval);
  Serial.print("blackval = ");
  Serial.println(blackval);
  Serial.print("thr = ");
  Serial.println(thr);
  Serial.end();
  // Serial Monitor Check end (uncomment included code for checking)

}


void loop() {
  // put your main code here, to run repeatedly:
  // val will store the pin value
  val = analogRead(pinIN)*4.9;
  if (val < thr)
  {
    digitalWrite(DTR,HIGH);
    digitalWrite(R,HIGH);
    delay(10);
    digitalWrite(DTR,LOW);
    delay(40);
    digitalWrite(R,LOW);
    delay(idleTime-50);
  }
}
