# Scorpion_Robot
 The hexapod scorpion robot is designed with MG995 servos to control the joint angles, providing 3 degrees of freedom for each joint. The robot has two hands and a tail joint designed using biomimetic principles. It uses both MG995 and SG90 servos for driving. The PWM signals for the servos are expanded to 32 channels using PCA9685.
 
# The main tasks of the project are as follows:
  1. 3D Modeling of the robot (Solidworks2022)
  2. PCB Circuit Design(Altium Designer)
  3. Establishment of the mathematical model simulation system for the robot(ROS-Gazebo, Pybullet and Matlab)
  4. Forward and inverse kinematic calculation for the robot.
  5. Robot GUI/HMI Design (HTML and Android Studio)
  6. Robot's Gait Design (Arduino IDE)


This project primarily controls 25 servos, thus requiring two PCA9685 modules to expand the PWM signals for the servos. The communication between PCA9685 and the ESP main controller is established using I2C protocol. Additionally, the addresses of the two PCA9685 modules need to be set as 0x40 and 0x41 to distinguish the communication targets.

# Documentation
##1. 3D Model Scorpion Robot MG995
  The file is modeled based on the MG995 servo motor. The hand joints of the robot are driven by bevel gears to achieve freedom along the z-axis. Meanwhile, the tail joint of the robot is designed using a biomimetic mechanical finger structure as a reference.
##2. 3D Model Scorpion Robot SG90
   The file is primarily designed using the SG90 servo motor for the robot. However, due to the limited driving torque of the SG90 and issues with the quality of plastic servos, this design version faces challenges in achieving the set objectives. As a result, it is only included here for reference purposes.
## 3. Control Code
   Inside, it is mainly divided into the following sections:
   ### 3.1 Gait Control
       Programming the robot's gait using forward kinematics algorithms.The two files represent the user interfaces (UI) for the robot's control system. One is developed using Android Studio, and the other is an HTML version.
       #### 3.1.1
       Android Studio Version (Android Studio)
       #### 3.1.2
       HTML Version (Arduino IDE + HTML)
   ### 3.2 Inverse Kinematic    
T      The file mainly adjusts the robot's posture using inverse kinematics. Based on this, the inverse kinematics can be integrated with sensors like MPU6050 to achieve closed-loop control of the robot's motion/pose.
## 4. ESPCAM
   By programming ESPCAM, this file is used for the vision system of the robot.
## 5. PCB_Robot
    The file is the PCB circuit design of the robot, created using Altium Designer 23 to layout the components on the module.
