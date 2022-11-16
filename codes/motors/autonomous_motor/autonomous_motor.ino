// Se importan las librerías
#include <ros.h>
#include <AFMotor.h>

// Se importan los mensajes
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/Int64MultiArray.h>
#include <ros/time.h>

#define PI 3.1415926535897932384626433832795

// Variables globales de ROS
ros::NodeHandle nHandler;
nav_msgs::Odometry odom_msgs;
std_msgs::String position_msgs;

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

double rightVelocity = 0.0;
double leftVelocity = 0.0;

int long currentTime = 0;
int long previousTime = 0;

// Variables para la odometría
double odom_x = 0.0;
double odom_y = 0.0;
double odom_theta = 0.0;

String string_position = "";


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
ros::Publisher posePublisher("pose_topic", &position_msgs);

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
  nHandler.advertise(posePublisher);

  // Se ajustan los motores
  Motor1.setSpeed(0);
  Motor2.setSpeed(0);
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
  int left_velocity = 120;
  int right_velocity = 130;

  // Se setea el time
  currentTime = millis();
  previousTime = currentTime;

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

// Right ticks per meter
int rightTick[] = {
  7550
};

// Left ticks per meter
int leftTick[] = {
  1000
};

// Right ticks per half turn
int rightTurn[] = {
};

// Left ticks per half turn
int leftTurn[] = {
};

double mean(int type){
  double sum = 0;
  double len = 0.0;
  if (type == 0){
    len = sizeof(rightTick)/sizeof(rightTick[0]);
    for (int i = 0; i < len; i++){
      sum += rightTick[i];
    }
  }
  else if (type == 1){
    len = sizeof(leftTick)/sizeof(leftTick[0]);
    for (int i = 0; i < len; i++){
      sum += leftTick[i];
    }
  }
  else if (type == 2){
    len = sizeof(rightTurn)/sizeof(rightTurn[0]);
    for (int i = 0; i < len; i++){
      sum += rightTurn[i];
    }
  }
  else if (type == 3){
    len = sizeof(leftTurn)/sizeof(leftTurn[0]);
    for (int i = 0; i < len; i++){
      sum += leftTurn[i];
    }
  }
  return sum / len;
}

double meanRight = mean(0);
double meanLeft = mean(1);
double meanTurnRight = mean(2);
double meanTurnLeft = mean(3);

double previousR_tick = 0.0;
double previousL_tick = 0.0;

void loop(){ 
  currentTime = millis();
  update_counter();

  if (currentTime - previousTime >= 100){

    double deltaR_ticks = counter_right - previousR_tick;
    double r_distance = deltaR_ticks / meanRight;

    double deltaL_ticks = counter_left - previousL_tick;
    double l_distance = deltaL_ticks / meanLeft;

    double delta_distance = (r_distance + l_distance) / 2.0;

    odom_theta += (r_distance - l_distance) / 0.215;
    
    if (odom_theta > 2.0*PI) odom_theta -= 2.0*PI;
    if (odom_theta < 0.0) odom_theta += 2.0*PI;

    odom_x += delta_distance * cos(odom_theta);
    odom_y += delta_distance * sin(odom_theta);

    previousR_tick = counter_right;
    previousL_tick = counter_left;
    previousTime = currentTime;
  }

  string_position = "";
  string_position += String(odom_x);
  string_position += "|";
  string_position += String(odom_y);
  string_position += "|";
  string_position += String(odom_theta);
  position_msgs.data = string_position.c_str();
  posePublisher.publish(&position_msgs);

  nHandler.spinOnce();

}
