#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
#define SERVOMIN 102
#define SERVOMAX 500
#include <ESP32Servo.h>

Servo myservo;  

float a0; float a1; float a2; float a3;


float currentFrontFemurR, currentFrontTibiaR, currentFrontCoxaR, currentMiddleFemurR, currentMiddleTibiaR, currentMiddleCoxaR, currentRearFemurR, currentRearTibiaR, currentRearCoxaR;
float currentFrontFemurL, currentFrontTibiaL, currentFrontCoxaL, currentMiddleFemurL, currentMiddleTibiaL, currentMiddleCoxaL, currentRearFemurL, currentRearTibiaL, currentRearCoxaL;



//robot dimensions 
float BodySideLength = 10.5;
float bodylength = 200; 

// Leg dimensions (modify these based on your robot's design)
const float COXA_LENGTH =77 ;   // length of coxa segment (mm)
const float FEMUR_LENGTH = 85;  // length of femur segment (mm)
const float TIBIA_LENGTH = 110;  // length of tibia segment (mm)

 
 


void moveUpDownTilt(float Z, float L1, float tiltX, float tiltY) {
    // Inverse Kinematics
    float coxaFL, femurFL, tibiaFL;
    float coxaFR, femurFR, tibiaFR;
    float coxaML, femurML, tibiaML;
    float coxaMR, femurMR, tibiaMR;
    float coxaBL, femurBL, tibiaBL;
    float coxaBR, femurBR, tibiaBR;

    // Front-left leg
    InverseKinematics(&coxaFL, &femurFL, &tibiaFL, Z-tiltY-tiltX, L1);
    // Front-right leg
    InverseKinematics(&coxaFR, &femurFR, &tibiaFR, Z-tiltY+tiltX, L1);
    // Middle-left leg
    InverseKinematics(&coxaML, &femurML, &tibiaML, Z-tiltX, L1);
    // Middle-right leg
    InverseKinematics(&coxaMR, &femurMR, &tibiaMR, Z+tiltX, L1);
    // Back-left leg
    InverseKinematics(&coxaBL, &femurBL, &tibiaBL, Z+tiltY-tiltX, L1);
    // Back-right leg
    InverseKinematics(&coxaBR, &femurBR, &tibiaBR, Z+tiltY+tiltX, L1);

    servoControl(femurFR, tibiaFR, coxaFR, femurMR, tibiaMR, coxaMR, femurBR, tibiaBR, coxaBR, 
                 femurFL, tibiaFL, coxaFL, femurML, tibiaML, coxaML, femurBL, tibiaBL, coxaBL);
    delay(500);

}





void twist(float twist) {

    // Inverse Kinematics
    float coxa,femur, tibia;
    
    InverseKinematics(&coxa, &femur, &tibia, 10, 45);
    
    float coxaT=coxa-twist;

    // Servo Control
    servoControl(femur, tibia, coxaT, femur, tibia, coxaT, femur, tibia, coxaT, femur, tibia, coxaT, femur, tibia, coxaT, femur, tibia, coxaT);
    delay(500);


}


void demo (){

  float coxa,femur, tibia;

  

float Z=20;
float L1=40;

  float tiltX = 0;
  float tiltY = 0;

 moveUpDownTilt(Z, L1, tiltX, tiltY);

  Z=40;

  moveUpDownTilt(Z, L1, tiltX, tiltY);
  
  Z=10;

  moveUpDownTilt(Z, L1, tiltX, tiltY);
  

float twistangle = 25;

twist(twistangle) ;

twistangle= -25;
twist(twistangle);


  Z=20;
  tiltX = 20;
  tiltY = 20;

 moveUpDownTilt(Z, L1, tiltX, tiltY);
            

   tiltX = -20;
   tiltY = -20;

 moveUpDownTilt(Z, L1, tiltX, tiltY);


   tiltX = 20;
   tiltY = -10;
 moveUpDownTilt(Z, L1, tiltX, tiltY);


 tiltX = -20;
   tiltY = 10;

    moveUpDownTilt(Z, L1, tiltX, tiltY);

  
}


float cubicsp(float _thetai, float _thetaf, double _tf, double t) {
  float _theta0;
  a0 = _thetai;
  a1 = 0;
  a2 = 3/(pow(_tf,2))*(_thetaf - _thetai);
  a3 = -2/(pow(_tf,3))*(_thetaf - _thetai);
  _theta0 = a0 + a1*(t) + a2*(pow(t,2)) + a3*(pow(t,3));
  return _theta0;
 
}






int angle2pulse(int angle){
  int pulse ;

  if (angle <= 180){
    pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  }
  return pulse;
}



