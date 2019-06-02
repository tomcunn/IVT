
#include <IVT.h>

IVT::IVT()
{
}

//*******************************************************
//   init
//   
//   Sets up the L298 motor driver 
//*******************************************************
void IVT:: init()
{
   //Configure the pins 
   pinMode(MotorA_PWM_Channel ,OUTPUT);
   pinMode(MotorA_DIR_ChannelA,OUTPUT);
   pinMode(MotorA_DIR_ChannelB,OUTPUT);
   
   pinMode(MotorB_PWM_Channel ,OUTPUT);
   pinMode(MotorB_DIR_ChannelA,OUTPUT);
   pinMode(MotorB_DIR_ChannelB,OUTPUT);
   
   //Set the pin modes
   pinMode(MotorA_Encoder_Channel_A,INPUT);
   pinMode(MotorA_Encoder_Channel_B,INPUT);

   pinMode(MotorB_Encoder_Channel_A,INPUT);
   pinMode(MotorB_Encoder_Channel_B,INPUT);
   
   delay(500);
   
   //Turn off the bridges
   digitalWrite(MotorA_DIR_ChannelA ,0);
   digitalWrite(MotorA_DIR_ChannelB, 0);

   digitalWrite(MotorB_DIR_ChannelA, 0);
   digitalWrite(MotorB_DIR_ChannelB, 0);
   
   //Turn off the PWM driver
   analogWrite(MotorA_PWM_Channel , 0);
   analogWrite(MotorB_PWM_Channel , 0);

}

//*******************************************************
//   SetMotorPWM
//   
//   Used by application developer to control the motor
//*******************************************************
void IVT:: SetMotorPWM(MOTOR_ID_ENUM motor_id, int pwm_value, int direction)
{
   if(motor_id == MotorA)
   {
      SetMotor(MotorA_DIR_ChannelA,MotorA_DIR_ChannelB,MotorA_PWM_Channel,pwm_value,direction);
   }
   else if(motor_id == MotorB)
   {
      SetMotor(MotorB_DIR_ChannelA,MotorB_DIR_ChannelB,MotorB_PWM_Channel,pwm_value,direction);
   }
}
//*******************************************************
//   SetMotor 
//   
//   Used internally to set the motor pwm and direction
//*******************************************************
void IVT:: SetMotor(int ChannelA, int ChannelB, int PWM_Channel, int pwm_value, int direction)
{
   //Set the direction
   if(direction == 1)
   {
      digitalWrite(ChannelA ,0);
      digitalWrite(ChannelB, 1);
   }
   else if(direction == 2)
   {
      digitalWrite(ChannelB ,0);
      digitalWrite(ChannelA, 1);
   }
   //Direction is 0 or not defined
   else
   {
      digitalWrite(ChannelA ,0);
      digitalWrite(ChannelB, 0);
   }
   //Set the PWM value
   if(pwm_value <= 255)
   {
      analogWrite(PWM_Channel , pwm_value); 
   }
   else
   {
      analogWrite(PWM_Channel , 0); 
   }
}
