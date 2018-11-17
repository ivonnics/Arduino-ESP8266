

/*
 * WiFiShield Lab
 * Baseado no exemplo httpserver da biblioteca WiFlyHQmaster
 */

#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 
#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <SD.h>



 #include "DHT.h"
 #define DHTPIN 7 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados)
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);

#include "Wire.h"
 
 const int chipSelecta = 4;
 
 unsigned long previousMillis = 0;        // will store last time SD was written

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
const unsigned long interval = 60000;           // interval at which SD will be written (milliseconds)

  unsigned long previousMillisTW = 0;        // will store last time STWITTER was sent

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
const unsigned long intervalTW = 3600000;           // interval at which TWITTER will be sent (milliseconds)

 const char mySSID[] = "NETGEAR50"; // Wifi network name 
 const char myPassword[] = "gracefulpond393"; // Wifi network password 
 char token[] = "3344962781-iDl7UNowAwTLoRJcAkdVEqIXmGpEZE4x8EJR0is"; // Get this from arduino-tweet.appspot.com 
   
 // Fixed settings: 
 const char site[] = "arduino-tweet.appspot.com"; // Site that sends our tweets 
 
  char charah[10];               
 char charat[10]; 
char charaa[10];
char charap[10];
char charay[10];

 char charatw[10]; 
 char charahw[10]; 
 
char charaaw[10];
char charapw[10];

char msg[140];  //tweet length
String tweetText; // Variable to hold the tweet text
 
Adafruit_BMP085 bmp; 
 
             #define DS3231_I2C_ADDRESS 0x68

// Convert normal decimal numbers to binary coded decimal

byte decToBcd(byte val)

{

  return( (val/10*16) + (val%10) );

}

// Convert binary coded decimal to normal decimal numbers

byte bcdToDec(byte val)

{

  return( (val/16*10) + (val%16) );

}
 



SoftwareSerial wifiSerial(10,11);

WiFly wifly;



char buf[80];

// Altere as duas variáveis abaixo de acordo com sua rede Wifi 
const char rede[] = "NETGEAR50";
const char senha[] = "gracefulpond393";

void setup()
{
   Serial.begin(9600);
   
   
    if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
   
  Serial.println(F("Inicializando"));
  Serial.print(F("Memoria livre: "));
  Serial.println(wifly.getFreeMemory(),DEC);

  wifiSerial.begin(9600);
  if (!wifly.begin(&wifiSerial, &Serial)) {
    Serial.println(F("Falha ao inicializar o wifly"));
    wifly.terminal();
  }

  //Se não estiver associado a uma rede configura a rede no módulo
  if (!wifly.isAssociated()) 
  {
    Serial.println(F("Conectando..."));
    wifly.setSSID(rede);
    wifly.setPassphrase(senha);
    wifly.enableDHCP();
    wifly.save();

    if (wifly.join()) 
    {
      Serial.println(F("Conectado a Rede"));
    } 
    else 
    {
      Serial.println(F("Falha ao conectar na rede"));
      wifly.terminal();
    }
  } 
  else 
  {
    Serial.println(F("Pronto para entrar na rede"));
  }

  wifly.setBroadcastInterval(0);	// Desliga UPD broadcast

  Serial.print(F("MAC: "));
  Serial.println(wifly.getMAC(buf, sizeof(buf)));
  Serial.print(F("IP: "));
  Serial.println(wifly.getIP(buf, sizeof(buf)));
  Serial.print("Netmask: ");
    Serial.println(wifly.getNetmask(buf, sizeof(buf)));
    Serial.print("Gateway: ");
    Serial.println(wifly.getGateway(buf, sizeof(buf)));
  wifly.setDeviceID("ShieldWiFi");

  if (wifly.isConnected()) 
  {
    Serial.println(F("Fechando outras conexões ativas"));
    wifly.close();
  }

  wifly.setProtocol(WIFLY_PROTOCOL_TCP);
  if (wifly.getPort() != 80) 
  {
    wifly.setPort(80);
    wifly.save();
    Serial.println(F("Trocando para a porta 80, reiniciando..."));
    wifly.reboot();    // Reiniciar o módulo para alterar a número da porta
    delay(3000);
  }
  Serial.println(F("Pronto"));
  
     // set the initial time here:

  // DS3231 seconds, minutes, hours, day, date, month, year

  // setDS3231time(30,42,21,4,26,11,14);
  
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
   Serial.print("Initializing SD card...");

   // see if the card is present and can be initialized:

   if (!SD.begin(chipSelecta)) {
     Serial.println("Card failed, or not present");
     // don't do anything more:
     return;
   }
   Serial.println("card initialized.");
   
   File logFile = SD.open("LOGTEMP.csv", FILE_WRITE);

   // if the file is available, write to it:
   if (logFile) {
     logFile.println(",,,,"); //Just a leading blank line, incase there was previous data
     String header = "DATE, TIME, TEMPERATURA H (C), TEMPERATURA P (C), HUMEDAD (%), PRESION (Pa), ALTITUDE (m)";
     logFile.println(header);
     logFile.close();
     Serial.println(header);

}
  
}