void move(float distance, float speed) {
  
    float stepSize = 20;
    int steps = distance / stepSize;
    float timePerStep = 2000 / speed;

    // Starting from rest position
    float coxa[6] = {90, 90, 90, 90, 90, 90};
    float femur[6] = {70, 70, 70, 70, 70, 70};
    float tibia[6] = {0, 0, 0, 0, 0, 0};

    float deltaCoxa = stepSize;


   servoControl(femur[0], tibia[0], coxa[0], femur[1], tibia[1], coxa[1], femur[2], tibia[2], coxa[2],
                     femur[3], tibia[3], coxa[3], femur[4], tibia[4], coxa[4], femur[5], tibia[5], coxa[5]);


    for (int step = 0; step < steps; step++) {
      

        //move tripod 1 forward
        InverseKinematics(&coxa[0], &femur[0], &tibia[0], -10, 40);
        InverseKinematics(&coxa[2], &femur[2], &tibia[2], -10, 40);
        InverseKinematics(&coxa[4], &femur[4], &tibia[4], -10, 40);

        
        coxa[0] += deltaCoxa;
        coxa[2] += deltaCoxa;
        coxa[4] -= deltaCoxa;

        //move tripod 2 backward
        InverseKinematics(&coxa[1], &femur[1], &tibia[1], 0, 30);
        InverseKinematics(&coxa[3], &femur[3], &tibia[3],30, 30);
        InverseKinematics(&coxa[5], &femur[5], &tibia[5], 10, 30);
        coxa[1] -= deltaCoxa;
        coxa[3] += deltaCoxa;
        coxa[5] += deltaCoxa;



        delay(timePerStep / 2);
        servoControl(femur[0], tibia[0], coxa[0], femur[1], tibia[1], coxa[1], femur[2], tibia[2], coxa[2],
                     femur[3], tibia[3], coxa[3], femur[4], tibia[4], coxa[4], femur[5], tibia[5], coxa[5]);

        //move them all down
        InverseKinematics(&coxa[0], &femur[0], &tibia[0], 30, 40);
        InverseKinematics(&coxa[2], &femur[2], &tibia[2], 10, 40);
        InverseKinematics(&coxa[4], &femur[4], &tibia[4], 20, 40);

        InverseKinematics(&coxa[1], &femur[1], &tibia[1], 20, 30);
        InverseKinematics(&coxa[3], &femur[3], &tibia[3], 10, 30);
        InverseKinematics(&coxa[5], &femur[5], &tibia[5], 30, 30);

        delay(timePerStep / 4);
        servoControl(femur[0], tibia[0], coxa[0], femur[1], tibia[1], coxa[1], femur[2], tibia[2], coxa[2],
                     femur[3], tibia[3], coxa[3], femur[4], tibia[4], coxa[4], femur[5], tibia[5], coxa[5]);

        //Move tripod 1 backward
        InverseKinematics(&coxa[0], &femur[0], &tibia[0], 20, 30);
        InverseKinematics(&coxa[2], &femur[2], &tibia[2], 10, 30);
        InverseKinematics(&coxa[4], &femur[4], &tibia[4], 20, 30);
        coxa[0] -= deltaCoxa;
        coxa[2] -= deltaCoxa;
        coxa[4] += deltaCoxa;

        //move tripod 2 forward
        InverseKinematics(&coxa[1], &femur[1], &tibia[1], -10, 40);
        InverseKinematics(&coxa[3], &femur[3], &tibia[3], -10, 40);
        InverseKinematics(&coxa[5], &femur[5], &tibia[5], -10, 40);
        coxa[1] += deltaCoxa;
        coxa[3] -= deltaCoxa;
        coxa[5] -= deltaCoxa;

        delay(timePerStep / 2);
        servoControl(femur[0], tibia[0], coxa[0], femur[1], tibia[1], coxa[1], femur[2], tibia[2], coxa[2],
                     femur[3], tibia[3], coxa[3], femur[4], tibia[4], coxa[4], femur[5], tibia[5], coxa[5]);

        //move them all down
           InverseKinematics(&coxa[0], &femur[0], &tibia[0], 30, 30);
        InverseKinematics(&coxa[2], &femur[2], &tibia[2], 10, 30);
        InverseKinematics(&coxa[4], &femur[4], &tibia[4], 20, 40);

        InverseKinematics(&coxa[1], &femur[1], &tibia[1], 20, 40);
        InverseKinematics(&coxa[3], &femur[3], &tibia[3], 10, 40);
        InverseKinematics(&coxa[5], &femur[5], &tibia[5], 30, 40);

        delay(timePerStep / 4);
        servoControl(femur[0], tibia[0], coxa[0], femur[1], tibia[1], coxa[1], femur[2], tibia[2], coxa[2],
                     femur[3], tibia[3], coxa[3], femur[4], tibia[4], coxa[4], femur[5], tibia[5], coxa[5]);
    }


delay(500);

     servoControl(femur[0], tibia[0], coxa[0], femur[1], tibia[1], coxa[1], femur[2], tibia[2], coxa[2],
                     femur[3], tibia[3], coxa[3], femur[4], tibia[4], coxa[4], femur[5], tibia[5], coxa[5]);
}







