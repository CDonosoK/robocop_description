// Se importan las librerías
#include <ros.h>
#include <AFMotor.h>

// Se importan los mensajes
#include <geometry_msgs/Twist.h>

#define PI 3.1415926535897932384626433832795

// Variables globales de ROS
ros::NodeHandle nHandler;

// Se instancian las variables
float linearVelocity;
float angularVelocity;
float turnRightVel;
float turnLeftVel;

int forwardRight, forwardLeft, turnRight, turnLeft;
AF_DCMotor Motor1(3);
AF_DCMotor Motor2(4);

const byte encoder_rightA = 18;
const byte encoder_rightB = 19;
const byte encoder_leftA = 20;
const byte encoder_leftB = 21;

static long counter_right = 0;
static long counter_left = 0;
static long counter_right_old = 0;
static long counter_left_old = 0;

volatile bool fired_right, fired_left;
volatile bool up_right, up_left;

double x = 0;
double y = 0;
double theta = 0;

double rightVelocity = 0.0;
double leftVelocity = 0.0;

int long currentTime = 0;
int long previousTime = 0;

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void callback(const geometry_msgs::Twist& vel){
  linearVelocity = vel.linear.x;
  angularVelocity = vel.angular.z;

  int right_velocity = 150;
  int left_velocity = 150;
  if (linearVelocity>0.1){
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor1.setSpeed(right_velocity);
    Motor2.setSpeed(left_velocity);
  }
  if (linearVelocity<-0.1){
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor1.setSpeed(right_velocity);
    Motor2.setSpeed(left_velocity);
  }
  if (angularVelocity>0.1){
    Motor1.run(FORWARD);
    Motor2.run(BACKWARD);
    Motor1.setSpeed(right_velocity);
    Motor2.setSpeed(left_velocity);
  }
  if (angularVelocity<-0.1){
    Motor1.run(BACKWARD);
    Motor2.run(FORWARD);
    Motor1.setSpeed(right_velocity);
    Motor2.setSpeed(left_velocity);
  }
  if (abs(angularVelocity)<0.1 and abs(linearVelocity)<0.1){
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
  }

}

// Creación de los nodos de ROS
ros::Subscriber<geometry_msgs::Twist> nodo_vel("cmd_vel", callback);

// Variables para la odometría
double odom_x = 1.0;
double odom_y = 0.0;
double odom_thet = 1.57;
void setup(){
  // Se ajustan los nodos
  nHandler.getHardware()->setBaud(115200);
  nHandler.initNode();
  nHandler.subscribe(nodo_vel);

  // Se ajustan los motores
  Motor1.setSpeed(0);
  Motor2.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);


  Serial.begin(115200);
}

void loop(){ 
  nHandler.spinOnce();

}
