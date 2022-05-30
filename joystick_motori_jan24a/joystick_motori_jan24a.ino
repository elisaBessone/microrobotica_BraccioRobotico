
#include <Servo.h> 

Servo myServoX;         // Oggetto ServoX
Servo myServoY;         // Oggetto ServoY
int servoX = 9;         // Pin Servo X
int servoY = 10;        // Pin Servo Y
int posX = 90;          // Posizione iniziale del ServoX
int posY = 90;          // Posizione iniziale del ServoY
int joyX = A0;          // Ingresso analogico del Joystick X
int joyY = A1;          // Ingresso analogico del Joystick Y
int joyValueX = 0;      // Valore letto X
int joyValueY = 0;      // Valore letto Y
int outValueX = 0;      // Valore mappato sull'out ServoX
int outValueY = 0;      // Valore mappato sull'out ServoY
int x = 5;

void setup() 
{
  myServoX.attach(servoX);  // ServoX collegato al Pin 9
  myServoY.attach(servoY);  // ServoY collegato al Pin 10
  myServoX.write(posX);     // Inizializzazione posizione del ServoX a 90°
  myServoY.write(posY);     // Inizializzazione posizione del ServoY a 90°
  delay(50);
  Serial.begin(9600);
} 

void loop() {
  // Lettura analogica del potenziometro
  joyValueX = analogRead(joyX);
  joyValueY = analogRead(joyY);
  // Mappatura del range, il potenziometro
  // viene letto sul piedino A0 (valore da 0 a 1023)
  // e convertito tramite l'istruzione map
  // ad un valore compreso tra 0° e 180°
  // cioè gli angoli che il Servo può coprire
  posX = map(joyValueX, 0, 1023, 0, 180);
  posY = map(joyValueY, 0, 1023, 0, 180);
  //Serial.println(posY);
  //myServoX.write(posX);
  //myServoY.write(posY);

  if(posX >= 177){
    myServoX.write(x);
    Serial.println("AVANTI");
    x+=5;
  }
  if(posX <= 2){
    myServoX.write(posX);
    Serial.println("INDIETRO");
    x+=5;
  }
  

  if(posY >= 177){
    myServoY.write(posY);
    Serial.println("DESTRA");
  }
  if(posY <= 2){
    myServoY.write(posY);
    Serial.println("SINISTRA");
  }

  
  delay(30);
}