void readDS3231time(byte *second,

byte *minute,

byte *hour,

byte *dayOfWeek,

byte *dayOfMonth,

byte *month,

byte *year)

{

  Wire.beginTransmission(DS3231_I2C_ADDRESS);

  Wire.write(0); // set DS3231 register pointer to 00h

  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

  // request seven bytes of data from DS3231 starting from register 00h

  *second = bcdToDec(Wire.read() & 0x7f);

  *minute = bcdToDec(Wire.read());

  *hour = bcdToDec(Wire.read() & 0x3f);

  *dayOfWeek = bcdToDec(Wire.read());

  *dayOfMonth = bcdToDec(Wire.read());

  *month = bcdToDec(Wire.read());

  *year = bcdToDec(Wire.read());

}



void loop()
{
  
  

  
  if (wifly.available() > 0) 
  {
    if (wifly.gets(buf, sizeof(buf))) 
    {
      if (strncmp_P(buf, PSTR("GET / "), 6) == 0) 
        //Verifica se é um GET na página Inicial - 192.168.1.14/
      {
        while (wifly.gets(buf, sizeof(buf)) > 0) 
        {
          // Ignora o restante da requisição
        }
                inicio();
                 // Pula para a rotina que imprime a página Inicial y lleva el Brazo a su Posición inicial
      }
             else 
      {
        wifly.flushRx();	// Descarta o restante da transmissão
        erro404();              //Envia página de erro - link não existe
      }
    }
  }
  
  dht.begin();
  
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time the SD was written 
    previousMillis = currentMillis;   

     byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,

  &year);
  
  float h = dht.readHumidity();
float hh=h;
dtostrf(hh, 2, 0, charah);
  
float t = dht.readTemperature();
float tt=t;
dtostrf(tt, 2, 0, charat);

float a = bmp.readAltitude();
float aa=a;
dtostrf(aa, 2, 0, charaa);

float p = bmp.readPressure();
float pp=p;
dtostrf(pp, 2, 0, charap);

float y = bmp.readTemperature();
float yy=y;
dtostrf(yy, 2, 0, charay);


// testa se retorno é valido, caso contrário algo está errado.
if (isnan(t) || isnan(h))
{
Serial.println("Failed to read from DHT");
}
else
{

  
  
  String dataString = String(dayOfMonth) + "/" +  String(month) + "/" + String(year) + "," + String(hour) + ":" + String(minute) + "," + String(charat) + "," + String(charay) + "," + String(charah) + "," + String(charap) + "," + String(charaa);
   
   File logFile = SD.open("LOGTEMP.csv", FILE_WRITE);

   // if the file is available, write to it:
   if (logFile) {
     logFile.println(dataString);
     logFile.close();
     Serial.println(dataString);

}

// if the file isn't open, pop up an error:
   else {
     Serial.println("error opening LOGTEMP.csv");
   }

}
  
   
   
  }
  
  //Comenzamos twitter
  
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,

  &year);
  
  float h = dht.readHumidity();
float hh=h;
dtostrf(hh, 2, 0, charah);
  
float t = dht.readTemperature();
float tt=t;
dtostrf(tt, 2, 0, charat);

float a = bmp.readAltitude();
float aa=a;
dtostrf(aa, 2, 0, charaa);

float p = bmp.readPressure();
float pp=p;
dtostrf(pp, 2, 0, charap);

float y = bmp.readTemperature();
float yy=y;
dtostrf(yy, 2, 0, charay);


// testa se retorno é valido, caso contrário algo está errado.
if (isnan(t) || isnan(h))
{
Serial.println("Failed to read from DHT");
}
else
{

  
  
  String dataString = String(dayOfMonth) + "/" +  String(month) + "/" + String(year) + "," + String(hour) + ":" + String(minute) + "," + String(charat) + "," + String(charay) + "," + String(charah) + "," + String(charap) + "," + String(charaa);

  
   unsigned long currentMillisTW = millis();
 
  if(currentMillisTW - previousMillisTW > intervalTW) {
    // save the last time the SD was written 
    previousMillisTW = currentMillisTW;  
    
    // Message to post
tweetText = dataString + ",,," + millis(); // Construct the tweet; random because Twitter doesn't like to see the same thing repeatedly 
        
 tweet(); // THIS IS THE LINE THAT TWEETS: details below 

    

}

  
  
  
}}

// Página Posición Inicial
void inicio()
{

 webpage();
  
}