void setup() {

  
   pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setOscillatorFrequency(27000000);
  pwm1.begin();
  pwm1.setPWMFreq(50);
  pwm1.setOscillatorFrequency(27000000);

  Serial.begin(9600);

}

void loop() {
    static char received_chars[32];
    static byte num_received = 0;
    static int function_number;
    static float val1, val2,val3,val4;

    if (Serial.available() > 0) {
        char in_char = Serial.read();
        
        if (in_char != '\n') {
            // Add incoming byte to the buffer
            received_chars[num_received] = in_char;
            num_received++;
        } else {
            // Null-terminate the received string
            received_chars[num_received] = 0;
        
            // Parse the string for function_number and parameters
           // Parse the string for function_number and parameters
           if (sscanf(received_chars, "%d,%f,%f,%f,%f", &function_number, &val1, &val2, &val3, &val4) >= 1) {
    
                // Choose function to execute based on function_number
                switch(function_number) {
                    case 0:
                        // Validate inputs for moveUpDown
                        if ((FEMUR_LENGTH + TIBIA_LENGTH) <= val2)
                        {
                            Serial.println("Invalid L1 value. It should be less than the sum of FEMUR_LENGTH and TIBIA_LENGTH.");
                            break;
                        }

                        if (abs(FEMUR_LENGTH - TIBIA_LENGTH) >= val2)
                        {
                            Serial.println("Invalid L1 value. It should be more than the absolute difference between FEMUR_LENGTH and TIBIA_LENGTH.");
                            break;
                        }
                        if ((FEMUR_LENGTH + TIBIA_LENGTH) <= val1)
                        {
                            Serial.println("Invalid Z value. It should be less than the sum of FEMUR_LENGTH and TIBIA_LENGTH.");
                            break;
                        }
                        
                         moveUpDownTilt(val1, val2,val3,val4);
                       // moveUpDown(val1, val2);
                         
                        Serial.println(currentFrontFemurR);
                        break;
                    case 1:
                        // Execute function twist
                        demo();
                        break;
                
                  case 2:
                        // Execute function twist
                        move(val1,val2);
                        break;

                  default:
                        Serial.println("Invalid function_number");
                
                }
            } else {
                Serial.println("Failed to parse input");
            }

            // Reset the buffer
            num_received = 0;
        }
    }

    delay(100);
}



void InverseKinematics (float* coxa,
                       float* femur,
                       float* tibia ,
                       float Z,
                       float L1)
{
     //*coxa = 90; 

    *femur = 270 - (acos(   (pow(FEMUR_LENGTH ,2) +  pow(L1 ,2) - pow(Z ,2)  -pow(TIBIA_LENGTH ,2)) /  (2 * FEMUR_LENGTH * sqrt(pow(Z ,2) +pow(L1 ,2)) ) ) * (180 / PI)) - (acos(  Z / sqrt(pow(Z ,2) +pow(L1 ,2) ) ) * (180 / PI));

   *tibia = (acos(   (pow(FEMUR_LENGTH ,2) +  pow(TIBIA_LENGTH ,2) - pow(L1 ,2)  -pow(Z ,2)) /  (2 * FEMUR_LENGTH * TIBIA_LENGTH) ) * (180 / PI));
}


 






 

