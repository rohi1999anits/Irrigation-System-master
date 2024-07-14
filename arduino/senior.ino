
//#include &lt;Wire.h&gt;
//#include &lt;Adafruit_Sensor.h&gt;
//#include &lt;Adafruit_BMP085_U.h&gt;
//#include &lt;dht.h&gt;
#include<dht.h>
#define dht_apin A0 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
char ssid[]="qwe";  // replace with your ssid & pass
char pass[]="16716716";
dht DHT;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
#define DHT11_PIN 7
#define MQ_PIN (0) //define which analog input channel you are going to use
#define RL_VALUE (5) //define the load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR (9.83) //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean
air)/RO,
//which is derived from the chart in datasheet
/***********************Software Related Macros************************************/
#define CALIBARAION_SAMPLE_TIMES (50) //define how many samples you are going to take in
the calibration phase
#define CALIBRATION_SAMPLE_INTERVAL (500) //define the time interal(in milisecond) between
each samples in the
//cablibration phase
#define READ_SAMPLE_INTERVAL (50) //define how many samples you are going to take in
normal operation
#define READ_SAMPLE_TIMES (5) //define the time interal(in milisecond) between each
samples in
//normal operation
/**********************Application Related Macros**********************************/
#define GAS_LPG (0)
#define GAS_CO (1)
#define GAS_SMOKE (2)
/*****************************Globals***********************************************/
float LPGCurve[3] = {2.3,0.21,-0.47}; //two points are taken from the curve.
//with these two points, a line is formed which is &quot;approximately
equivalent&quot;
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59)
float COCurve[3] = {2.3,0.72,-0.34}; //two points are taken from the curve.
//with these two points, a line is formed which is &quot;approximately
equivalent&quot;
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000, 0.15)
float SmokeCurve[3] ={2.3,0.53,-0.44}; //two points are taken from the curve.

//with these two points, a line is formed which is &quot;approximately
equivalent&quot;
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000, -0.22)
float Ro = 10; //Ro is initialized to 10 kilo ohms

/****************** MQResistanceCalculation ****************************************
Input: raw_adc - raw value read from adc, which represents the voltage
Output: the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
across the load resistor and its resistance, the resistance of the sensor
could be derived.
************************************************************************************/
float MQResistanceCalculation(int raw_adc)
{
return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}
/***************************** MQCalibration ****************************************
Input: mq_pin - analog channel
Output: Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use
MQResistanceCalculation to calculates the sensor resistance in clean air
and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about
10, which differs slightly between different sensors.
************************************************************************************/
float MQCalibration(int mq_pin)
{
int i;
float val=0;
for (i=0;i&lt;CALIBARAION_SAMPLE_TIMES;i++) { //take multiple samples
val += MQResistanceCalculation(analogRead(mq_pin));
delay(CALIBRATION_SAMPLE_INTERVAL);
}
val = val/CALIBARAION_SAMPLE_TIMES; //calculate the average value
val = val/RO_CLEAN_AIR_FACTOR; //divided by RO_CLEAN_AIR_FACTOR yields the Ro
//according to the chart in the datasheet
return val;
}

void setup(){
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
Serial.print(&quot;Calibrating...&quot;);
Ro = MQCalibration(MQ_PIN); //Calibrating the sensor. Please make sure the sensor is in
clean air
//when you perform the calibration
Serial.print(&quot;Calibration is done...&quot;);
Serial.print(&quot;Ro=&quot;);
Serial.print(Ro);
Serial.print(&quot;kohm&quot;);
Serial.print(&quot;&quot;);
displaySensorDetails();

Serial.println(&quot;Pressure Sensor Test&quot;); Serial.println(&quot;&quot;);

/* Initialise the sensor */
if(!bmp.begin())
{
/* There was a problem detecting the BMP085 ... check your connections */
Serial.print(&quot;Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!&quot;);
while(1);
}


}
void displaySensorDetails(void)
{
sensor_t sensor;
bmp.getSensor(&amp;sensor);
Serial.println(&quot;-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- &quot;);
Serial.print (&quot;Sensor: &quot;); Serial.println(sensor.name);
Serial.print (&quot;Driver Ver: &quot;); Serial.println(sensor.version);
Serial.print (&quot;Unique ID: &quot;); Serial.println(sensor.sensor_id);
Serial.print (&quot;Max Value: &quot;); Serial.print(sensor.max_value); Serial.println(&quot; hPa&quot;);
Serial.print (&quot;Min Value: &quot;); Serial.print(sensor.min_value); Serial.println(&quot; hPa&quot;);
Serial.print (&quot;Resolution: &quot;); Serial.print(sensor.resolution); Serial.println(&quot; hPa&quot;);
Serial.println(&quot;-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- &quot;);
Serial.println(&quot;&quot;);
delay(500);
}

