#include<dht.h>
#define dht_apin A0 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
char ssid[]="Rama_wifi";  // replace with your ssid & pass
char pass[]="9441059211";
dht DHT;
/*//Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
//#define DHT11_PIN 7
//#define MQ_PIN (0) //define which analog input channel you are going to use
//#define RL_VALUE (5) //define the load resistance on the board, in kilo ohms
//#define RO_CLEAN_AIR_FACTOR (9.83) //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean
air)/RO,
*/
#define dht_apin A0 
#include<SoftwareSerial.h>
SoftwareSerial comm(2,3); //setting Rx and Tx pins
/*
String server="192.168.29.76"; //variable for sending data to webpage
String uri = "/irrigationupdate.php";


boolean No_IP=false; //variable to check for ip Address
String IP=""; //variable to store ip Address
char temp1='0';

*/
String data = "";

int ACWATERPUMP = 13; //You can remove this line, it has no use in the program.
int sensor = 8; //You can remove this line, it has no use in the program.

//dht DHT;
char str1[]="don't water the plants";
char str2[]="water the plants";//This variable stores the value received from Soil moisture sensor.
int a=0;
int b=0;
int x,y,z,val;
int sensor_pin=A0;
int sensor1;
void setup() 
{
   //Serial.begin(9600);
   Serial.println("Reading from the sensor");
  /* comm.begin(115200);
   wifi_init();
  */
  Serial.begin(9600);
Serial.begin(9600);
delay(500);
  Serial.print("Connecting to..");
  Serial.println(ssid);
  delay(500);
  WiFi.disconnect();
  WiFi.begin(ssid,pass);
while(WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.println(".");
}
  pinMode(ACWATERPUMP,OUTPUT); //Set pin 13 as OUTPUT pin, to send signal to relay
  pinMode(sensor1,INPUT); //Set pin 8 as input pin, to receive data from Soil moisture sensor.
  pinMode(dht_apin,INPUT);//sets pin ao to recieve data from hummidity sensor.`
   Serial.println("System Ready..");
}

void loop() 
{
  if((WiFi.status() == WL_CONNECTED)) 
 {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
 // b=0;
  Serial.println("Refresh Page");
    DHT.read11(dht_apin);
    Serial.print("Current humidity = ");
    x=DHT.humidity;
    Serial.print(x);
    Serial.println("%  ");
    Serial.print("temperature = ");
    y=DHT.temperature;
    Serial.println(y); 
    Serial.println("C   ");
  
 z=analogRead(sensor1);
 z=map(z,1023,0,0,100);
  
  Serial.println("moisture =");
  Serial.println(z);
   Serial.println("%  ");
  Serial.print(x);
  Serial.print("   ");
   Serial.print(z);
  Serial.print("   ");
   Serial.print(y);
  Serial.print("   " );
  Serial.println("Starting");
 
if(z<20 || DHT.humidity<20) 
  {
  // while(z<50 || DHT.humidity<80 )
   //{
  digitalWrite(ACWATERPUMP,HIGH); //if soil moisture sensor provides LOW value send LOW value to relay
    Serial.println(str2);
 // z = analogRead(sensor1)  ;
//  z=map(z,1023,0,0,100);
  delay(4000);
 // }
  }
  else
  {
  digitalWrite(ACWATERPUMP,LOW); //if soil moisture sensor provides HIGH value send HIGH value to relay
   Serial.println(str1 );
  }
 Serial.println("Finished");
  delay(4000);

http.begin("http://192.168.29.76/irrigationupdate.php?humidity=" + String(x) + "&moisture=" + String(z) + "&temperature=" + String(y));
}

}
