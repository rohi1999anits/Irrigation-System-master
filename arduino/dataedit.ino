#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <dht.h>
#define dht_apin A0 
#include<SoftwareSerial.h>
SoftwareSerial comm(2,3); //setting Rx and Tx pins

String server="192.168.29.76"; //variable for sending data to webpage
String uri = "/irrigationupdate.php";
boolean No_IP=false; //variable to check for ip Address
String IP=""; //variable to store ip Address
char temp1='0';
String data = "";
/* #define ACWATERPUMP 13*/int ACWATERPUMP = 13; //You can remove this line, it has no use in the program.
//int sensor = 8; //You can remove this line, it has no use in the program.
dht DHT;
char str1[]="don't water the plants";
char str2[]="water the plants";//This variable stores the value received from Soil moisture sensor.
int a=0;
int b=0;
int x,y,z;
    
int sensor1=A1;
void setup() 
{
   Serial.begin(9600);
   Serial.println("Reading from the sensor");
    pinMode(ACWATERPUMP,OUTPUT);
    digitalWrite(ACWATERPUMP,LOW);
   comm.begin(115200);
   wifi_init();
// pinMode(ACWATERPUMP,OUTPUT); //Set pin 13 as OUTPUT pin, to send signal to relay
 pinMode(sensor1,INPUT); //Set pin 8 as input pin, to receive data from Soil moisture sensor.
  pinMode(dht_apin,INPUT);//sets pin ao to recieve data from hummidity sensor.`
   Serial.println("System Ready..");
}

void loop() 
{
 // b=0;
    
  Serial.println("Refresh Page");
    DHT.read11(dht_apin);
    Serial.print("Current humidity = ");
    x=DHT.humidity;
    Serial.print(x);
    Serial.println("%  ");
    Serial.print("temperature = ");
    y=DHT.temperature;
    Serial.print(y); 
    Serial.println("C   ");
  
  z = analogRead(sensor1)  ;
  z=map(z,1023,0,0,100);
  Serial.println("moisture =");
  Serial.print(z);
   Serial.println("%  ");
 
  data = "humidity=" + String(x) + "&moisture=" + String(z) + "&temperature=" + String(y);
  Serial.println(data);
  Serial.println("Starting");
  postData(data);

    if(z<20 || DHT.humidity<20) 
  {
 //  while(z<50 || DHT.humidity<80 )
  // {
  digitalWrite(ACWATERPUMP,HIGH); //if soil moisture sensor provides LOW value send LOW value to relay
    Serial.println(str2);
 // z = analogRead(sensor1)  ;
 // z=map(z,1023,0,0,100);
  delay(4000);
 // }
  }
  else
  {
  digitalWrite(ACWATERPUMP,LOW); //if soil moisture sensor provides HIGH value send HIGH value to relay
   Serial.println(str1);
    delay(4000);
  }
 Serial.println("Finished");
  delay(12000);


}
void findIp(int time1) //check for the availability of IP Address
{
  int time2=millis();
  while(time2+time1>millis())
  {
    while(comm.available()>0)
    {
      if(comm.find("IP has been read"))
      {
        No_IP=true;
      }
    }
  }
}

void showIP()//Display the IP Address 
{
  IP="";
  Serial.println("IP");
  char ch=0;
  while(1)
  {
    comm.println("AT+CIFSR");
    while(comm.available()>0)
    {
      if(comm.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while(comm.available()>0)
        {
          ch=comm.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
}

void establishConnection(String command, int timeOut) //Define the process for sending AT commands to module
{
  
  int q=0;
  while(1)
  {
    Serial.println(command);
    comm.println(command); 
    while(comm.available())
    {
      if(comm.find("OK"))
      q=8;
    }
    delay(timeOut);
    if(q>5)
    break;
    q++;
  }
  if(q==8)
  Serial.println("Error");
  else
  Serial.println("OK");
}

void wifi_init() //send AT commands to module
{
     establishConnection("AT",100);
      establishConnection("AT+CWMODE=3",100);
      establishConnection("AT+CWQAP",100);  
      establishConnection("AT+RST",100);
      findIp(100);
      if(!No_IP)
      {
        Serial.println("Connecting Wifi....");
        establishConnection("AT+CWJAP=\"Rama_wifi\",\"9441059211\"",7000);         //provide your WiFi username and password here
      }
      else
        {
          
        }
      delay(2000);
     // showIP();//
}

void postData (String data1) {
  establishConnection("AT+CIPSTATUS",100);
  establishConnection("AT+CIPSTART=\"TCP\",\"" + server + "\",80", 100);//start a TCP connection.
  delay(1000);
  String postRequest = "POST " + uri + " HTTP/1.0\r\n" +
                       "Host: " + server + "\r\n" +
                       "Accept: *" + "/" + "*\r\n" +
                       "Content-Length: " + data.length() + "\r\n" +
                       "Content-Type: application/x-www-form-urlencoded\r\n" +"\r\n" + data;
  Serial.println(postRequest);
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  comm.print(sendCmd);
      Serial.println("Sending..");
          Serial.println("Packet Sent");
  comm.println(postRequest.length() );
  delay(500);
  if(comm.find(">")) { 
    Serial.println("Sending..");
    comm.print(postRequest);
    if( comm.find("SEND OK")) { 
      Serial.println("Packet sent");
      while (comm.available()) {
        String tmpResp = comm.readString();
          Serial.println(tmpResp);
          
      }
    }
  }
  comm.println("AT+CIPCLOSE=0");
}
