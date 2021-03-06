#include <util/atomic.h> // For the ATOMIC_BLOCK macro

#define MOTOR_PIN_1 8
#define MOTOR_PIN_2 9
#define MOTOR_PWM_PIN 10

#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

#define MAX_POWER 100
#define ERROR_TOLERANCE 0

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long time_previous = 0;
float error_previous = 0;
float error_integral = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_PIN_A,INPUT);
  pinMode(ENCODER_PIN_B,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A),readEncoder,RISING);
  
  pinMode(MOTOR_PWM_PIN,OUTPUT);
  pinMode(MOTOR_PIN_1,OUTPUT);
  pinMode(MOTOR_PIN_2,OUTPUT);
  Serial.print("target pos\n");
}

void loop() {
  int target = 210;//1000;
  //int target = 202*sin(time_previous/1e6);

  // PID constants
  float kp = 20.00;
  float kd = 1.55;
  float ki = 0.05;

  long time_current = micros(); // time difference
  float time_delta = ((float) (time_current - time_previous))/( 1.0e6 );
  time_previous = time_current;

  // Read the position in an atomic block to avoid a potential misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { pos = posi;
  }
  
  int error = target - pos; //Error
  error_integral = error_integral + error*time_delta; //Integral
  float dedt = (error-error_previous)/(time_delta); //Derivative  
  float u = (kp*error) + (kd*dedt) + (ki*error_integral); //Control signal

  float power = fabs(u); //Motor power
  if( power > MAX_POWER ){ power = MAX_POWER; }
  else if( error < ERROR_TOLERANCE ){ 
    power = 0; 
    u = 0;
  }
  

  
  int dir = 0; // motor direction
  if(u<0){ dir = -1; }
  else if(u>0){ dir = 1; }

  setMotor(dir,power,MOTOR_PWM_PIN,MOTOR_PIN_1,MOTOR_PIN_2); //Signal the motor

  error_previous = error; // store previous error

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}

void setMotor(int dir, int pwmVal, int pwmPin, int pin1, int pin2){
  analogWrite(pwmPin,pwmVal);
  if(dir == 1){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
  }
  else if(dir == -1){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,HIGH);
  }
  else{
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
  }  
}

void readEncoder(){
  int b = digitalRead(ENCODER_PIN_B);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}
