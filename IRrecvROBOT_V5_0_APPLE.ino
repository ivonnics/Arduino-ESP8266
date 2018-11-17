/* Robot IR Remote Control
   by: José Luis Martínez
   
   date: December 26th, 2014

   This sketch uses Ken Shirriff's *awesome* IRremote library:
       https://github.com/shirriff/Arduino-IRremote

   RGB Remote Control uses an IR receiver diode (https://www.sparkfun.com/products/10266) to
   control a Radioshack MAKE Robot (http://www.radioshack.com/make-it-robotics-starter-kit/2770168.html#.VJ8mmpXVYA).

   The IR Remote's power button turns the LED 13 on or off. The up
   and down arrows moves the Robot Forward or Backwards.
   The left and right moves the Robot Left or Right, and
   Circle stops it.

   Hardware setup:
     * The output of an IR Receiver Diode (38 kHz demodulating
       version) should be connected to the Arduino's pin 11.
       * The IR Receiver diode should also be powered off the
         Arduino's 5V and GND rails.
     
 */

#include<MakeItRobotics.h> //include library
#define IDLE            0
#define STARTING        1
#define MOVING          2
#define DEFAULT_SPEED	150
#define IR_HIGH	        1
#define IR_LOW	        0
#define CYCLE  	        400
#define CYCLE_UPPER		401
#define CYCLE_LOWER		400
#define SPEED_LOWER_LIMIT	150
#define SPEED_UPPER_LIMIT	220
#define SLIDE_DURATION	5 

// --------------------------------------------------------------------------------------------
// ------ Fine-tune optical sensor stopping position (0-70)  																				-----
// ------ Default value is 10; larger value creates a larger offset from sensor
// --------------------------------------------------------------------------------------------

#define	left_stop_position	10	
#define right_stop_position	10	

MakeItRobotics Walkingrobot;//declare object

#include <IRremote.h> // Include the IRremote library

/* Setup constants for NET's IR Remote: */
#define NUM_BUTTONS 6 // The NET's IR remote control has many buttons, but we'll use only 6
/* Define the IR remote button codes. We're only using the
   least signinficant two bytes of these codes. Each one 
   should actually have 0xE17A in front of it. Find these codes
   by running the IRrecvDump example sketch included with
   the IRremote library.*/
const uint16_t BUTTON_POWER = 0x4050; // i.e. 0xE17A48B7
const uint16_t BUTTON_UP = 0xD050;
const uint16_t BUTTON_DOWN = 0xB050;
const uint16_t BUTTON_LEFT = 0x1050;
const uint16_t BUTTON_RIGHT = 0xE050;
const uint16_t BUTTON_CIRCLE = 0xBA50;

boolean ledstatus = false;

/* Connect the output of the IR receiver diode to pin 11. */
int RECV_PIN = 10;
/* Initialize the irrecv part of the IRremote  library */
IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0xA857; // This keeps track of the last code RX'd

void forward();
void backward();
void right();
void left();


void setup()
{
   Serial.begin(10420);                 //tell the Arduino to communicate with Make: it PCB
  Walkingrobot.walking_robot_setup();  //initialize the status of the walking robot
  Walkingrobot.all_stop();             //all motors stop
  delay(500);                          //delay 500ms
  irrecv.enableIRIn(); // Start the receiver
  pinMode(13,OUTPUT);
 }

// loop() constantly checks for any received IR codes. At the
// end it updates the last button pressed
void loop() 
{
  if (irrecv.decode(&results)) 
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a 
     button is held down. If we get 0xFFFFFFF, let's just
     assume the previously pressed button is being held down */
    if (resultCode == 0xFFFF)
      resultCode = lastCode;
    else
      lastCode = resultCode;

    // This switch statement checks the received IR code against
    // all of the known codes. Each button press produces a 
    // serial output, and has an effect on the output.
    switch (resultCode)
    {
      case BUTTON_POWER:
  
        ledstatus = 1^ledstatus;
  digitalWrite(13,ledstatus); //Testa a conexão e inverte o estado do LED ligado ao pino 13
        break;
      case BUTTON_UP:
               forward();
        
        break;
      case BUTTON_DOWN:
        
        backward();
        
        break;
      case BUTTON_LEFT:
        
        right();
        
        break;
      case BUTTON_RIGHT:
      
        left();
        
        break;
      case BUTTON_CIRCLE:
       Walkingrobot.all_stop();             //all motors stop
        
        break;
                  
    }    
    irrecv.resume(); // Receive the next value
  }
