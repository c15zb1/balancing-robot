#ifndef _balancingrobot_h_
#define _balancingrobot_h_

#include <stdint.h> // Needed for uint8_t

#define PWM_FREQUENCY 20000 // The motor driver can handle a pwm frequency up to 20kHz
#define PWMVALUE F_CPU/PWM_FREQUENCY/2 // Frequency is given by F_CPU/(2*N*ICR) - where N is the prescaler, we use no prescaling so frequency is given by F_CPU/(2*ICR) - ICR = F_CPU/PWM_FREQUENCY/2

/* Used for the PS3 Communication and motor functions */
int lastCommand; // This is used set a new targetPosition
enum Command {
  update,
  stop,
  forward,
  backward,
  left,
  right,
};

/* These are used to read and write to the port registers - see http://www.arduino.cc/en/Reference/PortManipulation 
 I do this to save processing power - see this page for more information: http://www.billporter.info/ready-set-oscillate-the-fastest-way-to-change-arduino-pins/ */
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

/* Left motor */
#define leftPort PORTD
#define leftPortDirection DDRD
#define leftPwmPortDirection DDRB
//connection 2
#define leftA PIND0 // PD0 - pin 0
#define leftB PIND1 // PD1 - pin 1
#define leftPWM PINB1 // PB1 - pin 9 (OC1A)

/* Right motor */
#define rightPort PORTC
#define rightPortDirection DDRC
#define rightPwmPortDirection DDRB
//connection 1
#define rightA PINC2 // PC2 - pin A2
#define rightB PINC3 // PC3 - pin A3
#define rightPWM PINB2 // PB2 - pin 10 (OC1B)

/* Encoders */
#define leftEncoder1 2
#define leftEncoder2 4
#define rightEncoder1 3
#define rightEncoder2 5

volatile long leftCounter = 0;
volatile long rightCounter = 0;

/* IMU */
//#define gyroY A0
//#define accY A1
//#define accZ A2

#define buzzer 6 // Connected to a BC547 transistor - there is a protection diode at the buzzer as well

// Zero voltage values for the sensors - [0] = gyroY, [1] = accY, [2] = accZ
double zeroValues[3] = { 0 };

/* Kalman filter variables and constants */
const double Q_angle = 0.001; // Process noise covariance for the accelerometer - Sw
const double Q_gyro = 0.003; // Process noise covariance for the gyro - Sw
const double R_angle = 0.03; // Measurement noise covariance - Sv

double angle; // The angle output from the Kalman filter
double bias = 0; // The gyro bias calculated by the Kalman filter
double P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;
double dt, y, S;
double K_0, K_1;

// Results
double accYangle;
double gyroYrate;
double pitch;

/* PID variables */
const double Kp = 7;
const double Ki = 2;
const double Kd = 8;
const double targetAngle = 180;

double lastError; // Store position error
double iTerm; // Store integral term

/* Used for timing */
unsigned long timer;

#define STD_LOOP_TIME 10000 // Fixed time loop of 10 milliseconds
unsigned long lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime;

bool steerForward;
bool steerBackward;
bool steerStop = true; // Stop by default
bool steerLeft;
bool steerRight;

bool stopped; // This is used to set new target position after breaking

bool layingDown = true; // Use to indicate if the robot is laying down

double targetOffset = 0; // Offset for going forward and backward
double turningOffset = 0; // Offset for turning left and right

uint8_t loopCounter = 0; // Used to update wheel velocity and buzzer beep
long wheelPosition;
long lastWheelPosition;
long wheelVelocity;
long targetPosition;
int zoneA = 4000;
int zoneB = 2000;
double positionScaleA = 250; // one resolution is 464 pulses
double positionScaleB = 500; 
double positionScaleC = 1000;
double velocityScaleMove = 40;
double velocityScaleStop = 30;
double velocityScaleTurning = 35;
#endif
