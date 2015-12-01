#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor[2];

class QuadEncoder {
  public:
    //long long pos;
    long pos; //switch with top statement??
    bool reversed; // set
    char pin[2];
    QuadEncoder() {
      reset();
    }
    void attach(int pin1, int pin2) {
      pinMode(pin1, INPUT);
      pinMode(pin2, INPUT);
      pin[0] = pin1;
      pin[1] = pin2;
      pin_state[0] = digitalRead(pin[0]) == HIGH;
      pin_state[1] = digitalRead(pin[1]) == HIGH;
    }
    long long read() {
      update();
      return pos;
    }
    void reset() {
      pin[0] = 0;
      pin[1] = 0;
      pos = 0;
      velocity = 1; // velocity can either be 1 or -1
      reversed = false;
      pin_state[0] = 0;
      pin_state[1] = 0;
    }
  private:
    void update() {
      if (pin[0] == 0 || pin[1] == 0)
        return;
      // FSA : reg :: 00 01 11 10
      //     : rev :: 00 10 11 01
      char new_state[2] = {
        digitalRead(pin[0]) == HIGH,
        digitalRead(pin[1]) == HIGH
      };
      char delta_state[2] = {
        new_state[0] != pin_state[0],
        new_state[1] != pin_state[1]
      };
      if (delta_state[0] && delta_state[1]) {
        pos += velocity * 2 * (reversed ? -1 : 1);
      } else if (delta_state[1]) {
        velocity = (new_state[0] == new_state[1]) ? -1 : 1;
        pos += velocity * (reversed ? -1 : 1);
      } else if (delta_state[0]) {
        velocity = (new_state[0] == new_state[1]) ? 1 : -1;
        pos += velocity * (reversed ? -1 : 1);
      }
      pin_state[0] = new_state[0];
      pin_state[1] = new_state[1];
    }
    char pin_state[2];
    long long velocity;  // estimated
};

  QuadEncoder encoder1;
  QuadEncoder encoder2;
  QuadEncoder encoder3;
  QuadEncoder encoder4;
  
void setup() {
  motor[0] = AFMS.getMotor(1);
  AFMS.begin();
  // put your setup code here, to run once:
  encoder1.attach(2, 3); //pin nums
  encoder2.attach(4, 5);
  encoder3.attach(6, 7);
  encoder4.attach(8, 11);
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  long value1 = encoder1.read();
  long value2 = encoder2.read();
  long value3 = encoder3.read();
  long value4 = encoder4.read();
  Serial.print("ENC 1: ");
  Serial.print(value1);
  Serial.print(" | ENC 2: ");
  Serial.print(value2);
  Serial.print(" | ENC 3: ");
  Serial.print(value3);
  Serial.print(" | ENC 4: ");
  Serial.println(value4);
}
