#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

void inicio();
void st10resta();//A
void st50resta();//B
void st100resta();//C
void st500resta();//D
void st500suma();//E
void st100suma();//F
void st50suma();//G
void st10suma();//H
void sy10resta();//I
void sy50resta();//J
void sy100resta();//K
void sy500resta();//L
void sy500suma();//M
void sy100suma();//N
void sy50suma();//O
void sy10suma();//P
void ey10resta();//Q
void ey50resta();//R
void ey100resta();//S
void ey500resta();//T
void ey500suma();//U
void ey100suma();//V
void ey50suma();//W
void ey10suma();//X
void wy10resta();//Y
void wy50resta();//Z
void wy100resta();//a
void wy500resta();//b
void wy500suma();//c
void wy100suma();//d
void wy50suma();//e
void wy10suma();//f
void wrist_twistanthor();//g
void wrist_twistmid();//h
void wrist_twisthor();//i
void gripopen();//j
void gripmid();//k
void gripclose();//l
void st20resta();//1
void st20suma();//0
void st200resta();//2
void st200suma();//3
void sy20resta();//4
void sy20suma();//5
void sy200resta();//6
void sy200suma();//7
void ey20resta();//8
void ey20suma();//9
void ey200resta();//m
void ey200suma();//n
void wy20resta();//o
void wy20suma();//p
void wy200resta();//q
void wy200suma();//r
void erro404();
void fvertical();  



int shouldertwist = constrain(shouldertwist, 510, 1950);
int shouldery = constrain(shouldery, 800, 1800);
int elbowy = constrain(elbowy, 800, 2200);
int wristy = constrain(wristy, 850, 1500);


int wristtwist = 1450;
int griper = 1800;
int timito = 5000;
int timedelay = 5000;

int i; //VariÃ¡vel para contagem
const char* ssid = "NETGEAR50";
const char* password = "gracefulpond393";
 
