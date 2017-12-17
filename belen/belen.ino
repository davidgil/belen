//www.elegoo.com
//2016.12.12

/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.

 */

#include <Stepper.h>
#include <Servo.h>
#include <AccelStepper.h>

class Panadero
{
  Servo servo;              // the servo
  int pos;              // current servo position 
  int increment;        // increment to move for each interval
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
  unsigned long lastPause; // last pause of position
  bool paused = false;

  int posicionInicial = 160;
  int posicionFinal = 20;

  int tiempoPausa = 2000;
 
  public: Panadero(int interval)
  {
    pos = 20;
    updateInterval = interval;
    increment = 1;
  }
  
  void Attach(int pin)
  {
    servo.attach(pin);
  }
  
  void Detach()
  {
    servo.detach();
  }
  
  void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      
      if (!paused && ( (pos == posicionInicial)  || (pos == posicionFinal) ) )
      {
        lastPause = millis();
        paused = true;
      }

      if(paused && (millis() - lastPause) < tiempoPausa) {
        return;
      } else if (paused) {
        paused = false;
      }
      
      pos += increment;
      servo.write(pos);
      if ((pos > posicionInicial) || (pos < posicionFinal)) // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }
};




const int stepsPerRevolution = 400;  // change this to fit the number of steps per revolution

// initialize the stepper library on pins 8 through 11:
//Stepper stepperBurro(stepsPerRevolution, 8, 10, 9, 11);
//Stepper stepperPerro(stepsPerRevolution, 4, 6, 5, 7);

#define FULLSTEP 4
#define HALFSTEP 8
AccelStepper stepperPerro(HALFSTEP, 4, 6, 5, 7);

AccelStepper stepperBurro(HALFSTEP, 8, 10, 9, 11);


// motor del panadero
Panadero panadero(25);

void setup() {
  panadero.Attach(13);
  stepperBurro.setSpeed(40);
  //stepperPerro.setSpeed(50);

  stepperBurro.setMaxSpeed(500);
   stepperBurro.setSpeed(100);

  stepperPerro.setMaxSpeed(1000);
  stepperPerro.setSpeed(500); 
}


void loop() {
  panadero.Update();
  //stepperBurro.step(1);
  //stepperPerro.step(1);
    stepperBurro.runSpeed();

  stepperPerro.runSpeed();

  // Movimiento del burro
  /*
  myStepper.setSpeed(5);
  myStepper.step(500);

  myStepper.setSpeed(10);
  myStepper.step(500);

  myStepper.setSpeed(15);
  myStepper.step(500);

  myStepper.setSpeed(20);
  myStepper.step(500);

  myStepper.setSpeed(25);
  myStepper.step(500);
  
  myStepper.setSpeed(30);
  myStepper.step(500);

  myStepper.setSpeed(25);
  myStepper.step(500);

  myStepper.setSpeed(20);
  myStepper.step(500);

  myStepper.setSpeed(15);
  myStepper.step(500);

  myStepper.setSpeed(10);
  myStepper.step(500);

  myStepper.setSpeed(5);
  myStepper.step(500);
  */

 //delay(5000);
 
}