void webpage()
{
  
   wifly.println(F("HTTP/1.1 200 OK"));
  wifly.println(F("Content-Type: text/html"));
  wifly.println(F("Transfer-Encoding: chunked"));
  wifly.println();
  
  
  
  wifly.sendChunkln(F("<html>"));
  wifly.sendChunkln(F("<title>Sensors</title>"));
   wifly.sendChunkln(F("<body bgcolor = blue>"));
    
int tw = dht.readTemperature();
dtostrf(tw, 2, 0, charatw);
int hw = dht.readHumidity();
dtostrf(hw, 2, 0, charahw);

float pw = bmp.readPressure();
int pww = bmp.readPressure();// ESTO NO SIRVE!!!
long pwww = bmp.readPressure();
dtostrf(pw, 2, 0, charapw);
String dataStringpw = String(charapw) + " Pa";

float aw = bmp.readAltitude();
dtostrf(aw, 2, 0, charaaw);

  wifly.sendChunkln(F("<BR><center></B></I></U><a href=\"http://www.stnicholas.com.br\"> <img src=http://i.huffpost.com/gen/1649894/images/o-AMERICAN-FLAG-facebook.jpg width = 700 height = 350 ></a></B></I></U> </center>"));

  wifly.sendChunk(F("<h1><p><center>TEMPERATURE (in C) is = "));
    wifly.sendChunk(charatw);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</center></p></h1>"));
    
    wifly.sendChunk(F("<h1><p><center>HUMIDITY (in %) is = "));
    wifly.sendChunk(charahw);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</center></p></h1>"));
    
    wifly.sendChunk(F("<h1><p><center>PRESSURE (in Pa) is = "));
    wifly.sendChunk(charapw);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</center></p></h1>"));
  
   wifly.sendChunk(F("<h1><p><center>ALTITUDE (in m) is = "));
    wifly.sendChunk(charaaw);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</center></p></h1>"));
    
    wifly.sendChunk(F("<h1><p><center>Millis is (in mseg) = "));
    wifly.sendChunk(millis);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</center></p></h1>"));
  
   wifly.sendChunk(F("<h4><p><left>P&aacute;gina desarrollada por Jos&eacute; Luis Mart&iacute;nez."));
   wifly.sendChunkln(F("</leftt></p></h4>"));
   
   wifly.sendChunkln(F("<BR><center></B></I></U><a href=\"http://www.stnicholas.com.br\"> <img src=https://upload.wikimedia.org/wikipedia/commons/thumb/5/5b/Greater_coat_of_arms_of_the_United_States.svg/451px-Greater_coat_of_arms_of_the_United_States.svg.png width = 300 height = 310></a></B></I></U> </center>"));
   
   
  

   wifly.sendChunkln(F("</body>"));
  wifly.sendChunkln(F("</html>"));
  wifly.sendChunkln();
  }
  
    // Envia página de erro caso seja digitado uma URL inválida
void erro404()
{
    wifly.println(F("HTTP/1.1 200 OK"));
  wifly.println(F("Content-Type: text/html"));
  wifly.println(F("Transfer-Encoding: chunked"));
  wifly.println();
  wifly.sendChunkln(F("<head><title>TRY_AGAIN...</title></head>"));
 wifly.sendChunkln(F("<body><h1>Bienvenidos al hogar de los Mart&iacute;nez Anzola!!!</h1></body>"));
  wifly.sendChunkln(F("<body><h1>No se encontr&oacute; la p&aacute;gina, intente de nuevo...</h1></body>"));
    wifly.sendChunkln(F("</html>"));
  wifly.sendChunkln();
  

}


 void tweet() 
 { 
 digitalWrite(13, HIGH); // Turn on the LED to show we're sending 
  
 // The following copied from setup() above: 
  
   wifiSerial.begin(9600); // Wifly baud rate: It's OK that this doesn't match serial port rate above. 
 if (!wifly.begin(&wifiSerial, &Serial)) { 
     Serial.println("Failed to start wifly"); 
 terminal(); 
   } 
  
 /* Join wifi network if not already associated */ 
 if (!wifly.isAssociated()) { 
 /* Setup the WiFly to connect to a wifi network */ 
     Serial.println("Joining network"); 
     wifly.setSSID(mySSID); 
     wifly.setPassphrase(myPassword); 
     wifly.enableDHCP(); 
 if (wifly.join()) { 
       Serial.println("Joined wifi network"); 
     }  
 else { 
       Serial.println("Failed to join wifi network"); 
 terminal(); 
     } 
   }  
 else { 
     Serial.println("Already joined network"); 
   } 
  
 if (wifly.open(site, 80)) { // Go to the website on http port 80 
     Serial.print("Connected to "); 
     Serial.println(site); 
 // end of text copied from setup() 
  
 /* Send the tweet */ 
 for ( int i = 0; i < sizeof( msg ); i++ ) { 
       msg[i] = '\0'; 
     } 
     tweetText.toCharArray( msg, tweetText.length() + 1 ); 
     Serial.println("connected"); 
     wifly.println("POST http://arduino-tweet.appspot.com/update HTTP/1.0"); 
     wifly.print("Content-Length: "); 
     wifly.println(strlen(msg)+strlen(token)+14); 
     wifly.println(); 
     wifly.print("token="); 
     wifly.print(token); 
     wifly.print("&status="); 
     wifly.println(msg); 
   }  
 else { 
     Serial.println("Failed to connect"); 
   } 
  
 digitalWrite(13, LOW); // We're done. 
 } 
 
 /* Connect the WiFly serial to the serial monitor. */ 
 void terminal() 
 { 
 while (1) { 
 if (wifly.available() > 0) { 
       Serial.write(wifly.read()); 
     } 
  
   } 
 } 