int ledPin = D4; // GPIO13
WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = 1;
  if (request.indexOf("/INITIALPOSITION") != -1)  {
    value = 2;
    inicio();
      
  }
  if (request.indexOf("/LED=Blink8") != -1)  {
    for(i=0;i<=8;i++) //LaÃ§o para o efeito de piscagem do LED
  {
     digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
    value = 3;
   
}

if (request.indexOf("/FULLVERTICAL") != -1)  {
    
    fvertical();  
    value = 70;
   
}



if (request.indexOf("/LEFT1") != -1)  {
    st10suma();
    value = 4;
 
}

if (request.indexOf("/LEFT2") != -1)  {
    st20suma();
    value = 5;
   
}

if (request.indexOf("/LEFT5") != -1)  {
    st50suma();
    value = 6;
    
}

if (request.indexOf("/LEFT7") != -1)  {
    st100suma();
    value = 7;

}

if (request.indexOf("/LEFT8") != -1)  {
    st200suma();
    value = 8;

}

if (request.indexOf("/LEFT45") != -1)  {
    st500suma();
    value = 9;

}

if (request.indexOf("/RIGHT1") != -1)  {
    st10resta();
    value = 10;
    
}

if (request.indexOf("/RIGHT2") != -1)  {
    st20resta();
    value = 11;
   
}

if (request.indexOf("/RIGHT5") != -1)  {
    st50resta();
    value = 12;
    
}

if (request.indexOf("/RIGHT7") != -1)  {
    st100resta();
    value = 14;
   
}

if (request.indexOf("/RIGHT8") != -1)  {
    st200resta();
    value = 13;
   
}


if (request.indexOf("/RIGHT45") != -1)  {
    st500resta();
    value = 15;
   
}

/*SY
 * 
 */

if (request.indexOf("/SYUP1") != -1)  {
    sy10resta();
    value = 16;
   
}

if (request.indexOf("/SYUP2") != -1)  {
    sy20resta();
    value = 17;
   
}
if (request.indexOf("/SYUP5") != -1)  {
    sy50resta();
    value = 18;
 
}
if (request.indexOf("/SYUP7") != -1)  {
   sy100resta();;
    value = 19;
    
}
if (request.indexOf("/SYUP8") != -1)  {
    sy200resta();
    value = 20;
    
}
if (request.indexOf("/SYUP45") != -1)  {
    sy500resta();
    value = 21;
    
}
if (request.indexOf("/SYDOWN45") != -1)  {
    sy500suma();
    value = 22;
  
}
if (request.indexOf("/SYDOWN8") != -1)  {
    sy200suma();
    value = 23;
 
}
if (request.indexOf("/SYDOWN7") != -1)  {
    sy100suma();
    value = 24;
   
}
if (request.indexOf("/SYDOWN5") != -1)  {
    sy50suma();
    value = 25;
  
}
if (request.indexOf("/SYDOWN2") != -1)  {
    sy20suma();
    value = 26;
    
}
if (request.indexOf("/SYDOWN1") != -1)  {
    sy10suma();
    value = 27;
   
}


/*EY
 * 
 */

if (request.indexOf("/EYUP1") != -1)  {
    ey10suma();
    value = 29;
   
}

if (request.indexOf("/EYUP2") != -1)  {
    ey20suma();
    value = 30;
   
}
if (request.indexOf("/EYUP5") != -1)  {
    ey50suma();
    value = 31;
   
}
if (request.indexOf("/EYUP7") != -1)  {
    ey100suma();
    value = 32;
   
}
if (request.indexOf("/EYUP8") != -1)  {
    ey200suma();
    value = 33;
   
}
if (request.indexOf("/EYUP45") != -1)  {
    ey500suma();
    value = 34;
 
}
if (request.indexOf("/EYDOWN45") != -1)  {
    ey500resta();
    value = 35;
   
}
if (request.indexOf("/EYDOWN8") != -1)  {
    ey200resta();
    value = 36;
   
}
if (request.indexOf("/EYDOWN7") != -1)  {
    ey100resta();
    value = 37;
   
}
if (request.indexOf("/EYDOWN5") != -1)  {
    ey50resta();
    value = 38;
   
}
if (request.indexOf("/EYDOWN2") != -1)  {
    ey20resta();
    value = 39;
   
}
if (request.indexOf("/EYDOWN1") != -1)  {
    ey10resta();
    value = 40;
  
}


/* WY
*/

if (request.indexOf("/WYUP1") != -1)  {
    wy10suma();
    value = 41;
   
}

if (request.indexOf("/WYUP2") != -1)  {
    wy20suma();
    value = 42;
   
}
if (request.indexOf("/WYUP5") != -1)  {
    wy50suma();
    value = 43;
   
}
if (request.indexOf("/WYUP7") != -1)  {
    wy100suma();
    value = 44;
 
}
if (request.indexOf("/WYUP8") != -1)  {
    wy200suma();
    value = 45;

}
if (request.indexOf("/WYUP45") != -1)  {
    wy500suma();
    value = 46;
   
}
if (request.indexOf("/WYDOWN45") != -1)  {
    wy500resta();
    value = 47;
   
}
if (request.indexOf("/WYDOWN8") != -1)  {
    wy200resta();
    value = 48;
  
}
if (request.indexOf("/WYDOWN7") != -1)  {
    wy100resta();
    value = 49;
  
}
if (request.indexOf("/WYDOWN5") != -1)  {
    wy50resta();
    value = 50;
   
}
if (request.indexOf("/WYDOWN2") != -1)  {
    wy20resta();
    value = 51;
  
}
if (request.indexOf("/WYDOWN1") != -1)  {
    wy10resta();
    value = 52;
  
}

/*WT
 * 
 */

if (request.indexOf("/WTFA") != -1)  {
   wrist_twistanthor();
    value = 53;
  
}

if (request.indexOf("/WTC") != -1)  {
    wrist_twistmid();
    value = 54;

}
if (request.indexOf("/WTFC") != -1)  {
   wrist_twisthor();
    value = 55;

}

/*G
 * 
 */

if (request.indexOf("/GO") != -1)  {
   gripopen();
    value = 56;
  
}
if (request.indexOf("/GMO") != -1)  {
   gripmid();
    value = 57;

}
if (request.indexOf("/GC") != -1)  {
   gripclose();
    value = 58;

}




 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 /*
  * int shouldertwist = 1150;
int shouldery = 1300;
int elbowy = 1430;
int wristy = 2150;
int wristtwist = 1450;
int griper = 1800;
  */
  client.print("My position is: ");
  client.print(shouldertwist);
   client.print(", ");
    client.print(shouldery);
    client.print(", ");
     client.print(elbowy);
     client.print(", ");
      client.print(wristy);
      client.print(", ");
       client.print(wristtwist);
       client.print(", ");
        client.print(griper);
        client.println(".");
        
    
  
  
 
  if(value == 2) {
    client.print(" Initial Position");
  }
  else{
    if(value == 3){
    client.print(" Connection OK");
    }
     else {
if(value == 4){
    client.print("blink8");
    }
     else {

if(value == 70){
    client.print("Full Vertical");
    }
     else {
    client.print(" #");
  }
  }
  }
  }
/*

  client.println("<a href=\"/LED=ON\"\"><center><button>Turn On </button></a>");
  client.println("<a href=\"/LED=Blink4\"\"><button>Blink4 </button></a>");
  client.println("<a href=\"/LED=Blink8\"\"><button>Blink8 </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></center></a><br />");

  client.println("<a href=\"/FUNCION A EJECUTAR\"\"><center><button>TITULO DEL BOTON</button></a>");

  */

  client.println("<p><b><h1><center>Numeric Control of the Robotic Arm</center></h1></b></p>");
  client.println("<h4><p><left>Jos&eacute; Luis Mart&iacute;nez.</left></p></h4>");
  client.println("<a href=\"/INITIALPOSITION\"\"><p><center><button>INITIAL POSITION</button></a>");
  client.println("<a href=\"/LED=Blink8\"\"><button>Test Connection </button></a>");
  client.println("<a href=\"/FULLVERTICAL\"\"><button>FULL VERTICAL</button></center><p></a>");
  client.println("<p><b><h5><center>SHOULDER TWIST (#27)</center></h5></b></p>");

  client.println("<a href=\"/LEFT1\"\"><center><button>LEFT 1 </button></a>");
  client.println("<a href=\"/LEFT2\"\"><button>LEFT 2  </button></a>");
  client.println("<a href=\"/LEFT5\"\"><button>LEFT 5  </button></a>");
  client.println("<a href=\"/LEFT7\"\"><button>LEFT 10 </button></a>");
  client.println("<a href=\"/LEFT8\"\"><button>LEFT 20 </button></a>");
  client.println("<a href=\"/LEFT45\"\"><button>LEFT 45 </button></a>");
  client.println("<a href=\"/RIGHT45\"\"><button>RIGHT 45</button></a>");
  client.println("<a href=\"/RIGHT8\"\"><button>RIGHT 20</button></a>");
  client.println("<a href=\"/RIGHT7\"\"><button>RIGHT 10</button></a>");
  client.println("<a href=\"/RIGHT5\"\"><button>RIGHT 5 </button></a>");
  client.println("<a href=\"/RIGHT2\"\"><button>RIGHT 2 </button></a>");
  client.println("<a href=\"/RIGHT1\"\"><button>RIGHT 1 </button></center></a><br />");
 
  client.println("<p><b><h5><center>SHOULDER Y (#31)</center></h5></b></p>");

  client.println("<a href=\"/SYUP1\"\"><center><button>UP 1 </button></a>");
  client.println("<a href=\"/SYUP2\"\"><button>UP 2   </button></a>");
  client.println("<a href=\"/SYUP5\"\"><button>UP 5   </button></a>");
  client.println("<a href=\"/SYUP7\"\"><button>UP 10  </button></a>");
  client.println("<a href=\"/SYUP8\"\"><button>UP 20  </button></a>");
  client.println("<a href=\"/SYUP45\"\"><button>UP 45  </button></a>");
  client.println("<a href=\"/SYDOWN45\"\"><button>DOWN 45</button></a>");
  client.println("<a href=\"/SYDOWN8\"\"><button>DOWN 20</button></a>");
  client.println("<a href=\"/SYDOWN7\"\"><button>DOWN 10</button></a>");
  client.println("<a href=\"/SYDOWN5\"\"><button>DOWN 5 </button></a>");
  client.println("<a href=\"/SYDOWN2\"\"><button>DOWN 2 </button></a>");
  client.println("<a href=\"/SYDOWN1\"\"><button>DOWN 1 </button></center></a><br />");
  
  client.println("<p><b><h5><center>ELBOW Y (#3)</center></h5></b></p>");

  client.println("<a href=\"/EYUP1\"\"><center><button>UP 1   </button></a>");
  client.println("<a href=\"/EYUP2\"\"><button>UP 2   </button></a>");
  client.println("<a href=\"/EYUP5\"\"><button>UP 5   </button></a>");
  client.println("<a href=\"/EYUP7\"\"><button>UP 10  </button></a>");
  client.println("<a href=\"/EYUP8\"\"><button>UP 20  </button></a>");
  client.println("<a href=\"/EYUP45\"\"><button>UP 45  </button></a>");
  client.println("<a href=\"/EYDOWN45\"\"><button>DOWN 45</button></a>");
  client.println("<a href=\"/EYDOWN8\"\"><button>DOWN 20</button></a>");
  client.println("<a href=\"/EYDOWN7\"\"><button>DOWN 10</button></a>");
  client.println("<a href=\"/EYDOWN5\"\"><button>DOWN 5 </button></a>");
  client.println("<a href=\"/EYDOWN2\"\"><button>DOWN 2 </button></a>");
  client.println("<a href=\"/EYDOWN1\"\"><button>DOWN 1 </button></center></a><br />");
     
  client.println("<p><b><h5><center>WRIST Y (#7)</center></h5></b></p>");

  client.println("<a href=\"/WYUP1\"\"><center><button>UP 1   </button></a>");
  client.println("<a href=\"/WYUP2\"\"><button>UP 2   </button></a>");
  client.println("<a href=\"/WYUP5\"\"><button>UP 5   </button></a>");
  client.println("<a href=\"/WYUP70\"\"><button>UP 10  </button></a>");
  client.println("<a href=\"/WYUP8\"\"><button>UP 20  </button></a>");
  client.println("<a href=\"/WYUP45\"\"><button>UP 45  </button></a>");
  client.println("<a href=\"/WYDOWN45\"\"><button>DOWN 45</button></a>");
  client.println("<a href=\"/WYDOWN8\"\"><button>DOWN 20</button></a>");
  client.println("<a href=\"/WYDOWN7\"\"><button>DOWN 10</button></a>");
  client.println("<a href=\"/WYDOWN5\"\"><button>DOWN 5 </button></a>");
  client.println("<a href=\"/WYDOWN2\"\"><button>DOWN 2 </button></a>");
  client.println("<a href=\"/WYDOWN1\"\"><button>DOWN 1 </button></center></a><br />");
  
  client.println("<p><b><h5><center>WRIST TWIST (#11)</center></h5></b></p>");

  client.println("<a href=\"/WTFA\"\"><center><button>Wrist Twist Full AntiClock </button></a>");
  client.println("<a href=\"/WTC\"\"><button>Wrist Twist Center </button></a>");
  client.println("<a href=\"/WTFC\"\"><button>Wrist Twist Full Clock </button></center></a><br />");

  client.println("<p><b><h5><center>GRIP (#15)</center></h5></b></p>");

  client.println("<a href=\"/GO\"\"><center><button>Grip Open</button </button></a>");
  client.println("<a href=\"/GMO\"\"><button>Grip Mid Open </button></a>");
  client.println("<a href=\"/GC\"\"><button>Grip Close </button></center></a><br />");
      

   
  client.println("</html>");

/*

   
  client.println("<p><center><button onclick=\"window.location.href='http://192.168.1.17/H'\">  LEFT 1  </button><button onclick=\"window.location.href='http://192.168.1.17/0'\">  LEFT 2  </button><button onclick=\"window.location.href='http://192.168.1.17/G'\">  LEFT 5  </button><button onclick=\"window.location.href='http://192.168.1.17/F'\">  LEFT 10  </button><button onclick=\"window.location.href='http://192.168.1.17/3'\">  LEFT 20  </button><button onclick=\"window.location.href='http://192.168.1.17/E'\">  LEFT 45  </button><button onclick=\"window.location.href='http://192.168.1.17/D'\">  RIGHT 45  </button><button onclick=\"window.location.href='http://192.168.1.17/2'\">  RIGHT 20  </button><button onclick=\"window.location.href='http://192.168.1.17/C'\">  RIGHT 10  </button><button onclick=\"window.location.href='http://192.168.1.17/B'\">  RIGHT 5  </button><button onclick=\"window.location.href='http://192.168.1.17/1'\">  RIGHT 2  </button><button onclick=\"window.location.href='http://192.168.1.17/A'\">  RIGHT 1  </button></center></p>");
  
 client.println("<p><center><button onclick=\"window.location.href='http://192.168.1.17/I'\">  UP 1  </button><button onclick=\"window.location.href='http://192.168.1.17/4'\">  UP 2  </button><button onclick=\"window.location.href='http://192.168.1.17/J'\">  UP 5  </button><button onclick=\"window.location.href='http://192.168.1.17/K'\">  UP 10  </button><button onclick=\"window.location.href='http://192.168.1.17/6'\">  UP 20  </button><button onclick=\"window.location.href='http://192.168.1.17/L'\">  UP 45  </button><button onclick=\"window.location.href='http://192.168.1.17/M'\">  DOWN 45  </button><button onclick=\"window.location.href='http://192.168.1.17/7'\">  DOWN 20  </button><button onclick=\"window.location.href='http://192.168.1.17/N'\">  DOWN 10  </button><button onclick=\"window.location.href='http://192.168.1.17/O'\">  DOWN 5  </button><button onclick=\"window.location.href='http://192.168.1.17/5'\">  DOWN 2  </button><button onclick=\"window.location.href='http://192.168.1.17/P'\">  DOWN 1  </button></center></p>");

 client.println("<p><center><button onclick=\"window.location.href='http://192.168.1.17/X'\">  UP 1  </button><button onclick=\"window.location.href='http://192.168.1.17/9'\">  UP 2  </button><button onclick=\"window.location.href='http://192.168.1.17/W'\">  UP 5  </button><button onclick=\"window.location.href='http://192.168.1.17/V'\">  UP 10  </button><button onclick=\"window.location.href='http://192.168.1.17/n'\">  UP 20  </button><button onclick=\"window.location.href='http://192.168.1.17/U'\">  UP 45  </button><button onclick=\"window.location.href='http://192.168.1.17/T'\">  DOWN 45  </button><button onclick=\"window.location.href='http://192.168.1.17/m'\">  DOWN 20  </button><button onclick=\"window.location.href='http://192.168.1.17/S'\">  DOWN 10  </button><button onclick=\"window.location.href='http://192.168.1.17/R'\">  DOWN 5  </button><button onclick=\"window.location.href='http://192.168.1.17/8'\">  DOWN 2  </button><button onclick=\"window.location.href='http://192.168.1.17/Q'\">  DOWN 1  </button></center></p>");
  
   client.println("<p><center><button onclick=\"window.location.href='http://192.168.1.17/f'\">  Up 1  </button><button onclick=\"window.location.href='http://192.168.1.17/p'\">  UP 2  </button><button onclick=\"window.location.href='http://192.168.1.17/e'\">  UP 5  </button><button onclick=\"window.location.href='http://192.168.1.17/d'\">  UP 10  </button><button onclick=\"window.location.href='http://192.168.1.17/r'\">  UP 20  </button><button onclick=\"window.location.href='http://192.168.1.17/c'\">  UP 45  </button><button onclick=\"window.location.href='http://192.168.1.17/b'\">  DOWN 45  </button><button onclick=\"window.location.href='http://192.168.1.17/q'\">  DOWN 20  </button><button onclick=\"window.location.href='http://192.168.1.17/a'\">  DOWN 10  </button><button onclick=\"window.location.href='http://192.168.1.17/Z'\">  DOWN 5  </button><button onclick=\"window.location.href='http://192.168.1.17/o'\">  DOWN 2  </button><button onclick=\"window.location.href='http://192.168.1.17/Y'\">  DOWN 1  </button></center></p>");
   
  client.println("<p><center><button onclick=\"window.location.href='http://192.168.1.17/g'\">Wrist Twist Full AntiClock</button><button onclick=\"window.location.href='http://192.168.1.17/h'\">Wrist Twist Center</button><button onclick=\"window.location.href='http://192.168.1.17/i'\">Wrist Twist Full Clock</button></center></p>"));

  client.println("<p><center><button onclick=\"window.location.href='http://192.168.1.17/j'\">Grip Open</button><button onclick=\"window.location.href='http://192.168.1.17/k'\">Grip Mid Open</button><button onclick=\"window.location.href='http://192.168.1.17/l'\">Grip Close</button></center></p>"));
 
 */ 
  
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}

