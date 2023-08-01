 #include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
#define SERVOMIN 102
#define SERVOMAX 500
#include <ESP32Servo.h>
#include "BluetoothSerial.h"

BluetoothSerial serialBT;
char cmd;



double a0; double a1; double a2; double a3;
int leg1[] = {0,1,2};int leg2[] = {3,4,5}; int leg3[] = {6,7,8};
int leg4[] = {9,10,11}; int leg5[] = {12,13,14}; int leg6[] = {15,0,1};


double theta1;
double theta2;
double theta3; 
double theta4;
double theta5;
double theta6;

// 更改tf k
void setup() {
  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setOscillatorFrequency(27000000);
  pwm1.begin();
  pwm1.setPWMFreq(50);
  pwm1.setOscillatorFrequency(27000000);
  Serial.begin(115200);
  serialBT.begin("ESP32-BT");
  pinMode(2,OUTPUT);
}

void loop(){
  if (serialBT.available()){
    cmd = serialBT.read();
  }
  if(cmd == '1'){
        forward(); 
        digitalWrite(2,HIGH);
     }
  if(cmd == '2'){
        backward(); 
        digitalWrite(2,LOW);
    }
  if(cmd == '3'){
        turnright(); 
        digitalWrite(2,HIGH);
    }
  if(cmd == '4'){
        turnleft(); 
        digitalWrite(2,LOW);
     }  
  if(cmd == '5'){
        stand(); 
        digitalWrite(2,HIGH);
     } 
  delay(100);
}




int angle2pulse(int angle){
  int pulse ;
  if (angle <= 180){
    pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  }
  return pulse;
}

double cubicsp(double _thetai, double _thetaf,double _tf,double t){
      //a0 = 70;a1 = 0;a2 = 30 ;a3 = -10;
      //double thetai=45; double thetaf=135;
      double _theta0;
//      a0 = _thetai;
//      a1 = 0;
//      a2 = 3/(pow(_tf,2))*(_thetaf - _thetai);
//      a3 = -2/(pow(_tf,3))*(_thetaf - _thetai);
//      _theta0 = a0 + a1*(t) +a2*(pow(t,2)) +a3*(pow(t,3));
    
      return _thetaf; 
  }

void tryit(){
  pwm.setPWM(2,0,angle2pulse(60));   
  pwm.setPWM(8,0,angle2pulse(60)); 
  pwm.setPWM(14,0,angle2pulse(60)); 
  pwm.setPWM(5,0,angle2pulse(120));   
  pwm.setPWM(11,0,angle2pulse(120)); 
  pwm.setPWM(17,0,angle2pulse(120));
}
void stand(){

  pwm.setPWM(0,0,angle2pulse(90));
  pwm.setPWM(1,0,angle2pulse(75));
  pwm.setPWM(2,0,angle2pulse(0));

  pwm.setPWM(6,0,angle2pulse(90));
  pwm.setPWM(7,0,angle2pulse(75));
  pwm.setPWM(8,0,angle2pulse(0));

  pwm.setPWM(12,0,angle2pulse(90));
  pwm.setPWM(13,0,angle2pulse(75));
  pwm.setPWM(14,0,angle2pulse(0));

  pwm.setPWM(3,0,angle2pulse(90));
  pwm.setPWM(4,0,angle2pulse(75));
  pwm.setPWM(5,0,angle2pulse(0));

  pwm.setPWM(9,0,angle2pulse(90));
  pwm.setPWM(10,0,angle2pulse(75));
  pwm.setPWM(11,0,angle2pulse(0));

  pwm.setPWM(15,0,angle2pulse(90));
  pwm1.setPWM(0,0,angle2pulse(75));
  pwm1.setPWM(1,0,angle2pulse(0));

}

void backward() {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(60,60,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<1;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0));
}

void forward() {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<1;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0));
}

