#include <AccelStepper.h>
#include <MultiStepper.h>

// LDR
#define LDR A0
#define button 7
// Stepper
#define dirPin 1
#define stepPin 3
#define enPin 5
#define motorInterfaceType 1

bool overrideState = false;
bool lastBtnState = false;
bool currBtnState = false;

bool gateOpen = false;

int value = 0;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(enPin, OUTPUT);

  currBtnState = digitalRead(button);
  
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(10000);
  digitalWrite(enPin, HIGH);
}

void loop() {
  Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();
  Serial.println("----------------------------------------------------------------------------");
  currBtnState = digitalRead(button);
  Serial.print("currBtnState ");
  Serial.println(currBtnState ? "pressed" : "not pressed");
  Serial.print("overrideState ");
  Serial.println(overrideState ? "enabled" : "disabled");
  Serial.print("gate is ");
  Serial.println(gateOpen ? "open" : "closed");
  
  if (!overrideState) {
    value = analogRead(LDR);
    Serial.print("Analog value: ");
    Serial.println(value);
  }

  if (currBtnState != lastBtnState) {
    lastBtnState = currBtnState;
    if(currBtnState == true){
      Serial.print("Button Pressed! overrideState ");
      overrideState = !overrideState;
      Serial.println(overrideState ? "enabled" : "disabled");
      // Toggle Gate
      if (gateOpen) {
        digitalWrite(enPin, LOW);
        stepper.moveTo(0);
        stepper.runToPosition();
        digitalWrite(enPin, HIGH);
        gateOpen = !gateOpen;
      }
      else if (!gateOpen) {
        digitalWrite(enPin, LOW);
        stepper.moveTo(3000);
        stepper.runToPosition();
        digitalWrite(enPin, HIGH);
        gateOpen = !gateOpen;
      }
    }
  }

  if (!overrideState) {
    if (value < 200) {
      // Donker
      if (gateOpen) {
        digitalWrite(enPin, LOW);
        stepper.moveTo(0);
        stepper.runToPosition();
        digitalWrite(enPin, HIGH);
        gateOpen = !gateOpen;
      }
    }
    else if (value > 200) {
      // Licht
      if (!gateOpen) {
        digitalWrite(enPin, LOW);
        stepper.moveTo(3000);
        stepper.runToPosition();
        digitalWrite(enPin, HIGH);
        gateOpen = !gateOpen;
      }
    }
  }
  
  delay(500);
}
