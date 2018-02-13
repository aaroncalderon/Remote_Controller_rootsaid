// WIP
// Need to review comments and clarify the actual purpose of the code
//
// http://rztronics.com/control-brushless-motor-using-arduino/
#include <Servo.h>//Using servo library to control ESC
Servo esc; //Creating a servo class with name as esc

int xpotPin = A0;  // select analog pin 0 as “input pin” for X signal

int rangeT = 200;              // output range throttle
int thresholdT = rangeT / 100; // resting threshold
int centerT = 0;               // resting position is `zero`. this gives you a value `0 - n`

//
// process code without delay
// https://www.arduino.cc/en/tutorial/BlinkWithoutDelay
unsigned long previousMillis = 0;     // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to process code
// without delay

void setup()
{
  pinMode(xpotPin, INPUT); //
  Serial.begin(9600);

  // ESC
  Serial.println("Wait for it... ");
  esc.attach(3); //Specify the esc signal pin,Here as D3
  // this sectin will most likelly change once I test this
  // on the RC Plane.
  esc.writeMicroseconds(1000); //initialize the signal to 1000
}
void loop ()
{
  int xval;  // initialize variable

  xval = readAxis(xpotPin, rangeT, thresholdT, centerT );   // xval variable is the value read from signal pin 0

  // ESC
  int escVal; //Creating a variable val

  escVal = map(xval, 0, rangeT, 1000, 2000); //mapping val to minimum and maximum(Change if needed)
  esc.writeMicroseconds(escVal); //using val as the signal to esc

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    Serial.print(xval);
    Serial.print(",");
    Serial.println(escVal);
  }

}

// from mouse control
int readAxis(int thisAxis, int range, int threshold, int center) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

