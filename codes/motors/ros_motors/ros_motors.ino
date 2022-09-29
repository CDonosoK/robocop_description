// Se importan las librerías
#include <ros.h>
#include <AFMotor.h>

// Se importan los mensajes
#include <geometry_msgs/Twist.h>

// Se crea un NOdeHandle
ros::NodeHandle nodo;

// Se instancian las variables
float linearVelocity;
float angularVelocity;
AF_DCMotor Motor1(3);
AF_DCMotor Motor2(4);

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void callback(const geometry_msgs::Twist& vel){
  linearVelocity = vel.linear.x;
  angularVelocity = vel.angular.z;
  if (linearVelocity>0.1){
    Serial.println("PARA ADELANTE");
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor1.setSpeed(200);
    Motor2.setSpeed(200);
  }
  if (linearVelocity<-0.1){
    Serial.println("PARA ATRAS");
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor1.setSpeed(200);
    Motor2.setSpeed(200);
  }
  if (angularVelocity>0.1){
    Serial.println("PARA IZQUIERDA");
    Motor1.run(BACKWARD);
    Motor2.run(FORWARD);
    Motor1.setSpeed(200);
    Motor2.setSpeed(200);
  }
  if (angularVelocity<-0.1){
    Serial.println("PARA DERECHA");
    Motor1.run(FORWARD);
    Motor2.run(BACKWARD);
    Motor1.setSpeed(200);
    Motor2.setSpeed(200);
  }
  if (abs(angularVelocity)<0.1 and abs(linearVelocity)<0.1){
    Serial.println("DETENIDO");
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
  }
}

ros::Subscriber<geometry_msgs::Twist> nodo_vel("cmd_vel", callback);


void setup(){
  // Se ajustan los motores
  nodo.initNode();
  nodo.subscribe(nodo_vel);

  Motor1.setSpeed(200);
  Motor2.setSpeed(200);
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);

  Serial.begin(57600);
}

void loop(){
  nodo.spinOnce();
  delay(10);
}