/***************************** MQRead *********************************************

Input: mq_pin - analog channel
Output: Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
The Rs changes as the sensor is in the different consentration of the target
gas. The sample times and the time interval between samples could be configured
by changing the definition of the macros.
************************************************************************************/
float MQRead(int mq_pin)
{
int i;
float rs=0;
for (i=0;i&lt;READ_SAMPLE_TIMES;i++) {
rs += MQResistanceCalculation(analogRead(mq_pin));
delay(READ_SAMPLE_INTERVAL);
}
rs = rs/READ_SAMPLE_TIMES;
return rs;
}
/***************************** MQGetGasPercentage **********************************
Input: rs_ro_ratio - Rs divided by Ro
gas_id - target gas type
Output: ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which
calculates the ppm (parts per million) of the target gas.
************************************************************************************/
int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
if ( gas_id == GAS_LPG ) {
return MQGetPercentage(rs_ro_ratio,LPGCurve);
} else if ( gas_id == GAS_CO ) {
return MQGetPercentage(rs_ro_ratio,COCurve);
} else if ( gas_id == GAS_SMOKE ) {
return MQGetPercentage(rs_ro_ratio,SmokeCurve);
}
return 0;
}
/***************************** MQGetPercentage **********************************
Input: rs_ro_ratio - Rs divided by Ro
pcurve - pointer to the curve of the target gas
Output: ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm)
of the line could be derived if y(rs_ro_ratio) is provided. As it is a

logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic
value.
************************************************************************************/
int MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

void loop()
{

 if((WiFi.status() == WL_CONNECTED)) 
 {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url



  
int chk = DHT.read11(DHT11_PIN);
//Serial.print(&quot;Temperature = &quot;);
//Serial.println(DHT.temperature);
Serial.print(&quot;Humidity = &quot;);
Serial.println(DHT.humidity);
delay(1000);

sensors_event_t event;
bmp.getEvent(&amp;event);

/* Display the results (barometric pressure is measure in hPa) */
if (event.pressure)
{
/* Display atmospheric pressue in hPa */
Serial.print(&quot;Pressure: &quot;);
Serial.print(event.pressure);
Serial.println(&quot; hPa&quot;);

/* Calculating altitude with reasonable accuracy requires pressure *
* sea level pressure for your position at the moment the data is *
* converted, as well as the ambient temperature in degress *
* celcius. If you don&#39;t have these values, a &#39;generic&#39; value of *
* 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA *
* in sensors.h), but this isn&#39;t ideal and will give variable *
* results from one day to the next. *
* *
* You can usually find the current SLP value by looking at weather *
* websites or from environmental information centers near any major *
* airport. *
* *
* For example, for Paris, France you can check the current mean *
* pressure and sea level at: http://bit.ly/16Au8ol */


/* First we get the current temperature from the BMP085 */
float temperature;
bmp.getTemperature(&amp;temperature);
Serial.print(&quot;Temperature: &quot;);
Serial.print(temperature);
Serial.println(&quot; C&quot;);
/* Then convert the atmospheric pressure, and SLP to altitude */
/* Update this next line with the current SLP for better results */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
Serial.print(&quot;Altitude: &quot;);
Serial.print(bmp.pressureToAltitude(seaLevelPressure,
event.pressure));
//Serial.println(&quot; m&quot;);
Serial.println(&quot;&quot;);


Serial.print(&quot;LPG:&quot;);
Serial.print(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_LPG) );
Serial.print( &quot;ppm&quot; );
Serial.print(&quot; &quot;);
Serial.print(&quot;CO:&quot;);
Serial.print(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO) );
Serial.print( &quot;ppm&quot; );
Serial.print(&quot; &quot;);
Serial.print(&quot;SMOKE:&quot;);
Serial.print(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_SMOKE) );
Serial.print( &quot;ppm&quot; );
Serial.print(&quot;&quot;);
delay(200);
Serial.println(&quot;&quot;);


}
else
{
Serial.println(&quot;Sensor error&quot;);
}
delay(1000);
 http.begin("http://rajeshwebproject2.000webhostapp.com/receiver.php?temperature='"+temperature+"'&pressure='"+event.pressure+"'&gas='"+MQGetGasPercentage()+"'&humidity='"+DHT.humidity+"'");
}}