void initial(){
  pwm.setPWM(0,0,angle2pulse(90));
  pwm.setPWM(1,0,angle2pulse(135));
  pwm.setPWM(2,0,angle2pulse(0));

  pwm.setPWM(6,0,angle2pulse(90));
  pwm.setPWM(7,0,angle2pulse(135));
  pwm.setPWM(8,0,angle2pulse(0));

  pwm.setPWM(12,0,angle2pulse(90));
  pwm.setPWM(13,0,angle2pulse(135));
  pwm.setPWM(14,0,angle2pulse(0));

  pwm.setPWM(3,0,angle2pulse(90));
  pwm.setPWM(4,0,angle2pulse(135));
  pwm.setPWM(5,0,angle2pulse(0));

  pwm.setPWM(9,0,angle2pulse(90));
  pwm.setPWM(10,0,angle2pulse(135));
  pwm.setPWM(11,0,angle2pulse(0));

  pwm.setPWM(15,0,angle2pulse(90));
  pwm1.setPWM(0,0,angle2pulse(135));
  pwm1.setPWM(1,0,angle2pulse(0));

}

void turnleft() {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<1;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0));
}

void turnright() {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<1;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(30));   
  pwm.setPWM(11,0,angle2pulse(30)); 
  pwm1.setPWM(1,0,angle2pulse(30));
}
void goright() {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(30));   
  pwm.setPWM(8,0,angle2pulse(30)); 
  pwm.setPWM(14,0,angle2pulse(30)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(30));   
  pwm.setPWM(11,0,angle2pulse(30)); 
  pwm1.setPWM(1,0,angle2pulse(30)); 

  delay(1000);
  for(int j=0;j<1;j++){     
  double timestep = 0.01; //1
  double tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
 
  timestep = 0.01; //2
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);             // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                 // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta4 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(0,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,0,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  }
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0));
}
void goleft() {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(30));   
  pwm.setPWM(8,0,angle2pulse(30)); 
  pwm.setPWM(14,0,angle2pulse(30)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(30));   
  pwm.setPWM(11,0,angle2pulse(30)); 
  pwm1.setPWM(1,0,angle2pulse(30)); 

  delay(1000);
  for(int j=0;j<1;j++){     
  double timestep = 0.01; //1
  double tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
 
  timestep = 0.01; //2
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);             // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                 // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta2 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta4 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(0,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(60,0,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  }
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0));
}
void niupipi(){
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(75));   
  pwm.setPWM(7,0,angle2pulse(75)); 
  pwm.setPWM(13,0,angle2pulse(75)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(75));   
  pwm.setPWM(10,0,angle2pulse(75)); 
  pwm1.setPWM(0,0,angle2pulse(75)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000); 
  for(int j=0;j<10;j++){    
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(90,105,tf,t); //小腿1,3,5
    theta2 = linearsp(90,75,tf,t); //小腿2,4,6
    theta3=linearsp(90,90,tf,t);
    theta4=linearsp(75,90,tf,t);

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //1
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(105,90,tf,t); //小腿1,3,5
    theta2 = linearsp(75,90,tf,t); //小腿2,4,6
    theta3 =linearsp(90,90,tf,t);
    theta4 =linearsp(90,75,tf,t); //小腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //1
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(90,75,tf,t); //小腿1,3,5
    theta2 = linearsp(90,105,tf,t); //小腿2,4,6
    theta3 =linearsp(90,90,tf,t);
    theta4 =linearsp(75,90,tf,t); //小腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //1
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(75,90,tf,t); //小腿1,3,5
    theta2 = linearsp(105,90,tf,t); //小腿2,4,6
    theta3 =linearsp(90,90,tf,t);
    theta4 =linearsp(90,75,tf,t); //小腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
}

double linearsp(double _thetai, double _thetaf,double _tf,double t){ 
      double theta;
      theta=_thetai+(_thetaf-_thetai)*t/_tf;
      return _thetaf; 
  }
