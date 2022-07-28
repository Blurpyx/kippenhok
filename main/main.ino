#include <AccelStepper.h>
#include <MultiStepper.h>

// LDR
#define LDR A0
#define button 7
// Stepper
#define dirPin 1
#define stepPin 11
#define enPin 5
#define motorInterfaceType 1
#define isOpen 2
#define isClosed 3

#define stepsOpen 2000

bool overrideState = false;

bool lastBtnState = false;
bool currBtnState = false;

bool btnOpen = false;
bool btnClosed = false;

int value = 0;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(enPin, OUTPUT);

  currBtnState = digitalRead(button);
  
  stepper.setMaxSpeed(60);
  stepper.setAcceleration(20);
  digitalWrite(enPin, LOW);
}

void loop() {
  Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();
  Serial.println("----------------------------------------------------------------------------");
  currBtnState = digitalRead(button);
  btnOpen = digitalRead(isOpen);
  btnClosed = digitalRead(isClosed);
  Serial.println("Buttons:");
  Serial.print("  - Master button: ");
  Serial.println(currBtnState ? "pressed" : "not pressed");
  Serial.print("  - End-switch open: ");
  Serial.println(btnOpen ? "pressed" : "not pressed");
  Serial.print("  - End-switch closed: ");
  Serial.println(currBtnState ? "pressed" : "not pressed");
  Serial.print("overrideState ");
  Serial.println(overrideState ? "active" : "disabled");

  if (!overrideState) {
    value = analogRead(LDR);
    Serial.print("Analog value: ");
    Serial.println(value);

    // Als motor nog geen bestemming heeft
    if (stepper.distanceToGo() == 0) {
      // Veronderstel dat poort gesloten is bij opstart
      if (btnClosed && !btnOpen) {
        // Poort gesloten
        if (value <= 1010) {
          // Licht
          stepper.moveTo(stepsOpen);
        } else {
          // Donker
          stepper.moveTo(-stepsOpen);
        }
      } else if (btnOpen && !btnClosed) {
        // Poort open
        if (value <= 1010) {
          // Licht
          stepper.moveTo(stepsOpen);
        } else {
          // Donker
          stepper.moveTo(-stepsOpen);
        }
      }
    }
  }
  
  if (stepper.currentPosition() == stepsOpen) {
    stepper.setCurrentPosition(0);
    stepper.moveTo(0);
  }

  if (currBtnState != lastBtnState) {
    lastBtnState = currBtnState;
    if(currBtnState == true){
      Serial.print("Button Pressed! overrideState ");
      overrideState = !overrideState;
      Serial.println(overrideState ? "enabled" : "disabled");
      // Toggle Gate
      if (btnClosed) {
        // If closed then open
        stepper.moveTo(stepsOpen);
      }
      else {
        // If not closed then close
        stepper.moveTo(-stepsOpen);
      }
    }
  }
  while (stepper.distanceToGo() != 0) {
    stepper.run(); 
    // delay(500);
  }
}
