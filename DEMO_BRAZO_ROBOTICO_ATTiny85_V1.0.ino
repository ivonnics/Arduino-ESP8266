#include <SoftwareSerial.h>

/*Recuerda que debe ser programado con el "Clock Internal: 8 MHz" Setting, para que funcione el puerto Serial!!!! 
 * 
 */


#define RX    3   // *** D3, Pin 2
#define TX    4   // *** D4, Pin 3



SoftwareSerial MySerial(RX, TX);

void setup() {

pinMode(RX, INPUT);

pinMode(TX, OUTPUT);
 MySerial.begin(9600);
  delay(10);

}

void loop() {
  
  
  
    MySerial.println("#31 P1300 #27 P1150 #3 P1430 #7 P2200 #11 P1550 #15 P1800 T5000");
    delay (3000);
    MySerial.println("#3 P650 T2000");
    delay (3000);
    MySerial.println("#3 P1430 T2000");
    delay (3000);
    MySerial.println("#3 P2200 T2000");
    delay (3000);
    MySerial.println("#7 P1500 T2000");
    delay (3000);
    MySerial.println("#7 P850 T2000");
    delay (3000);
    MySerial.println("#7 P1500 T2000");
    delay (3000);
    MySerial.println("#11 P700 T2000");
    delay (3000);
    MySerial.println("#11 P1550 T2000");
    delay (3000);
    MySerial.println("#11 P2400 T2000");
    delay (3000);
    MySerial.println("#11 P1550 T2000");
    delay (3000);
    MySerial.println("#15 P2250 T2000");
    delay (3000);
    MySerial.println("#15 P1800 T2000");
    delay (3000);
    
    delay (3000);
    MySerial.println("#27 P1950 #3 P650 #7 P2200 T3000");
    delay (3000);
    MySerial.println("#27 P1150 #3 P1430 #7 P1550 T3000");
    delay (3000);


    MySerial.println("#31 P1300 #27 P1150 #3 P1430 #7 P2200 #11 P1550 #15 P1800 T5000");
    delay (3000);
    MySerial.println("#3 P650 T2000");
    delay (3000);
    MySerial.println("#3 P1430 T2000");
    delay (3000);
    MySerial.println("#3 P2200 T2000");
    delay (3000);
    MySerial.println("#7 P1500 T2000");
    delay (3000);
    MySerial.println("#7 P850 T2000");
    delay (3000);
    MySerial.println("#7 P1500 T2000");
    delay (3000);
    MySerial.println("#11 P700 T2000");
    delay (3000);
    MySerial.println("#11 P1550 T2000");
    delay (3000);
    MySerial.println("#11 P2400 T2000");
    delay (3000);
    MySerial.println("#11 P1550 T2000");
    delay (3000);
    MySerial.println("#15 P2250 T2000");
    delay (3000);
    MySerial.println("#15 P1800 T2000");
    delay (3000);
    
    delay (3000);
    MySerial.println("#27 P550 #3 P650 #7 P2200 T3000");
    delay (3000);
    MySerial.println("#27 P1150 #3 P1430 #7 P1550 T3000");
    delay (3000);
    

}
