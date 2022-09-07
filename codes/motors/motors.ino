// Variables from control
String data, joystick_izq, joystick_der, buttons;
int separ1, separ2, separ_joyIzq, separ_joyDer, separ_bot1, separ_bot2;
float joy_izqX, joy_izqY, joy_derX, joy_derY;
int bot1, bot2, bot3, bot4;

float vel_forward, vel_backward, vel_turn;

// Variables from motors
int PWMA = 5, PWMB = 4, DA = 0, DB = 2;
void setup() {
  Serial.begin(115200);
  pinMode(PWMA, OUTPUT); 
  pinMode(PWMB, OUTPUT); 
  pinMode(DA, OUTPUT); 
  pinMode(DB, OUTPUT);
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  if (Serial.available()){
    // Obtain data from python
    data = Serial.readStringUntil('\n');
    separ1 = data.indexOf("|");
    separ2 = data.indexOf("|",separ1+1);

    // Obtain data for left joystick
    joystick_izq = data.substring(1, separ1-1);
    separ_joyIzq = joystick_izq.indexOf(",");
    joy_izqX = (joystick_izq.substring(0,separ_joyIzq)).toFloat();
    joy_izqY = (joystick_izq.substring(separ_joyIzq+1, joystick_izq.length())).toFloat();

    // Obtain data for right joystick
    joystick_der = data.substring(separ1+2, separ2-1);
    separ_joyDer = joystick_der.indexOf(",");
    joy_derX = (joystick_der.substring(0,separ_joyDer)).toFloat();
    joy_derY = (joystick_der.substring(separ_joyDer+1, joystick_der.length())).toFloat();

    // Obtain data for buttons.
    buttons = data.substring(separ2+2, data.length()-1);
    separ_bot1 = buttons.indexOf(",");
    bot1 = (buttons.substring(0,separ_bot1)).toInt();
    separ_bot2 = buttons.indexOf(",", separ_bot1+1);
    bot2 = (buttons.substring(separ_bot1+1,separ_bot2)).toInt();
    separ_bot1 = buttons.indexOf(",", separ_bot2+1);
    bot3 = (buttons.substring(separ_bot2+1,separ_bot1)).toInt();
    separ_bot2 = buttons.indexOf(",", separ_bot1+1);
    bot4 = (buttons.substring(separ_bot1+1,buttons.length())).toInt();

    // Move motors
    motorControl(joy_izqX, joy_izqY, joy_derX, joy_derY);

  }
}

//Function to move motors 
void motorControl(float joy_izqX, float joy_izqY, float joy_derX, float joy_derY){
  // Stay
  if (abs(joy_izqY) < 0.1 and abs(joy_derX) < 0.1){
    analogWrite(PWMA, LOW);
    analogWrite(DA, LOW);
    analogWrite(PWMB, LOW);
    analogWrite(DB, LOW);
    Serial.println("Stay");
  }
  
  // Forward
  else if (joy_izqY > 0.1){
    vel_forward = map_float(joy_izqY, 0.1, 1.00, 0, 255);
    Serial.println("Forward");
    analogWrite(PWMA, vel_forward);
    analogWrite(DA, LOW);
    analogWrite(PWMB, vel_forward);
    analogWrite(DB, LOW);
  }

  // Backward
  else if (joy_izqY < -0.1){
    vel_backward = map_float(abs(joy_izqY), 0.1, 1.00, 0, 255);
    Serial.println("Backward");
    Serial.println(vel_backward);
    analogWrite(PWMA, vel_backward);
    analogWrite(DA, vel_backward);
    analogWrite(PWMB, vel_backward);
    analogWrite(DB, vel_backward);
  }

  // Turn left on point
  else if (abs(joy_izqY) <= 0.1 and joy_derX >= 0.1){
    vel_turn = map_float(joy_derX, 0, 1.00, 0, 255);
    Serial.println("Turn left on point");
    analogWrite(PWMA, vel_turn);
    analogWrite(DA, vel_turn);
    analogWrite(PWMB, vel_turn);
    analogWrite(DB, LOW);
  }

  // Turn right on point
  else if (abs(joy_izqY) <= 0.1 and joy_derX <= -0.1){
    vel_turn = map_float(abs(joy_derX), 0.1, 1.00, 0, 255);
    Serial.println("Turn right on point");
    analogWrite(PWMA, vel_turn);
    analogWrite(DA, LOW);
    analogWrite(PWMB, vel_turn);
    analogWrite(DB, vel_turn);
  }
}
