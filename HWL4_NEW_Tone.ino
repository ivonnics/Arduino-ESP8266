void setup() {
 Serial.begin(9600);
pinMode(11, OUTPUT);
 
}

void loop() {

for (int i=0; i <= 4; i++){
     
   

  Serial.println("Tone sending...");
 //tone(pin,frequency,durantion);
 tone(11, 200,1500);  //play note for 1500ms
 delay(1500);
 Serial.println("Tone done");
 delay(500);
} 
while(1);
}
