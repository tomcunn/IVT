#ifndef IVT_h
#define IVT_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

#define MotorA_PWM_Channel  9  
#define MotorA_DIR_ChannelA 7 
#define MotorA_DIR_ChannelB 8

#define MotorB_PWM_Channel  10  
#define MotorB_DIR_ChannelA 5 
#define MotorB_DIR_ChannelB 6

#define  MotorA_Encoder_Channel_A  2
#define  MotorA_Encoder_Channel_B  12

#define  MotorB_Encoder_Channel_A  3
#define  MotorB_Encoder_Channel_B  11

enum MOTOR_ID_ENUM
{
   MotorA,
   MotorB
};


// Your class header here...
class IVT
{
    public:
       //Constructor, required, same name as class
       IVT();
       //Sets up all the pins properly for the I/O needed to control the L298
       void init();
       //Used by the application developer to control the motors
       void SetMotorPWM(MOTOR_ID_ENUM motor_id, int pwm_value, int direction);

    private:
       //Used internally to load the values for the motor.
       void SetMotor(int ChannelA, int ChannelB, int PWM_Channel, int pwm_value, int direction);
};
#endif
