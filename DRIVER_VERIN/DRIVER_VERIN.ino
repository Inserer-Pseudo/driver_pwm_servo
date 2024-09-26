#define feedMinValue 50           // Valeur analog limite haute
#define feedMaxValue 585          // Valeur analog limite haute
#define minPWM 50                 // PWM min moteur

#define Kp 3                      // Gain du correcteur P
#define errorMargin 5            // MArger d'erreur

const int pwmInputPin = 3;       // PWM d'entrée (D3)
const int pwmOut1Pin = 9;        // PWM sens horaire (D9)
const int pwmOut2Pin = 10;       // PWM sens antihoraire (D10)
const int potPin = A0;           // Position (A0)

unsigned long pwmInputHighTime;  // Durée du signal haut
unsigned long pwmInputPeriod;    // Période
int targetPosition = 0;          // Position cible
int actualPosition = 0;          // Position réelle

int positionMin = 0;  // Limite basse en position
int positionMax = 255;  // Limite haute en position
int marginPot = 0; // augmenter pour reduire plagfe


void setup() {
  pinMode(pwmInputPin, INPUT);
  pinMode(pwmOut1Pin, OUTPUT);
  pinMode(pwmOut2Pin, OUTPUT);
  //Serial.begin(9600);
}

void loop() {

  // ----------------------- Gestion de la consigne -----------------------------

  pwmInputHighTime = pulseIn(pwmInputPin, HIGH);
  pwmInputPeriod = pwmInputHighTime + pulseIn(pwmInputPin, LOW);

  if (pwmInputPeriod > 0) {
    targetPosition = constrain(map(pwmInputHighTime, 1000, 2000, 0, 255),0,255);
  }

  // ----------------------- Gestion de la position -----------------------------

  actualPosition = analogRead(potPin);
  actualPosition = map(actualPosition, feedMinValue, feedMaxValue, 0, 255);

  int error = targetPosition - actualPosition;

  // ----------------------- Commande -------------------------------------------

  if (abs(error) <= errorMargin) {
    analogWrite(pwmOut1Pin, 0);
    analogWrite(pwmOut2Pin, 0);
  }
  else {
    if (error > 0) {
      int pwmValue = constrain(Kp * map(error, 0, 255, 0, 255), 0, 255);
      pwmValue = max(pwmValue, minPWM);
      analogWrite(pwmOut1Pin, pwmValue);
      analogWrite(pwmOut2Pin, 0);
    }
    else if (error < 0) {
      int pwmValue = constrain(Kp * map(-error, 0, 255, 0, 255), 0, 255);
      pwmValue = max(pwmValue, minPWM);
      analogWrite(pwmOut1Pin, 0);
      analogWrite(pwmOut2Pin, pwmValue);
    }
  }

  // ----------------------- Debogage -------------------------------------------

  // Serial.print("Position cible: ");
  // Serial.print(targetPosition);
  // Serial.print(" Position réelle: ");
  // Serial.print(actualPosition);
  // Serial.print(" Erreur: ");
  // Serial.println(error);

  delay(50);
}

