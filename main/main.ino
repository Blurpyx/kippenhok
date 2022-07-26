#include <AccelStepper.h>
#include <MultiStepper.h>

// LDR
#define LDR A0
#define button 7
// Stepper
#define dirPin 11
#define stepPin 12
#define enPin 5

#define isOpen 2
#define isClosed 3

bool overrideState = false;
bool lastBtnState = false;
bool currBtnState = false;

bool opened = false;

int value = 0;
int stSp = 80;
int mxSp = stSp * 10;

#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(enPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(isOpen), CloseUp, RISING);
  attachInterrupt(digitalPinToInterrupt(isClosed), CloseCl, RISING);
  
  /*attachInterrupt(digitalPinToInterrupt(isOpen), StepUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(isClosed), StepCl, FALLING);*/

  currBtnState = digitalRead(button);
  
  stepper.setMaxSpeed(mxSp);
  stepper.setSpeed(stSp);
  digitalWrite(enPin, LOW);
}

void CloseUp() {
  if(!opened && digitalRead(isOpen)) {
    opened = true;
    stepper.stop();
  }
}

void CloseCl() {
  if(opened && digitalRead(isClosed)) {
    opened = false;
    stepper.stop();
  }
}
void OpenGate() {
  // Open gate
  stepper.setSpeed(stSp);
  while(!opened) stepper.runSpeed();
}
void CloseGate() {
  // Close gate
  stepper.setSpeed(-stSp);
  while(opened) stepper.runSpeed();
}
/*
void StepUp() {
  stepper.stop();
  stepper.setSpeed(stSp);
  while(!opened) {
    stepper.runSpeed();
  }
}

void StepCl() {
  stepper.stop();
  stepper.setSpeed(-stSp);
  while(opened) {
    stepper.runSpeed();
  }
}
*/
void loop() {
  Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();
  Serial.println("----------------------------------------------------------------------------");
  currBtnState = digitalRead(button);
  Serial.print("currBtnState ");
  Serial.println(currBtnState ? "pressed" : "not pressed");
  Serial.print("overrideState ");
  Serial.println(overrideState ? "enabled" : "disabled");
  
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
      // Open gate
      if(!opened) OpenGate();
      // Close gate
      if(opened) CloseGate();
      
    }
  }

  if (!overrideState) {
    if (!opened && (value < 300)) {
      // Open gate
      while(!opened) stepper.setSpeed(stSp); stepper.runSpeed(); 
    }
    else if (value > 300) {
      // Close gate
      while(opened) stepper.setSpeed(-stSp); stepper.runSpeed();
    }
  }

  //delay(500);
}
