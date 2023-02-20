#include <Wire.h> //Calling Library For Communication between I2C/TWI Devices, in this case RGB Sensor
#include "Adafruit_TCS34725.h" //Calling RGB Sensor Model Specific Library
#include <Servo.h> //Calling Library for Servo Motor

Servo myservo; //Declaration of Servo Variable
/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
int t = 500, g = 1, motorpin=11, servopin=9;

// Initialise RGB Sensor Variable With Specific int time and gain values 
Adafruit_TCS34725 tcs = Adafruit_TCS34725(t,g);

void setup(void) {
  pinMode(motorpin,OUTPUT); //Setting motorpin as output pin
  Serial.begin(9600); //Starting Serial Monitor at Baud Rate 9600
  myservo.attach(servopin); //Attaching Servo Variable to Servopin

  if (tcs.begin()) { //Checking if RGB Sensor is Connected
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop(void) {
  int test=0,counter=0,colour,current_angle,a=0;
  while (a==0){
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c); //Getting Raw Data from the RGB Sensor

    //Printing raw Red, Green, Blue values
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.println(" ");
    delay(300); //Delay Between Readings

    //Checking Colour
    if (r>b and r>g)
    {
      colour=1;
      //If r is greatest value then Detected Colour is Red

    } else if (b>g and b>r) {
      colour=2;
      //If b is greatest value then Detected Colour is Blue
      
    } else if (g>b and g>r) {
      if (r>b){
        colour=3;
        //If b is greatest value and red is greater than blue then Detected Colour is Yellow

      } else {
        colour=4;
        //If b is greatest value then Detected Colour is Green

      }
    }
    if (colour==test){ //Running a counter for number of times the same colour is output, for increased accuracy
      counter++;
      Serial.print(counter);
      Serial.print("\n");
    } else {
      counter=0; //counter is reset if colour changes
      Serial.print(counter);
      Serial.print("\n");
    }
    test=colour;
    if (counter>=5){ //Once counter reaches 5 it will start moving the motors based on detected colour
      if (colour==1){ //If colour is red
        Serial.print("Red\n");
        myservo.write(30); //Setting Servo angle 30
        delay(1000); //1 second delay to let the servo reach the required angle
        digitalWrite(motorpin,HIGH); //Extending Pneumatic Piston
        delay(3000); //3 second delay between extension and retraction
        digitalWrite(motorpin,LOW); //Retracting Pneumatic Piston
        delay(4000); //4 second delay allow the next ball to fall and be detected
      } else if (colour==2) { //Repeat for Blue
        Serial.print("Blue\n");
        myservo.write(70);
        delay(1000);
        digitalWrite(motorpin,HIGH);
        delay(3000);
        digitalWrite(motorpin,LOW);
        delay(4000);
      } else if (colour==3) { //Repeat for Yellow
        Serial.print("Yellow\n");
        myservo.write(110);
        delay(1000);
        digitalWrite(motorpin,HIGH);
        delay(3000);
        digitalWrite(motorpin,LOW);
        delay(4000);
      } else if (colour==4) { //Repeat for Green
        Serial.print("Green\n");
        myservo.write(150);
        delay(1000);
        digitalWrite(motorpin,HIGH);
        delay(3000);
        digitalWrite(motorpin,LOW);
        delay(4000);
      }
    }
  }
}

