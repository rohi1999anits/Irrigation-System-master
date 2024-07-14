#include"dht.h"
#define dht_apin A0 
int ACWATERPUMP = 13; //You can remove this line, it has no use in the program.
int sensor = 8; //You can remove this line, it has no use in the program.
int val=0;
dht DHT;
char str1[]="don't water the plants";
char str2[]="water the plants";//This variable stores the value received from Soil moisture sensor.
void setup() {
  Serial.begin(9600);
  pinMode(ACWATERPUMP,OUTPUT); //Set pin 13 as OUTPUT pin, to send signal to relay
  pinMode(sensor,INPUT); //Set pin 8 as input pin, to receive data from Soil moisture sensor.
  pinMode(dht_apin,INPUT);//sets pin ao to recieve data from hummidity sensor.`
}

void loop() { 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
  
  val = digitalRead(8);//Read data from soil moisture sensor  
  Serial.println(val);
  if(val == LOW) 
  {
  digitalWrite(ACWATERPUMP,LOW); //if soil moisture sensor provides LOW value send LOW value to relay
    Serial.println(str1);
  }
  else
  {
  digitalWrite(ACWATERPUMP,HIGH); //if soil moisture sensor provides HIGH value send HIGH value to relay
   Serial.println(str2);
  }
  delay(400); //Wait for few second and then continue the loop.
  if(DHT.humidity<60)
  {
    while(DHT.humidity<80)
    {
     digitalWrite(ACWATERPUMP,HIGH);
     }
  } digitalWrite(ACWATERPUMP,LOW);
}
