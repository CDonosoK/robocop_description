// Se importan las librerías
#include <ros.h>
#include <AFMotor.h>

// Se importan los mensajes
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/UInt16.h>
#include <ros/time.h>

#define PI 3.1415926535897932384626433832795

// Variables globales de ROS
ros::NodeHandle nHandler;
nav_msgs::Odometry odom_msgs;
geometry_msgs::TransformStamped tOdom;
geometry_msgs::TransformStamped tTF;
tf::TransformBroadcaster broadcaster;

// Se instancian las variables
float linearVelocity;
float angularVelocity;
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


float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void callback(const geometry_msgs::Twist& vel){
  linearVelocity = vel.linear.x;
  angularVelocity = vel.angular.z;

  int right_velocity = 100;
  int left_velocity = 100;
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

void read_right(){
  if (digitalRead(encoder_rightA)){
    up_right = !digitalRead(encoder_rightB);
  }
  else{
    up_right = digitalRead(encoder_rightB);
  }
  fired_right = true;
}
void read_left(){
  if (digitalRead(encoder_leftA)){
    up_left = !digitalRead(encoder_leftB);
  }
  else{
    up_left = digitalRead(encoder_leftB);
  }
  fired_left = true;
}

void setup(){
  // Se ajustan los nodos
  nHandler.getHardware()->setBaud(115200);
  nHandler.initNode();
  nHandler.subscribe(nodo_vel);
  broadcaster.init(nHandler);

  // Se ajustan los motores
  Motor1.setSpeed(100);
  Motor2.setSpeed(100);
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);

  // Se ajustan los encoders
  pinMode(encoder_rightA, INPUT);
  pinMode(encoder_rightB, INPUT);
  pinMode(encoder_leftA, INPUT);
  pinMode(encoder_leftB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder_rightA), read_right, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_leftA), read_left, CHANGE);

  // Se ajusta la velocidad
  int left_velocity = 100;
  int right_velocity = 100;

  Serial.begin(115200);
}

void print_values(){
  Serial.print(counter_right);  
  Serial.print(" - ");
  Serial.println(counter_left);
}

void update_counter(){
  // Encoder derecho 
  if (fired_right){
    if (up_right){
      counter_right++;      
    }
    else{
      counter_right--;      
    }
    fired_right = false;
  }

  // Encoder izquierdo
  if (fired_left){
    if (up_left){
      counter_left++;      
    }
    else{
      counter_left--;      
    }
    fired_left = false;
  }  
}

void loop(){ 
  update_counter();
  double dx = 0.2;
  double dtheta = 0.18;
  x += cos(theta)*dx*0.1;
  y += sin(theta)*dx*0.1;
  theta += dtheta*0.1;
  if (theta >PI){
    theta =-PI;
  }

  tOdom.header.frame_id = "/odom";
  tOdom.child_frame_id = "/base_link";
  tOdom.transform.translation.x = 0.0;
  tOdom.transform.translation.y = 0.0;
  tOdom.transform.rotation = tf::createQuaternionFromYaw(0.0);
  tOdom.header.stamp = nHandler.now();
  broadcaster.sendTransform(tOdom); 


  nHandler.spinOnce();
}
