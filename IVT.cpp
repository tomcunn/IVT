
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
   //Set the output pins
   pinMode(MotorA_PWM_Channel ,OUTPUT);
   pinMode(MotorA_DIR_ChannelA,OUTPUT);
   pinMode(MotorA_DIR_ChannelB,OUTPUT);
   
   pinMode(MotorB_PWM_Channel ,OUTPUT);
   pinMode(MotorB_DIR_ChannelA,OUTPUT);
   pinMode(MotorB_DIR_ChannelB,OUTPUT);
   
   //Set the inputs pin
   pinMode(MotorA_Encoder_Channel_A,INPUT);
   pinMode(MotorA_Encoder_Channel_B,INPUT);
   pinMode(MotorB_Encoder_Channel_A,INPUT);
   pinMode(MotorB_Encoder_Channel_B,INPUT);
   pinMode(JOYSTICKX, INPUT);
   pinMode(JOYSTICKY, INPUT);
   pinMode(BARE_POT, INPUT);
   pinMode(V_MOTOR_A3, INPUT);
   pinMode(A4, INPUT);
   pinMode(A5, INPUT);
   
   
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

//*******************************************************
//   ReadBatteryVoltage 
//   
//   Read in the battery pack voltage, return mv.
//*******************************************************
int IVT:: ReadBatteryVoltage()
{
   int BatteryVoltage_ADC;
   int BatteryVoltage_mV;
   
   //Read in the voltage of the following PIN, this is 10 bit
   BatteryVoltage_ADC =  analogRead(V_MOTOR_A3); 
   
   //Convert from ADC to mV, assume a 5v reference
   //Use 512 instead of 1024 since there is a voltage divider that 
   //divides by 2. 
   BatteryVoltage_mV = int(long(BatteryVoltage_ADC) * 5000 / 512);
   
   return BatteryVoltage_mV;
}

//*********************************************************
//   SendCANMessage
//
//   Send a serial message, not the CAN protocol but still teaches 
//   the fundamentals.
//**********************************************************
void IVT:: SendCANMessage(int data[])
{
   String datastring;
   String temp;
   
   datastring = "8 ";
   
   //Load the data and convert to hex.
   for(int i = 0; i < 8; i++)
   {
      datastring += " ";
      temp = String(data[i], HEX);
      //Pad the string
      if(temp.length() == 1)
      {
          temp = "0"+temp;
      }
      datastring += temp;

   }
   Serial.println(datastring);
}

//***********************************************************
//  GetJoystickValue
//
//  Pass in the 10 bit ADC value from the joystick and the end points
//  and this will return a value from -100 to 100, the joystick 
//  can be inteverted with the last argument
//***********************************************************
/*signed int IVT:: GetJoystickValue(int ADC, int Low_EndPoint_ADC, int High_EndPoint_ADC, bool inverted)
{
   int range;
   int midpoint;
   signed int handle_percent;
   
   //Calculate the range
   range = abs(High_EndPoint_ADC - Low_EndPoint_ADC);
   
   //scale 200% is the full scale range, instead of dividing by 200 use 20.
   scalex10 = range/20;
   
   handle_percent = (ADC-midpoint)*scalex10/10;  
   if(handle_percent > 100)
   {
	   handle_percent = 100;
   }
   else if(handle_percent <-100)
   {
	   handle_percent = -100;
   }
   
   return handle_percent;
}
*/