/*
Controlador Arduino de mi Belen, este año compuesto por:
  - Un burro que da vueltas alrededor de una piedra de moler trigo. Controlado por un motor stepper
  - Un panadero que saca el pan del horno. Controlado por un motor servo
  - Un perro que persigue a un gato. Controlado por un motor stepper.
*/
#include <Servo.h>
#include <AccelStepper.h>

class Pausador {
  bool estaPausado = false;
  unsigned long lastPause;
  int tiempoPausa;
  int cicloPausa;

  public: Pausador(int parameterCicloPausa, int parameterTiempoPausa) {
    lastPause = millis();   
    tiempoPausa = parameterTiempoPausa;
    cicloPausa = parameterCicloPausa;
  }

  public: bool isPausado() { 
    long currentTime = millis();   
    if (estaPausado) {
      if (haPasadoElTiempoDePausa()) {
        lastPause = currentTime;
        estaPausado = false;
        return false;
      } 
      return true;
    } else {
      if (hayQuePausar()) {
        estaPausado = true;
        lastPause = currentTime;
        return true;
      } 
      return false;
    }
  }

  bool haPasadoElTiempoDePausa() {
    return (millis() - lastPause) > tiempoPausa;
  }

  bool hayQuePausar() {
    return (millis() - lastPause) > cicloPausa;
  }
};

/**
 * Controlador del Panadero. 
 * Un panadero que saca el pan del horno. 
 * Controlado por un motor servo.
 */
class Panadero {
  const int POSICION_INICIAL = 160;
  const int POSICION_FINAL = 20;
  const int TIEMPO_PAUSA = 2000;
  
  Servo servo;              // the servo
  int pos;                  // current servo position 
  int increment;            // increment to move for each interval
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
  unsigned long lastPause;  // last pause of position
  bool paused = false;
 
  public: 
    Panadero(int interval) {
      pos = 20;
      updateInterval = interval;
      increment = 1;
    }
    
    void Attach(int pin) {
      servo.attach(pin);
    }
    
    void Detach() {
      servo.detach();
    }
    
    void update() {
      if((millis() - lastUpdate) > updateInterval) {
        lastUpdate = millis();
        
        if (!paused && ( (pos == POSICION_INICIAL)  || (pos == POSICION_FINAL) ) ) {
          lastPause = millis();
          paused = true;
        }
  
        if(paused && (millis() - lastPause) < TIEMPO_PAUSA) {
          return;
        } else if (paused) {
          paused = false;
        }
        
        pos += increment;
        servo.write(pos);
        if ((pos > POSICION_INICIAL) || (pos < POSICION_FINAL)) {
          // reverse direction
          increment = -increment;
        }
      }
    }
};

/**
 * Clase de control del burro.
 * Un burro que da vueltas alrededor de una piedra de moler trigo. 
 * Controlado por un motor stepper
 */
class Burro {
  AccelStepper stepperBurro;
  Pausador pausador;

  public: Burro(): stepperBurro(AccelStepper::HALF4WIRE, 8, 10, 9, 11), pausador(15000, 4000) {
    stepperBurro.setMaxSpeed(500);
    stepperBurro.setSpeed(200);
  }

  void update() {
   if (pausador.isPausado()) {
     stepperBurro.stop();
   } else {
     stepperBurro.setSpeed(200);
     stepperBurro.runSpeed();  //TODO: testar con run
   }
  }
}; 

/**
 * Un perro que persigue a un gato. 
 * Controlado por un motor stepper.
 */
class PerroGato {
  public: 
    PerroGato(): stepperPerro(AccelStepper::HALF4WIRE, 4, 6, 5, 7), pausador(8000, 3000) {
      stepperPerro.setMaxSpeed(1000);
      stepperPerro.setSpeed(500);
    }

    void update() {
      if (pausador.isPausado()) {
        stepperPerro.stop();
      } else {
        stepperPerro.setSpeed(500);

        stepperPerro.runSpeed();  //TODO: testar con run
      }
    }

  private: 
    AccelStepper stepperPerro;
    Pausador pausador;
};

/**
 * PROGRAMA PRINCIPAL
 */
Panadero panadero(25);
Burro burro;
PerroGato perroGato;

void setup() {
  panadero.Attach(13);
}

void loop() {
  panadero.update();
  burro.update();
  perroGato.update();
}