switch (lastCode) // Updates the last button pressed
    {
      case BUTTON_POWER:
  
        ledstatus = 1^ledstatus;
  digitalWrite(13,ledstatus); //Testa a conexão e inverte o estado do LED ligado ao pino 13
        break;
      case BUTTON_UP:
               forward();
        
        break;
      case BUTTON_DOWN:
        
        backward();
        
        break;
      case BUTTON_LEFT:
        
        right();
        
        break;
      case BUTTON_RIGHT:
      
        left();
        
        break;
      case BUTTON_CIRCLE:
       Walkingrobot.all_stop();             //all motors stop
        
        break;
  
}
}
void forward()
{
  static int sensor_in;                 //variable to store the value of read_optical function feedback   
  static int sensorCnt = 0;             //variable to count for trigger which optical
  static long sensorTimer1 = millis();  //last triggered time
  static long sensorTimer2 = millis();  //now time
  int leftIR_current;
  int left_status;
  long left_start_time;
  int left_speed;
  int left_duration;

  int rightIR_current;
  int right_status;
  long right_start_time;
  int right_speed;
  int right_duration;
  // ---------------------------------
  // ---------- Init. Motor ----------
  // ---------------------------------
  left_speed=DEFAULT_SPEED;
  right_speed=DEFAULT_SPEED;

  left_speed=150;  // default left leg on
  Walkingrobot.walking_robot_left_leg_forward(left_speed);
  left_status=STARTING;
  left_start_time = millis();

  right_speed=150;  // default right leg off
  Walkingrobot.walking_robot_right_leg_forward(0);
  right_status=IDLE;
	
  Walkingrobot.trigger_optical1();  // read optical sensor Rx1 first
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------  Main Loop ------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // - Left and right leg alternate movements
  // - Moving leg stops when the IR sensor detects the tape. Then the other leg starts moving.
  // - When sensor times out, assume the leg is not moving; increase driving force and retry the same leg.
  // - When cycle time is too long, assume speed is slow, increase driving force (PWM)
  // - When cycle time is too short, assuming speed is fast, decrease driving force (PWM)
  // ************************************************
  // ******** Read Optical Sensor from OpAmp ********
  // ************************************************
  while (1)
  {
    sensor_in=Walkingrobot.read_optical();
    
    if ((sensor_in & 0xf00)==0)
    {
      rightIR_current=sensor_in & 0xff;
      if (rightIR_current == 0x0)
        rightIR_current =IR_HIGH;
      else
        rightIR_current =IR_LOW;      
    }  
    else if((sensor_in & 0xf00)>>8==1) 
    {
      leftIR_current=sensor_in & 0xff;
      if (leftIR_current == 0x0)
        leftIR_current =IR_LOW;
      else
        leftIR_current =IR_HIGH;      
    }  
    sensorTimer2 = millis();                 //read now time
    if (sensorTimer2 - sensorTimer1 > 15)    //if now time minus last triggered time is greater than 15ms, then trigger the other optical sensor
    {
      sensorTimer1 = sensorTimer2;           //last triggered time = now time
      /***********************************************************************
         -> trigger optical -> greater than 15ms -> trigger optical2 -> greater than 15ms ->|
         |----------------------------------------------------------------------------------|
      ***********************************************************************/
      if (sensorCnt == 0)
        Walkingrobot.trigger_optical1();
      else if (sensorCnt == 1)
        Walkingrobot.trigger_optical2();
      sensorCnt++;
      if (sensorCnt == 2)
        sensorCnt = 0;
    }
    // ---------------------------------------------------------------------------------------------------
    // --------------------------------------------- Left Leg -------------------------------------------
    // ---------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************		
    left_duration= abs( millis() - left_start_time);// if duration too long without trigger => overshoot => stop and reduce speed

    if ( (left_duration > CYCLE*2) && (left_status !=IDLE) )
    {
      if (left_speed<(SPEED_UPPER_LIMIT+30))
      {
        left_speed=left_speed+30;	
	Walkingrobot.walking_robot_left_leg_forward(left_speed);
	left_status=STARTING;
	left_start_time = millis();				
       }
    }
    // ************************************
    // ********** Left IDLE State *********
    // ************************************
    if (left_status == IDLE)
    {
      // waiting for right leg
    }
    // ****************************************
    // ********** Left STARTING State ********
    // ****************************************
    else if (left_status == STARTING)
    {
      if (leftIR_current==IR_HIGH)// wait until left leg moves away from sensor
      { 
        left_status = MOVING;
      }
    }
    // **************************************
    // ********** Left MOVING State *********
    // **************************************
    else 																						// Moving now
    {
      if ((leftIR_current == IR_LOW) && (left_duration>200))// Optical Sensor Trigger
      {
        delay(SLIDE_DURATION);
        delay(left_stop_position);// trim stop position. fine-tune the left_stop_position on line 41
        Walkingrobot.walking_robot_left_leg_backward(150);// BRAKE 
	delay (10);// right leg				
        Walkingrobot.walking_robot_left_leg_backward(0);// Stop left leg
	left_status = IDLE;
	Walkingrobot.walking_robot_right_leg_forward(right_speed);// Start right leg
	right_status=STARTING;
	right_start_time = millis();
	if (left_duration > CYCLE_UPPER)// if duration too long but trigger => speed slow => increase PWM
	{
          if (left_speed<(SPEED_UPPER_LIMIT+20))
	    left_speed=left_speed+20;				
	}
	else if (left_duration < CYCLE_LOWER)// if duration too short but trigger => speed high => reduce PWM
	{
          if (left_speed>(SPEED_LOWER_LIMIT-10))
	    left_speed=left_speed-10;
	}
      }			
    }
    // ----------------------------------------------------------------------------------------------------
    // --------------------------------------------- Right Leg --------------------------------------------
    // ----------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************
    right_duration= abs( millis() - right_start_time);// if duration too long without trigger => overshoot => stop and reduce speed
    if ( (right_duration > CYCLE*2) && (right_status !=IDLE) )
    {
      if (right_speed<(SPEED_UPPER_LIMIT+30))
      {
        right_speed=right_speed+30;
	Walkingrobot.walking_robot_right_leg_forward(right_speed);
	right_status=STARTING;
	right_start_time = millis();				
      }		
    }
    // *************************************
    // ********** Right IDLE State **********
    // *************************************
    if (right_status == IDLE)
    {
      // waiting for left leg
    }
    // *****************************************
    // ********** RIGHT STARTING State *********
    // *****************************************
    else if (right_status == STARTING)
    {
      if (rightIR_current == IR_HIGH)// wait until right leg moves away from sensor
      { 
        right_status = MOVING;
      }
    }
    else 
    {
      if ((rightIR_current == IR_LOW) && (right_duration>200))// Optical Sensor Trigger
      {
        delay(SLIDE_DURATION);
	delay (right_stop_position);// trim stop position. fine-tune the right_stop_position on line 42
	Walkingrobot.walking_robot_right_leg_backward(150);													// BRAKE
	delay (10);	
	Walkingrobot.walking_robot_right_leg_backward(0);// Stop right leg
	right_status = IDLE;				
	Walkingrobot.walking_robot_left_leg_forward(left_speed);// Start left leg
	left_status=STARTING;
	left_start_time = millis();
	if (right_duration > CYCLE_UPPER)// if duration too long but trigger => speed slow => increase PWM
	{
	  if (right_speed<(SPEED_UPPER_LIMIT+20))
	    right_speed=right_speed+20;
        }
	else if (right_duration < CYCLE_LOWER)// if duration too short but trigger => speed high => reduce PWM
	{
	  if (right_speed>(SPEED_LOWER_LIMIT-10))
	    right_speed=right_speed-10;
	}
 Walkingrobot.all_stop();             //all motors stop
  break;
      }
     
    }
      }  
}
void backward()
{
  static int sensor_in;                 //variable to store the value of read_optical function feedback   
  static int sensorCnt = 0;             //variable to count for trigger which optical
  static long sensorTimer1 = millis();  //last triggered time
  static long sensorTimer2 = millis();  //now time
  int leftIR_current;
  int left_status;
  long left_start_time;
  int left_speed;
  int left_duration;

  int rightIR_current;
  int right_status;
  long right_start_time;
  int right_speed;
  int right_duration;
  // ---------------------------------
  // ---------- Init. Motor ----------
  // ---------------------------------
  left_speed=DEFAULT_SPEED;
  right_speed=DEFAULT_SPEED;

  left_speed=150;  // default left leg on
  
  left_status=STARTING;
  left_start_time = millis();

  right_speed=150;  // default right leg off
  Walkingrobot.walking_robot_right_leg_forward(0);
  right_status=IDLE;
	
  Walkingrobot.trigger_optical1();  // read optical sensor Rx1 first
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------  Main Loop ------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // - Left and right leg alternate movements
  // - Moving leg stops when the IR sensor detects the tape. Then the other leg starts moving.
  // - When sensor times out, assume the leg is not moving; increase driving force and retry the same leg.
  // - When cycle time is too long, assume speed is slow, increase driving force (PWM)
  // - When cycle time is too short, assuming speed is fast, decrease driving force (PWM)
  // ************************************************
  // ******** Read Optical Sensor from OpAmp ********
  // ************************************************
  while (1)
  {
    sensor_in=Walkingrobot.read_optical();
    
    if ((sensor_in & 0xf00)==0)
    {
      rightIR_current=sensor_in & 0xff;
      if (rightIR_current == 0x0)
        rightIR_current =IR_HIGH;
      else
        rightIR_current =IR_LOW;      
    }  
    else if((sensor_in & 0xf00)>>8==1) 
    {
      leftIR_current=sensor_in & 0xff;
      if (leftIR_current == 0x0)
        leftIR_current =IR_LOW;
      else
        leftIR_current =IR_HIGH;      
    }  
    sensorTimer2 = millis();                 //read now time
    if (sensorTimer2 - sensorTimer1 > 15)    //if now time minus last triggered time is greater than 15ms, then trigger the other optical sensor
    {
      sensorTimer1 = sensorTimer2;           //last triggered time = now time
      /***********************************************************************
         -> trigger optical -> greater than 15ms -> trigger optical2 -> greater than 15ms ->|
         |----------------------------------------------------------------------------------|
      ***********************************************************************/
      if (sensorCnt == 0)
        Walkingrobot.trigger_optical1();
      else if (sensorCnt == 1)
        Walkingrobot.trigger_optical2();
      sensorCnt++;
      if (sensorCnt == 2)
        sensorCnt = 0;
    }
    // ---------------------------------------------------------------------------------------------------
    // --------------------------------------------- Left Leg -------------------------------------------
    // ---------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************		
    left_duration= abs( millis() - left_start_time);// if duration too long without trigger => overshoot => stop and reduce speed

    if ( (left_duration > CYCLE*2) && (left_status !=IDLE) )
    {
      if (left_speed<(SPEED_UPPER_LIMIT+30))
      {
        left_speed=left_speed+30;	
	Walkingrobot.walking_robot_left_leg_backward(left_speed);
	left_status=STARTING;
	left_start_time = millis();				
       }
    }
    // ************************************
    // ********** Left IDLE State *********
    // ************************************
    if (left_status == IDLE)
    {
      // waiting for right leg
    }
    // ****************************************
    // ********** Left STARTING State ********
    // ****************************************
    else if (left_status == STARTING)
    {
      if (leftIR_current==IR_HIGH)// wait until left leg moves away from sensor
      { 
        left_status = MOVING;
      }
    }
    // **************************************
    // ********** Left MOVING State *********
    // **************************************
    else 																						// Moving now
    {
      if ((leftIR_current == IR_LOW) && (left_duration>200))// Optical Sensor Trigger
      {
        delay(SLIDE_DURATION);
        delay(left_stop_position);// trim stop position. fine-tune the left_stop_position on line 41
        Walkingrobot.walking_robot_left_leg_forward(150);// BRAKE 
	delay (10);// right leg				
        Walkingrobot.walking_robot_left_leg_forward(0);// Stop left leg
	left_status = IDLE;
	Walkingrobot.walking_robot_right_leg_backward(right_speed);// Start right leg
	right_status=STARTING;
	right_start_time = millis();
	if (left_duration > CYCLE_UPPER)// if duration too long but trigger => speed slow => increase PWM
	{
          if (left_speed<(SPEED_UPPER_LIMIT+20))
	    left_speed=left_speed+20;				
	}
	else if (left_duration < CYCLE_LOWER)// if duration too short but trigger => speed high => reduce PWM
	{
          if (left_speed>(SPEED_LOWER_LIMIT-10))
	    left_speed=left_speed-10;
	}
      }			
    }
    // ----------------------------------------------------------------------------------------------------
    // --------------------------------------------- Right Leg --------------------------------------------
    // ----------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************
    right_duration= abs( millis() - right_start_time);// if duration too long without trigger => overshoot => stop and reduce speed
    if ( (right_duration > CYCLE*2) && (right_status !=IDLE) )
    {
      if (right_speed<(SPEED_UPPER_LIMIT+30))
      {
        right_speed=right_speed+30;
	Walkingrobot.walking_robot_right_leg_backward(right_speed);
	right_status=STARTING;
	right_start_time = millis();				
      }		
    }
    // *************************************
    // ********** Right IDLE State **********
    // *************************************
    if (right_status == IDLE)
    {
      // waiting for left leg
    }
    // *****************************************
    // ********** RIGHT STARTING State *********
    // *****************************************
    else if (right_status == STARTING)
    {
      if (rightIR_current == IR_HIGH)// wait until right leg moves away from sensor
      { 
        right_status = MOVING;
      }
    }
    else 
    {
      if ((rightIR_current == IR_LOW) && (right_duration>200))// Optical Sensor Trigger
      {
        delay(SLIDE_DURATION);
	delay (right_stop_position);// trim stop position. fine-tune the right_stop_position on line 42
	Walkingrobot.walking_robot_right_leg_forward(150);													// BRAKE
	delay (10);	
	Walkingrobot.walking_robot_right_leg_forward(0);// Stop right leg
	right_status = IDLE;				
	Walkingrobot.walking_robot_left_leg_backward(left_speed);// Start left leg
	left_status=STARTING;
	left_start_time = millis();
	if (right_duration > CYCLE_UPPER)// if duration too long but trigger => speed slow => increase PWM
	{
	  if (right_speed<(SPEED_UPPER_LIMIT+20))
	    right_speed=right_speed+20;
        }
	else if (right_duration < CYCLE_LOWER)// if duration too short but trigger => speed high => reduce PWM
	{
	  if (right_speed>(SPEED_LOWER_LIMIT-10))
	    right_speed=right_speed-10;
	}
Walkingrobot.all_stop();             //all motors stop
  break;  
      }
   
     }
    
  }
 }