void printingh()
{
  Serial.println("Prueba de HIGH");
  Serial.println("");
}
void printingl()
{
  Serial.println("Prueba de LOW");
  Serial.println("");
}
void printingb4()
{
  Serial.println("Prueba de Blink4");
  Serial.println("");
}
void printingb8()
{
  Serial.println("Prueba de Blink8");
  Serial.println("");
}

/*
 * Insertando funciones del programa original
 * 
 */

 void inicio()
{

shouldertwist = 1150;
shouldery = 1300;
elbowy = 1430;
wristy = 2150;
wristtwist = 1450;
griper = 1800;



 
  Serial.print ("#27 P");
  Serial.print (shouldertwist);
  Serial.print (" #31 P");
  Serial.print (shouldery);
  Serial.print (" #3 P");
  Serial.print (elbowy);
  Serial.print (" #7 P");
  Serial.print (wristy);
  Serial.print (" #11 P");
  Serial.print (wristtwist);
  Serial.print (" #15 P");
  Serial.print (griper);
  Serial.print (" T");
  Serial.println(timito);
          delay (timedelay);
}
// 
void st10resta()
{
  shouldertwist = shouldertwist-10;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
void st50resta()
{
  shouldertwist = shouldertwist-50;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void st100resta()
{
  shouldertwist = shouldertwist-100;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void st500resta()
{
  shouldertwist = shouldertwist-500;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void st500suma()
{
  shouldertwist = shouldertwist+500;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void st100suma()
{
  shouldertwist = shouldertwist+100;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void st50suma()
{
  shouldertwist = shouldertwist+50;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void st10suma()
{
  shouldertwist = shouldertwist+10;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy10resta()
{
  shouldery = shouldery-10;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy50resta()
{
  shouldery = shouldery-50;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy100resta()
{
  shouldery = shouldery-100;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy500resta()
{
  shouldery = shouldery-500;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy10suma()
{
  shouldery = shouldery+10;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy50suma()
{
  shouldery = shouldery+50;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy100suma()
{
  shouldery = shouldery+100;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void sy500suma()
{
  shouldery = shouldery+500;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey10resta()
{
  elbowy = elbowy-10;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey50resta()
{
  elbowy = elbowy-50;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey100resta()
{
  elbowy = elbowy-100;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey500resta()
{
  elbowy = elbowy-500;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey10suma()
{
  elbowy = elbowy+10;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey50suma()
{
  elbowy = elbowy+50;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey100suma()
{
  elbowy = elbowy+100;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void ey500suma()
{
  elbowy = elbowy+500;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void wy10resta()
{
  wristy = wristy-10;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
  void wy50resta()
{
  wristy = wristy-50;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
  void wy100resta()
{
  wristy = wristy-100;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
  void wy500resta()
{
  wristy = wristy-500;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
  void wy10suma()
{
  wristy = wristy+10;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void wy50suma()
{
  wristy = wristy+50;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void wy100suma()
{
  wristy = wristy+100;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }
 void wy500suma()
{
  wristy = wristy+500;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
 }

 /*
  * int shouldertwist = 1150;
int shouldery = 1300;
int elbowy = 1430;
int wristy = 2150;
int wristtwist = 1450;
int griper = 1800;
  */
  void wrist_twistanthor()
{
 Serial.println("#11 P700 T5000");
 wristtwist = 700;
 delay (timedelay);
 }
 void wrist_twistmid()
{

 Serial.println("#11 P1550 T5000");
 wristtwist = 1450;
 delay (timedelay);
 }
 void wrist_twisthor()
{
 Serial.println("#11 P2400 T5000");
 wristtwist = 2200;
 delay (timedelay);
 }
 void st20resta()
{
  shouldertwist = shouldertwist-20;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void st200resta()
{
  shouldertwist = shouldertwist-200;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void st20suma()
{
  shouldertwist = shouldertwist+20;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void st200suma()
{
  shouldertwist = shouldertwist+200;
 Serial.print (" #27 P");
  Serial.print (shouldertwist);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void sy20resta()
{
  shouldery = shouldery-20;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void sy200resta()
{
  shouldery = shouldery-200;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void sy20suma()
{
  shouldery = shouldery+20;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void sy200suma()
{
  shouldery = shouldery+200;
 Serial.print (" #31 P");
  Serial.print (shouldery);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void ey20resta()
{
  elbowy = elbowy-20;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void ey200resta()
{
  elbowy = elbowy-200;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void ey20suma()
{
  elbowy = elbowy+20;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void ey200suma()
{
  elbowy = elbowy+200;
 Serial.print (" #3 P");
  Serial.print (elbowy);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void wy20resta()
{
  wristy = wristy-20;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void wy200resta()
{
  wristy = wristy-200;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
  }
  void wy20suma()
{
  wristy = wristy+20;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
  }
   void wy200suma()
{
  wristy = wristy+200;
 Serial.print (" #7 P");
  Serial.print (wristy);
   Serial.println(" T1000");
 delay (timedelay);
  }

  /*
  // Envia página de erro caso seja digitado uma URL inválida
void erro404()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Transfer-Encoding: chunked");
  client.println("");
  client.println("<head><title>TRY_AGAIN...</title></head>");
  client.println("<body><h1>P&aacute;gina n&atilde;o encontrada</h1></body>");
  client.println("</html>");
  client.println("");
}



*/
void gripopen()
{
 griper = 1800;
 Serial.println("#15 P1800 T1000");
 delay (timedelay);
 }
void gripmid()
{
 griper = 2000;
 Serial.println("#15 P2000 T1000");
 delay (timedelay);
 } 
 void gripclose()
{
 griper = 2200;
 Serial.println("#15 P2200 T1000");
 delay (timedelay);
 }

 void fvertical()
 {

shouldertwist = 1150;
shouldery = 1150;
elbowy = 1380;
wristy = 2150;
wristtwist = 1450;
griper = 1800;



 
  Serial.print ("#27 P");
  Serial.print (shouldertwist);
  Serial.print (" #31 P");
  Serial.print (shouldery);
  Serial.print (" #3 P");
  Serial.print (elbowy);
  Serial.print (" #7 P");
  Serial.print (wristy);
  Serial.print (" #11 P");
  Serial.print (wristtwist);
  Serial.print (" #15 P");
  Serial.print (griper);
  Serial.print (" T");
  Serial.println(timito);
          delay (timedelay);
}
 

