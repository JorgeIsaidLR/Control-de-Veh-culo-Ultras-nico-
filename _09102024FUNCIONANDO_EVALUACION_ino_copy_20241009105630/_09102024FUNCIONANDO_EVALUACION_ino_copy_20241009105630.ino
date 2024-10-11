#include <Robojax_L298N_DC_motor.h>

// Pines del sensor ultrasónico
int trigPin = 16; // Disparador (Trigger)
int echoPin = 17; // Eco (Echo)
long duration, cm;

#define LEDUltrasonico 19
#define LEDAdDer 23

// Motor 1 settings (lado izquierdo)
#define IN1 33
#define IN2 25
#define ENA 27 // Pin PWM

// Motor 2 settings (lado derecho)
#define IN3 14
#define IN4 26
#define ENB 32 // Pin PWM

const int CCW = 2; // Sentido antihorario
const int CW  = 1; // Sentido horario

#define motor1 1 // Motor izquierdo
#define motor2 2 // Motor derecho

Robojax_L298N_DC_motor motor(IN1, IN2, ENA, IN3, IN4, ENB, true);

// Función para mover el carro hacia adelante
void MoverCarroAdelante(int velocidad) {
  motor.rotate(motor1, velocidad, CCW); // Motor izquierdo
  motor.rotate(motor2, velocidad, CW);  // Motor derecho
}

// Función para girar el carro sobre su propio eje (25 grados aprox)
void GirarCarro25Grados() {
  motor.rotate(motor1, 45, CW);  // Motor izquierdo hacia adelante
  motor.rotate(motor2, 45, CW);  // Motor derecho hacia atrás
  delay(250);  // Ajusta este valor para lograr un giro de ~25 grados
  DetenerCarro();
}

// Función para detener el carro
void DetenerCarro() {
  motor.brake(motor1);
  motor.brake(motor2);
}

// Función para medir la distancia con el sensor ultrasónico
void medirDistancia() {
  // Limpia el trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Envía un pulso al trigPin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lee el tiempo que tarda el pulso en regresar
  duration = pulseIn(echoPin, HIGH);

  // Calcula la distancia en centímetros
  cm = duration * 0.0343 / 2;

  // Imprime la distancia medida
  Serial.print("Distancia: ");
  Serial.print(cm);
  Serial.println(" cm");
}

void setup() {
  Serial.begin(115200);
  motor.begin();

  // Configura los pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDUltrasonico, OUTPUT);
 
}

void loop() {
  medirDistancia();  // Mide la distancia en cada iteración del loop

  if (cm <= 35) {    // Si la distancia es 15 cm o menor
    DetenerCarro();
    digitalWrite(LEDUltrasonico, HIGH);
    Serial.println("¡Obstáculo detectado! Girando...");
    
    // Gira hasta que ya no detecte un obstáculo
    while (cm <= 35) {
      GirarCarro25Grados();
      medirDistancia();  // Actualiza la distancia
    }

    digitalWrite(LEDUltrasonico, LOW);  // Apaga el LED cuando ya no hay obstáculos
  } else {
    MoverCarroAdelante(50);  // Continúa avanzando si no hay obstáculos
  }

  delay(100);  // Pequeña pausa antes de la siguiente medición
}