void right()
{
  static int sensor_in;                 //variable to store the value of read_optical function feedback   
  static int sensorCnt = 0;             //variable to count for trigger which optical
  static long sensorTimer1 = millis();  //last triggered time
  static long sensorTimer2 = millis();  //now time
  int leftIR_current;
  int left_status;
  long left_start_time;
  int left_speed;
  int left_duration;

  int rightIR_current;
  int right_status;
  long right_start_time;
  int right_speed;
  int right_duration;
  // ---------------------------------
  // ---------- Init. Motor ----------
  // ---------------------------------
  left_speed=DEFAULT_SPEED;
  right_speed=DEFAULT_SPEED;

  left_speed=150;  // default left leg on
  Walkingrobot.walking_robot_right_leg_forward(left_speed);
  left_status=STARTING;
  left_start_time = millis();

  right_speed=150;  // default right leg off
  Walkingrobot.walking_robot_left_leg_forward(0);
  right_status=IDLE;
	
  Walkingrobot.trigger_optical1();  // read optical sensor Rx1 first
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------  Main Loop ------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // - Left and right leg alternate movements
  // - Moving leg stops when the IR sensor detects the tape. Then the other leg starts moving.
  // - When sensor times out, assume the leg is not moving; increase driving force and retry the same leg.
  // - When cycle time is too long, assume speed is slow, increase driving force (PWM)
  // - When cycle time is too short, assuming speed is fast, decrease driving force (PWM)
  // ************************************************
  // ******** Read Optical Sensor from OpAmp ********
  // ************************************************
  while (1)
  {break;
    sensor_in=Walkingrobot.read_optical();
    
    if ((sensor_in & 0xf00)==0)
    {
      rightIR_current=sensor_in & 0xff;
      if (rightIR_current == 0x0)
        rightIR_current =IR_HIGH;
      else
        rightIR_current =IR_LOW;      
    }  
    else if((sensor_in & 0xf00)>>8==1) 
    {
      leftIR_current=sensor_in & 0xff;
      if (leftIR_current == 0x0)
        leftIR_current =IR_LOW;
      else
        leftIR_current =IR_HIGH;      
    }  
    sensorTimer2 = millis();                 //read now time
    if (sensorTimer2 - sensorTimer1 > 15)    //if now time minus last triggered time is greater than 15ms, then trigger the other optical sensor
    {
      sensorTimer1 = sensorTimer2;           //last triggered time = now time
      /***********************************************************************
         -> trigger optical -> greater than 15ms -> trigger optical2 -> greater than 15ms ->|
         |----------------------------------------------------------------------------------|
      ***********************************************************************/
      if (sensorCnt == 0)
        Walkingrobot.trigger_optical1();
      else if (sensorCnt == 1)
        Walkingrobot.trigger_optical2();
      sensorCnt++;
      if (sensorCnt == 2)
        sensorCnt = 0;
    }
    // ---------------------------------------------------------------------------------------------------
    // --------------------------------------------- Left Leg -------------------------------------------
    // ---------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************		
    left_duration= abs( millis() - left_start_time);// if duration too long without trigger => overshoot => stop and reduce speed

    if ( (left_duration > CYCLE*2) && (left_status !=IDLE) )
    {
      if (left_speed<(SPEED_UPPER_LIMIT+30))
      {
        left_speed=left_speed+30;	
	Walkingrobot.walking_robot_right_leg_forward(left_speed);
	left_status=STARTING;
	left_start_time = millis();				
       }
    }
    // ************************************
    // ********** Left IDLE State *********
    // ************************************
    if (left_status == IDLE)
    {
      // waiting for right leg
    }
    // ****************************************
    // ********** Left STARTING State ********
    // ****************************************
    else if (left_status == STARTING)
    {
      if (leftIR_current==IR_HIGH)// wait until left leg moves away from sensor
      { 
        left_status = MOVING;
      }
    }
    // **************************************
    // ********** Left MOVING State *********
    // **************************************
    else 																						// Moving now
    {
      if ((leftIR_current == IR_LOW) && (left_duration>200))// Optical Sensor Trigger
      {
        delay(SLIDE_DURATION);
        delay(left_stop_position);// trim stop position. fine-tune the left_stop_position on line 41
        Walkingrobot.walking_robot_right_leg_backward(150);// BRAKE 
	delay (10);// right leg				
        Walkingrobot.walking_robot_right_leg_backward(0);// Stop left leg
	left_status = IDLE;
	Walkingrobot.walking_robot_left_leg_forward(right_speed);// Start right leg
	right_status=STARTING;
	right_start_time = millis();
	if (left_duration > CYCLE_UPPER)// if duration too long but trigger => speed slow => increase PWM
	{
          if (left_speed<(SPEED_UPPER_LIMIT+20))
	    left_speed=left_speed+20;				
	}
	else if (left_duration < CYCLE_LOWER)// if duration too short but trigger => speed high => reduce PWM
	{
          if (left_speed>(SPEED_LOWER_LIMIT-10))
	    left_speed=left_speed-10;
	}

      }	
		
    }
    // ----------------------------------------------------------------------------------------------------
    // --------------------------------------------- Right Leg --------------------------------------------
    // ----------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************
   
  }   
}  
void left()
{
  static int sensor_in;                 //variable to store the value of read_optical function feedback   
  static int sensorCnt = 0;             //variable to count for trigger which optical
  static long sensorTimer1 = millis();  //last triggered time
  static long sensorTimer2 = millis();  //now time
  int leftIR_current;
  int left_status;
  long left_start_time;
  int left_speed;
  int left_duration;

  int rightIR_current;
  int right_status;
  long right_start_time;
  int right_speed;
  int right_duration;
  // ---------------------------------
  // ---------- Init. Motor ----------
  // ---------------------------------
  left_speed=DEFAULT_SPEED;
  right_speed=DEFAULT_SPEED;

  left_speed=150;  // default left leg on
  Walkingrobot.walking_robot_left_leg_forward(left_speed);
  left_status=STARTING;
  left_start_time = millis();

  right_speed=150;  // default right leg off
  Walkingrobot.walking_robot_right_leg_forward(0);
  right_status=IDLE;
	
  Walkingrobot.trigger_optical1();  // read optical sensor Rx1 first
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------  Main Loop ------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------------------------
  // - Left and right leg alternate movements
  // - Moving leg stops when the IR sensor detects the tape. Then the other leg starts moving.
  // - When sensor times out, assume the leg is not moving; increase driving force and retry the same leg.
  // - When cycle time is too long, assume speed is slow, increase driving force (PWM)
  // - When cycle time is too short, assuming speed is fast, decrease driving force (PWM)
  // ************************************************
  // ******** Read Optical Sensor from OpAmp ********
  // ************************************************
  while (1)
  {break;
    sensor_in=Walkingrobot.read_optical();
    
    if ((sensor_in & 0xf00)==0)
    {
      rightIR_current=sensor_in & 0xff;
      if (rightIR_current == 0x0)
        rightIR_current =IR_HIGH;
      else
        rightIR_current =IR_LOW;      
    }  
    else if((sensor_in & 0xf00)>>8==1) 
    {
      leftIR_current=sensor_in & 0xff;
      if (leftIR_current == 0x0)
        leftIR_current =IR_LOW;
      else
        leftIR_current =IR_HIGH;      
    }  
    sensorTimer2 = millis();                 //read now time
    if (sensorTimer2 - sensorTimer1 > 15)    //if now time minus last triggered time is greater than 15ms, then trigger the other optical sensor
    {
      sensorTimer1 = sensorTimer2;           //last triggered time = now time
      /***********************************************************************
         -> trigger optical -> greater than 15ms -> trigger optical2 -> greater than 15ms ->|
         |----------------------------------------------------------------------------------|
      ***********************************************************************/
      if (sensorCnt == 0)
        Walkingrobot.trigger_optical1();
      else if (sensorCnt == 1)
        Walkingrobot.trigger_optical2();
      sensorCnt++;
      if (sensorCnt == 2)
        sensorCnt = 0;
    }
    // ---------------------------------------------------------------------------------------------------
    // --------------------------------------------- Left Leg -------------------------------------------
    // ---------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************		
    left_duration= abs( millis() - left_start_time);// if duration too long without trigger => overshoot => stop and reduce speed

    if ( (left_duration > CYCLE*2) && (left_status !=IDLE) )
    {
      if (left_speed<(SPEED_UPPER_LIMIT+30))
      {
        left_speed=left_speed+30;	
	Walkingrobot.walking_robot_left_leg_forward(left_speed);
	left_status=STARTING;
	left_start_time = millis();				
       }
    }
    // ************************************
    // ********** Left IDLE State *********
    // ************************************
    if (left_status == IDLE)
    {
      // waiting for right leg
    }
    // ****************************************
    // ********** Left STARTING State ********
    // ****************************************
    else if (left_status == STARTING)
    {
      if (leftIR_current==IR_HIGH)// wait until left leg moves away from sensor
      { 
        left_status = MOVING;
      }
    }
    // **************************************
    // ********** Left MOVING State *********
    // **************************************
    else 																						// Moving now
    {
      if ((leftIR_current == IR_LOW) && (left_duration>200))// Optical Sensor Trigger
      {
        delay(SLIDE_DURATION);
        delay(left_stop_position);// trim stop position. fine-tune the left_stop_position on line 41
        Walkingrobot.walking_robot_left_leg_backward(150);// BRAKE 
	delay (10);// right leg				
        Walkingrobot.walking_robot_left_leg_backward(0);// Stop left leg
	left_status = IDLE;
	Walkingrobot.walking_robot_right_leg_forward(right_speed);// Start right leg
	right_status=STARTING;
	right_start_time = millis();
	if (left_duration > CYCLE_UPPER)// if duration too long but trigger => speed slow => increase PWM
	{
          if (left_speed<(SPEED_UPPER_LIMIT+20))
	    left_speed=left_speed+20;				
	}
	else if (left_duration < CYCLE_LOWER)// if duration too short but trigger => speed high => reduce PWM
	{
          if (left_speed>(SPEED_LOWER_LIMIT-10))
	    left_speed=left_speed-10;
	}

      }	
		
    }
    // ----------------------------------------------------------------------------------------------------
    // --------------------------------------------- Right Leg --------------------------------------------
    // ----------------------------------------------------------------------------------------------------
    // ***********************************
    // ********** Check Duration *********
    // ***********************************
   
  }   
}

