#include <avr/io.h>
#include <avr/interrupt.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <IVT.h>

IVT ivt();
//****************DEFINE THE PINS**************************
#define JoyStickA 14
//*********************************************************


Encoder MotorPositionA(MotorA_Encoder_Channel_A, MotorA_Encoder_Channel_B);
Encoder MotorPositionB(MotorB_Encoder_Channel_A, MotorB_Encoder_Channel_B);



//Create a structure for movement data
struct MovementParms
{
  signed int Position;
  signed int Velocity;
  signed int Acceleration;
  unsigned int PWM;
  bool Direction;
};

volatile struct MovementParms MotorA_Commanded;
volatile struct MovementParms MotorA_Measured;

volatile struct MovementParms MotorB_Commanded;
volatile struct MovementParms MotorB_Measured;


volatile bool RunTask_10ms = 0;

//**************************************
//  10 ms Task Rate
//  Creates a periodic Task 
//  Vector Address 15
//**************************************
ISR(TIMER0_COMPA_vect)
{
  //Check to see if the flag was not cleared, this will track 
  //overruns in the 10ms task
  if(RunTask_10ms)
  {
    //digitalWrite(LED_BUILTIN,1);
  }
  RunTask_10ms = 1;
}

//*********************************************
//  Configure Timer 0
//*********************************************
void SetupTimer0()
{
  TCCR0A = 0; // set entire TCCR0A register to 0
  TCCR0B = 0; // same for TCCR0B
  TCNT0  = 0; //initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 155;// = (16*10^6) / (100*1024) - 1 = 155(must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // 1024 prescaler - page 142 of datasheet
  TCCR0B |= (1 << CS00); 
  TCCR0B |= (1 << CS02);  
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}

void setup() 
{
  pinMode(JoyStickA, INPUT);

  init();

  pinMode(A5,INPUT_PULLUP);
  
  delay(50);

  //Disable interrupts
  cli();
  
  //Setup Timers
  SetupTimer0();

  //Start the serial port
  Serial.begin(115200);

  delay(1000);
  
  //enable interrupts
  sei();
}

void loop() 
{
  static int PreviousPositionA = 0;
  static int PreviousPositionB = 0;
  signed int JoystickADC;
  static int Counter = 0;
  
  if(RunTask_10ms)
  {
     //Read the encoder value
     MotorA_Measured.Position = MotorPositionA.read();
     MotorB_Measured.Position = MotorPositionB.read();

     MotorA_Measured.Velocity = MotorA_Measured.Position - PreviousPositionA;
     MotorB_Measured.Velocity = MotorB_Measured.Position - PreviousPositionB;
     
     //Read the Joystick input
     JoystickADC =  analogRead(JoyStickA); 
     JoystickADC = JoystickADC >> 2; 
     Counter++;

     //Output this to the serial monitor
     //Serial.print(",");
     //Serial.print(MotorA_Measured.Velocity);
     //Serial.print(",");
     //Serial.println(JoystickADC);
     //Serial.println(",");
     //Store the previous position
     PreviousPositionA = MotorA_Measured.Position;
     PreviousPositionB = MotorB_Measured.Position;
     
     //Process the output channels
    

     
     
     if(JoystickADC > 92)
     {
        digitalWrite(MotorB_DIR_ChannelA, 1);
        digitalWrite(MotorB_DIR_ChannelB, 0);
     }
     else
     {
        digitalWrite(MotorB_DIR_ChannelA, 0);
        digitalWrite(MotorB_DIR_ChannelB, 1);     
     }
     JoystickADC = 3* abs(JoystickADC - 92);

     if(JoystickADC > 255)
     {
        JoystickADC = 255;
     }
     //if(digitalRead(A5) == 0)

        Serial.println(JoystickADC);


     
     {
        analogWrite(MotorB_PWM_Channel , JoystickADC);
        analogWrite(MotorA_PWM_Channel , 200);
     }
     /*
     else
     {
       analogWrite(MotorB_PWM_Channel , 0);
       analogWrite(MotorA_PWM_Channel , 0);
     }
     */
     //Clear the flag that gets set by the interrupt
     //This was added at end to detect overruns of the 10ms task
     RunTask_10ms = 0;
  }
  else
  {
    //nop
  }
  
}
