#include <EEPROM.h>
#include <TinyGPSPlusPlus.h>
#include <SoftwareSerial.h>
 
#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"
 

//drive motors
const int pin1=4,pin2=7,pin3=8,pin4=11,pin5=9,pin6=10;

/******************************** GPS *********************************/


double latitude=0, longitude=0;
int gates=6, current_gate=0, i=0, input5=0, eer=0; 

static const double gates_lat[6]={30.080876,30.081101,30.081120,30.081614,30.081381,30.081332};
static const double gates_lng[6]={31.298010,31.297694,31.298245,31.298135,31.297902,31.298497};


/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
unsigned long distance;
double x =0,y=0,dx,dy;


// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
HMC5883L mag;

int16_t mx, my, mz;
int compass_angle,required_angle;

//#define LED_PIN 13
//bool blinkState = false;

/**********************************************************/
int reach=0;
/******************************/
int currentmillis=0,oldmillis=0,start=0;

void setup()
{
  /*********************************/
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mindcloud Team"));
  Serial.println();
/**********************compass**********************/
// join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
//    Serial.begin(38400);
    // initialize device
    Serial.println("Initializing I2C devices...");
    mag.initialize();
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
    //eer=0;
//   EEPROM.put(eer,0);
 eer=0;
   EEPROM.get(eer,current_gate);

    // configure Arduino LED for
   // pinMode(LED_PIN, OUTPUT);
    /***************************************************/
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
 if (digitalRead(14)==LOW){
  pinMode(pin1,OUTPUT);
 pinMode(pin2,OUTPUT);
 pinMode(pin3,OUTPUT);
 pinMode(pin4,OUTPUT);
 }
 if (digitalRead(14)==HIGH){
  pinMode(pin1,INPUT);
 pinMode(pin2,INPUT);
 pinMode(pin3,INPUT);
 pinMode(pin4,INPUT);
 }
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
          displayInfo();
          check_reach();
          if(reach==0){
             getting_required_angle();
            getting_compass_angle();
            getting_course();
          }
          if (reach==1){reach=0;}
         }
         }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  i=0;
  while (i<100){
  if (gps.location.isValid())
  {
     i=i+1;
    latitude=latitude+ gps.location.lat();
    longitude=longitude+ gps.location.lng();
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  delay(25);
  while(digitalRead(14)==HIGH){
      pinMode(pin1,INPUT);
 pinMode(pin2,INPUT);
 pinMode(pin3,INPUT);
 pinMode(pin4,INPUT);
 pinMode(pin3,INPUT);
 pinMode(pin4,INPUT);
    i =0;
    latitude=0;
    longitude=0;
  }
  }
    i=0;
    latitude/=100;
    longitude/=100;
    Serial.print(F("Location: ")); 
  Serial.print(latitude, 6);
    Serial.print(F(","));
    Serial.print(longitude, 6);
     Serial.println();
     distance=
    (unsigned long)TinyGPSPlus::distanceBetween(
      latitude,
      longitude,
      gates_lat[current_gate], 
      gates_lng[current_gate]);
    printInt(distance, gps.location.isValid(), 9);
Serial.println();
y=latitude;
x=longitude;
 dx=gates_lng[current_gate]-x;dy=gates_lat[current_gate]-y;
  latitude=0;
      longitude=0;
 
if (start==0){
  start=1;
  forward();
  oldmillis=millis();
  currentmillis=millis();
  while(currentmillis<(oldmillis+10000)){
    forward();
    delay(1);
    currentmillis=millis();
    }
}
}
static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void check_reach(){
  if(distance<=5){
    reach=1;
    current_gate++;
    eer=0;
   EEPROM.put(eer,current_gate);
    stp();
    delay(5000);
  }
if (current_gate==gates){
  reach=2;
}
//eer=0;
//   EEPROM.put(eer,0);
}

void getting_compass_angle(){
  // read raw heading measurements from device
    mag.getHeading(&mx, &my, &mz);

    // display tab-separated gyro x/y/z values
    Serial.print("mag:\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.print(mz); Serial.print("\t");
    
// To calculate heading in degrees. 0 degree indicates North
    float heading = atan2(my, mx);
    if(heading < 0)
      heading += 2 * M_PI;
    Serial.print("heading:\t");
    Serial.println(heading * 180/M_PI);
    // blink LED to indicate activity
    //blinkState = !blinkState;
  //  digitalWrite(LED_PIN, blinkState);
   compass_angle=heading * 180/M_PI;
   compass_angle-=100;
   if (compass_angle<0){compass_angle+=360;}
    delay(50);
}
void getting_required_angle(){
if((dx>0)&&(dy>0)){
        required_angle=round(atan2(dy,dx)*180/3.14159265);
       required_angle=90-required_angle;
    }
    if((dx<0)&&(dy>0)){
      dx=dx*(-1.0);
        required_angle=round(atan2(dy,dx)*180/3.14159265);
       required_angle=270+required_angle;
    }
    if((dx>0)&&(dy<0)){
       dy=dy*(-1.0);
        required_angle=round(atan2(dy,dx)*180/3.14159265);
       required_angle=required_angle+90;
    }
    if((dx<0)&&(dy<0)){
      dx=dx*(-1.0);
      dy=dy*(-1.0);
       required_angle=round(atan2(dy,dx)*180/3.14159265);
       required_angle=270-required_angle;
    }
}
void getting_course(){
if (required_angle<10){
      if (compass_angle>349){
        compass_angle=compass_angle-360;
      }
    }
    if (required_angle>349){
      if (compass_angle<10){
        compass_angle=compass_angle+360;
      }
    }
    if((compass_angle<(required_angle+11))&&(compass_angle>(required_angle-11))){
      forward();
    }
    else{
      do{
        if ((required_angle<=179)&&(required_angle>=0)){
          if ((compass_angle>required_angle)&&(compass_angle<=(required_angle+180))){
            turnleft();
          }
          else{
            turnright();
          }
        }
         if ((required_angle>179)&&(required_angle<=359)){
          if ((compass_angle<required_angle)&&(compass_angle>=(required_angle-180))){
            turnright();
          }
          else{
            turnleft();
          }
        }
        getting_compass_angle();
        if (required_angle<10){
     if (compass_angle>349){
        compass_angle=compass_angle-360;
      }
    }
    if (required_angle>349){
      if (compass_angle<10){
        compass_angle=compass_angle+360;
      }
    }
      }while((compass_angle>(required_angle+11))||(compass_angle<(required_angle-11)));
    }
    forward();
    delay(100);
}
void turnright(){
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
  digitalWrite(pin3,HIGH);
  digitalWrite(pin4,LOW);
  analogWrite(pin5,100);
  analogWrite(pin6,255);
}
void turnleft(){
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
  digitalWrite(pin3,HIGH);
  digitalWrite(pin4,LOW);
  analogWrite(pin5,255);
  analogWrite(pin6,100);
}
void backward(){
   digitalWrite(pin1,LOW);
  digitalWrite(pin2,HIGH);
  digitalWrite(pin3,LOW);
  digitalWrite(pin4,HIGH);
  analogWrite(pin5,255);
  analogWrite(pin6,255);
}
void stp(){
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
  digitalWrite(pin3,HIGH);
  digitalWrite(pin4,LOW);
  analogWrite(pin5,0);
  analogWrite(pin6,0);
}
void forward(){
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
  digitalWrite(pin3,HIGH);
  digitalWrite(pin4,LOW);
  analogWrite(pin5,255);
  analogWrite(pin6,255);

}
