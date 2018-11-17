#include <DualMotor.h> //Includes DualMotor library

DualMotor dualmotor; //Calls DualMotor

#include <IRremote.h> // Include the IRremote library

/* Setup constants for Apple TV's IR Remote: */
#define NUM_BUTTONS 6 // The Apple TV IR remote control has seven buttons, but we'll use only six
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

/* Connect the output of the IR receiver diode to pin 10. */
int RECV_PIN = 10;
/* Initialize the irrecv part of the IRremote  library */
IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0xBA50; // This keeps track of the last code RX'd
uint16_t resultCode = lastCode;

void forward();
void backward();
void right();
void left();
void stope();


void setup()
{
   dualmotor.M1parar(); // Stops Motor 1
    dualmotor.M2parar(); //Stops Motor 2                     
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
    if (resultCode == 0xFFFF){
      resultCode = lastCode;}
    else{
      lastCode = resultCode;}

    // This switch statement checks the received IR code against
    // all of the known codes. Each button press produces a 
    // different movement in the car.
    switch (resultCode)
    {
      case BUTTON_POWER:
  
        ledstatus = 1^ledstatus;
  digitalWrite(13,ledstatus); //Test connection and inverts status of LED 13th
        break;
      case BUTTON_UP:
               forward();
        
        break;
      case BUTTON_DOWN:
        
        backward();
        
        break;
      case BUTTON_LEFT:
        
        left();
        
        break;
      case BUTTON_RIGHT:
      
        right();
        
        break;
      case BUTTON_CIRCLE:

        stope();
        break;
        
  
    }    
   irrecv.resume(); // Receive the next value
  }
switch (lastCode) // Updates the last button pressed
    {
      case BUTTON_POWER:
  
        
  digitalWrite(13,ledstatus); //Updates the status of LED 13th
        break;
      case BUTTON_UP:
               forward();
        
        break;
      case BUTTON_DOWN:
        
        backward();
        
        break;
      case BUTTON_LEFT:
        
        left();
        
        break;
      case BUTTON_RIGHT:
      
        right();
        
        break;
      case BUTTON_CIRCLE:
      
      stope();  
        break;
  
}
}
void forward()
{
 dualmotor.M1move(255,1);                   //Motor 1 forward
      dualmotor.M2move(255,1);              //Motor 2 forward
}
void backward()
{
  dualmotor.M1move(255,0);                  //Motor 1 backwards
      dualmotor.M2move(255,0);              //Motor 2 backwards
  }  
void left()
{
    dualmotor.M1move(255,0);                //Motor 1 backwards
      dualmotor.M2move(255,1);              //Motor 2 forward
}
void right()
{
   dualmotor.M1move(255,1);                 //Motor 1 forward
      dualmotor.M2move(255,0);              //Motor 2 backwards
}
void stope()
{
    dualmotor.M1parar(); // Stops Motor 1
    dualmotor.M2parar(); //Stops Motor 2
}