void servoControl(float frontFemurR, float frontTibiaR, float frontCoxaR, float middleFemurR, float middleTibiaR, float middleCoxaR, float rearFemurR, float rearTibiaR, float rearCoxaR, 
                  float frontFemurL, float frontTibiaL, float frontCoxaL, float middleFemurL, float middleTibiaL, float middleCoxaL, float rearFemurL, float rearTibiaL, float rearCoxaL) {
                    

    for (double t = 0; t <= 1; t += 0.01) {
        // Right
         double frontFemurR_Traj = cubicsp(currentFrontFemurR, frontFemurR, 2, t);
        double frontTibiaR_Traj = cubicsp(currentFrontTibiaR, frontTibiaR, 2, t);
        double frontCoxaR_Traj = cubicsp(currentFrontCoxaR, frontCoxaR-20, 2, t);
        double middleFemurR_Traj = cubicsp(currentMiddleFemurR, middleFemurR, 2, t);
        double middleTibiaR_Traj = cubicsp(currentMiddleTibiaR, middleTibiaR, 2, t);
        double middleCoxaR_Traj = cubicsp(currentMiddleCoxaR, middleCoxaR, 2, t);
        double rearFemurR_Traj = cubicsp(currentRearFemurR, rearFemurR, 2, t);
        double rearTibiaR_Traj = cubicsp(currentRearTibiaR, rearTibiaR , 2, t);
        double rearCoxaR_Traj = cubicsp(currentRearCoxaR, rearCoxaR+30 , 2, t);

        //Left
        double frontFemurL_Traj = cubicsp(currentFrontFemurL, frontFemurL , 2, t);
        double frontTibiaL_Traj = cubicsp(currentFrontTibiaL, frontTibiaL , 2, t);
        double frontCoxaL_Traj = cubicsp(currentFrontCoxaL, frontCoxaL+20 , 2, t);
        double middleFemurL_Traj = cubicsp(currentMiddleFemurL, middleFemurL , 2, t);
        double middleTibiaL_Traj = cubicsp(currentMiddleTibiaL, middleTibiaL , 2, t);
        double middleCoxaL_Traj = cubicsp(currentMiddleCoxaL, middleCoxaL , 2, t);
        double rearFemurL_Traj = cubicsp(currentRearFemurL, rearFemurL , 2, t);
        double rearTibiaL_Traj = cubicsp(currentRearTibiaL, rearTibiaL , 2, t);
        double rearCoxaL_Traj = cubicsp(currentRearCoxaL, rearCoxaL-30 , 2, t);

        currentFrontFemurR = frontFemurR_Traj;
        currentFrontTibiaR = frontTibiaR_Traj;
        currentFrontCoxaR = frontCoxaR_Traj;
        currentMiddleFemurR = middleFemurR_Traj;
        currentMiddleTibiaR = middleTibiaR_Traj;
        currentMiddleCoxaR = middleCoxaR_Traj;
        currentRearFemurR = rearFemurR_Traj;
        currentRearTibiaR = rearTibiaR_Traj;
        currentRearCoxaR = rearCoxaR_Traj;

        currentFrontFemurL = frontFemurL_Traj;
        currentFrontTibiaL = frontTibiaL_Traj;
        currentFrontCoxaL = frontCoxaL_Traj;
        currentMiddleFemurL = middleFemurL_Traj;
        currentMiddleTibiaL = middleTibiaL_Traj;
        currentMiddleCoxaL = middleCoxaL_Traj;
        currentRearFemurL = rearFemurL_Traj;
        currentRearTibiaL = rearTibiaL_Traj;
        currentRearCoxaL = rearCoxaL_Traj;


        // Apply to right side servos
        pwm.setPWM(0, 0, angle2pulse(frontCoxaR_Traj));        
        pwm.setPWM(2, 0, angle2pulse(frontTibiaR_Traj));
        pwm.setPWM(1, 0, angle2pulse(frontFemurR_Traj));
        pwm.setPWM(3, 0, angle2pulse(middleCoxaR_Traj));
        pwm.setPWM(5, 0, angle2pulse(middleTibiaR_Traj));
        pwm.setPWM(4, 0, angle2pulse(middleFemurR_Traj));
        pwm.setPWM(6, 0, angle2pulse(rearCoxaR_Traj));
        pwm.setPWM(8, 0, angle2pulse(rearTibiaR_Traj));
        pwm.setPWM(7, 0, angle2pulse(rearFemurR_Traj));


        // Apply to left side servos
        pwm.setPWM(9, 0, angle2pulse(frontCoxaL_Traj));
        pwm.setPWM(11, 0, angle2pulse(frontTibiaL_Traj));
        pwm.setPWM(10, 0, angle2pulse(frontFemurL_Traj));
        pwm.setPWM(12, 0, angle2pulse(middleCoxaL_Traj));
        pwm.setPWM(14, 0, angle2pulse(middleTibiaL_Traj));
        pwm.setPWM(13, 0, angle2pulse(middleFemurL_Traj));
        pwm.setPWM(15, 0, angle2pulse(rearCoxaL_Traj));
        pwm1.setPWM(1, 0, angle2pulse(rearTibiaL_Traj));
        pwm1.setPWM(0, 0, angle2pulse(rearFemurL_Traj));

        delay(10); // Wait a bit before the next step


    }
}








