//-----------------------------------------------------
// difine the pins
//-----------------------------------------------------
#include<Servo.h>
Servo myservo;

#define SEN_PIN_1 3
#define SEN_PIN_2 2

#define IN1 10
#define IN2 9
#define IN3 6
#define IN4 5

#define MOTOR_RIGHT 0
#define MOTOR_LEFT 1
// #define BLUETOOTH 0 

#define echo 11
#define trigger 12
#define servo A3
int set = 20;   // Minimum Distance for Ultrasonic sensor to stop
int pos;        //  Variable to Store first angle of servo  

int distance_L, distance_F = 30, distance_R;
long distance;

char bt_ir_data;
//-------------------------------------------------------------------------------
void setup() {
  pinMode(SEN_PIN_1, INPUT);
  pinMode(SEN_PIN_2, INPUT);
  pinMode(SEN_PIN_3, INPUT);
  pinMode(SEN_PIN_4, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  

  Serial.begin(9600); 
}

//-------------------------------------------------------------------------------
void forword(){  //forword
digitalWrite(IN1, HIGH); //Right Motor forword Pin 
digitalWrite(IN2, LOW);  //Right Motor backword Pin 
digitalWrite(IN3, LOW);  //Left Motor backword Pin 
digitalWrite(IN4, HIGH); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(IN1, LOW);  //Right Motor forword Pin 
digitalWrite(IN2, HIGH); //Right Motor backword Pin 
digitalWrite(IN3, HIGH); //Left Motor backword Pin 
digitalWrite(IN4, LOW);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(IN1, LOW);  //Right Motor forword Pin 
digitalWrite(IN2, HIGH); //Right Motor backword Pin  
digitalWrite(IN3, LOW);  //Left Motor backword Pin 
digitalWrite(IN4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(IN1, HIGH); //Right Motor forword Pin 
digitalWrite(IN2, LOW);  //Right Motor backword Pin 
digitalWrite(IN3, HIGH); //Left Motor backword Pin 
digitalWrite(IN4, LOW);  //Left Motor forword Pin 
}

void Stop(){ //stop
digitalWrite(IN1, LOW); //Right Motor forword Pin 
digitalWrite(IN2, LOW); //Right Motor backword Pin 
digitalWrite(IN3, LOW); //Left Motor backword Pin 
digitalWrite(IN4, LOW); //Left Motor forword Pin 
}


//-------------------------------------------------------------------------------
void remoteControlMode()
{
  //Read data from the Bluetooth module
  if (Serial.available())
  {
    char command = Serial.read();
    Serial.println(command);
    switch(command)
    {
      case 'F': //Forward
        Velocity(MOTOR_RIGHT , 200);
        Velocity(MOTOR_LEFT  , 200);
        break;      
      case 'B': //Backword
        Velocity(MOTOR_RIGHT , -200);
        Velocity(MOTOR_LEFT  , -200);
        break;
      case 'R': //Right
        Velocity(MOTOR_RIGHT ,  0 );
        Velocity(MOTOR_LEFT  , 200);
        break;
      case 'L': //Left
        Velocity(MOTOR_RIGHT , 200);
        Velocity(MOTOR_LEFT  ,  0 );
        break;
      case 'S': //STOP
        Velocity(MOTOR_RIGHT ,  0 );
        Velocity(MOTOR_LEFT  ,  0 );
        break;
    }
  }  
}
//-------------------------------------------------------------------------------
//--------------------------------Line_follower------------------------------------
//-------------------------------------------------------------------------------
void Line_follower(){
    int V1 = digitalRead (SEN_PIN_1);
    int V2 = digitalRead (SEN_PIN_2);
    if (V1 == 0 && V2 == 0 )
    {
      // Go Forward
      analogWrite(IN1, 255);         
      analogWrite(IN2, 0);
      analogWrite(IN3, 255);
      analogWrite(IN4, 0);
      analogWrite(IN1, 70);
      analogWrite(IN3, 70);
    }
    // Turn Right
    else if (V1 == 1 && V2 == 0 )
    {
      analogWrite(IN1, 255);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 255);
      analogWrite(IN1, 65);
      analogWrite(IN4, 65);
    }
    else if (V1 == 0 && V2 == 1 )
    {
      // Turn Left
      analogWrite(IN1, 0);
      analogWrite(IN2, 60);
      analogWrite(IN3, 60);
      analogWrite(IN4, 0);
    }
    else if (V1 == 1 && V2 == 1 )
    {
      // Go Forward
      analogWrite(IN1, 255);
      analogWrite(IN2, 0);
      analogWrite(IN3, 255);
      analogWrite(IN4, 0);
      analogWrite(IN1, 70);
      analogWrite(IN3, 70);
    }
  }
//--------------------------------------------------------------------------------------
//-------------------------------Remote----------------------------------------
//--------------------------------------------------------------------------------------
void Remote_mode(){
  bt_ir_data = Serial.read();
  Serial.println(bt_ir_data);
  if(bt_ir_data == 'F'){
    forword(); 
    delay(2000);}  // if the bt_data is '1' the DC motor will go forward
  else if(bt_ir_data == 'B'){backword();
  delay(2000);}  // if the bt_data is '2' the motor will Reverse
  else if(bt_ir_data == 'L'){turnLeft();
  delay(2000);}  // if the bt_data is '3' the motor will turn left
  else if(bt_ir_data == 'R'){turnRight();
  delay(2000);} // if the bt_data is '4' the motor will turn right
  else if(bt_ir_data == 'S'){Stop();
  delay(2000);}     // if the bt_data '5' the motor will Stop
}

//--------------------------------------------------------------------------------------
//-------------------------------obstacles----------------------------------------
//--------------------------------------------------------------------------------------
void obstacles()
{
  char distance_F;
  distance_F = Ultrasonic_read();
  Serial.print("S=");
  Serial.println(distance_F);
  if (distance_F > set){forword();}
  else{Check_side();}
  delay(10);

}

  /////////////////////////////////////////////////////////////////////////////////////////////

  long Ultrasonic_read(){
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
   int distance = pulseIn (echo, HIGH);
    return distance / 29 / 2;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////
int distance_L,distance_R;
  void compareDistance(){
        if (distance_L > distance_R){
    turnLeft();
    delay(350);
    }
    else if (distance_R > distance_L){
    turnRight();
    delay(350);
    }
    else{
    backword();
    delay(300);
    turnRight();
    delay(600);
    }
  }

  /////////////////////////////////////////////////////////////////

  void Check_side(){
      Stop();
      delay(100);
  for (int angle = 70; angle <= 140; angle += 5)  {
    servoPulse(servo, angle);  }
      delay(300);
      distance_L = Ultrasonic_read();
      delay(100);
    for (int angle = 140; angle >= 0; angle -= 5)  {
    servoPulse(servo, angle);  }
      delay(500);
      distance_R = Ultrasonic_read();
      delay(100);
  for (int angle = 0; angle <= 70; angle += 5)  {
    servoPulse(servo, angle);  }
      delay(300);
      compareDistance();
  }

  /////////////////////////////////////////////////////////////////

  void servoPulse (int pin, int angle){
  int pwm = (angle*11) + 500;      // Convert angle to microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(50);                   // Refresh cycle of servo
  }
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void loop() {
  
  if (Serial.available())
  {
    char mode = Serial.read();
    Serial.println(mode);
    switch(mode)
    {     
      case 'W': //Bluetooth mode
        Remote_mode();
        break;
      case 'U': //Left
        Line_follower();
        break;
      case 'O': //obstical mode
        obstacles();
        break;
    }
  }  
}
*/