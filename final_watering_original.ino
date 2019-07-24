#define Motor 8 
#define MuxEn 7
#define WAITDEFAULT 25
#define NCHAN 16

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include "Keyboard.h"

#define DT 1
//184

//                  s0,s1,s2,s3
int controlPin[] = {5 ,4 ,3 ,2 };           //x1  x2                              x1 
//                                1  ,2  ,3  ,4  ,5  ,6  ,7  ,8  ,9  ,10 ,11 ,12 ,13 ,14 ,15 ,16
unsigned int WaterTime[NCHAN] =  {DT ,DT ,DT ,DT ,DT ,DT ,40 ,DT ,DT ,DT ,DT ,40 ,DT ,DT ,DT ,DT};
unsigned char Status[NCHAN] =    {1  ,1  ,1  ,0  ,1  ,1  ,1  ,1  ,1  ,1  ,1  ,1  ,0  ,1  ,1  ,1};
unsigned char HumiSens[NCHAN] = {};
unsigned char TimeLeft[NCHAN] = {};
int muxChannel[16][4]={ {0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},
                        {0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},
                        {0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},
                        {0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};
void initMux();
void useMultiplexer();
int readMux(int channel);

/*
const int sensor_pin = A3; 
float moisture_percentage;
int sensor_analog;
*/


//--------------------------------------------//
//          Begin Setup                       //
//--------------------------------------------//

LiquidCrystal_I2C lcd(0x3F, 16, 2);
void setup(){
  initMux();
  Serial.begin(9600);
  //Keyboard.begin();
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  pinMode(Motor,OUTPUT);
  digitalWrite(Motor,HIGH);
  delay(1000);
  
}


//--------------------------------------------//
//          Begin Loop                        //
//--------------------------------------------//

void loop() 
{
  /*
  char inChar = '1';
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("starting");
  Serial.print("starting\n");
  while(inChar!='a'){
    if (Serial.available() > 0) {
      inChar = Serial.read();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(inChar);
      Serial.print(inChar);
      Serial.print("\n");
    }
  }
  */
  Serial.print("done\n");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("done");
//WAITING-------------------------------------------------------------
  for (long waiting=0;waiting<WAITDEFAULT;waiting++)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("waiting:");
    Serial.print("waiting,");
    lcd.setCursor(0,1);
    lcd.print(WAITDEFAULT-waiting);
    //Serial.print(WAITDEFAULT-waiting);
    /*
    sensor_analog = analogRead(sensor_pin);
    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
    lcd.setCursor(0,2);
    lcd.print("Moisture:");
    //lcd.setCursor(0,3);
    lcd.print(moisture_percentage);
    lcd.print("%");
    */
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("starting to water...");
  Serial.print("starting to water...");
  digitalWrite(Motor,LOW);
  digitalWrite(MuxEn, LOW);
  
  
//WATER---------------------------------------------------------------
  for (int ic = 0; ic < NCHAN; ic++)
  {
    if (Status[ic]==0) continue; // channel is off
    //if(ic!=0) digitalWrite(Port[ic-1],HIGH);
    //digitalWrite(Port[ic],LOW);
    readMux(ic);
    for (int water=0;water<WaterTime[ic];water++)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("watering ");
      Serial.print("watering");
      lcd.print(ic+1);
      Serial.print(ic+1);
      lcd.setCursor(0,1);
      lcd.print(WaterTime[ic]-water);
      Serial.print(WaterTime[ic]-water);
    /*
    sensor_analog = analogRead(sensor_pin);
    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
    lcd.setCursor(0,2);
    lcd.print("Moisture Percentage = ");
    lcd.setCursor(0,3);
    lcd.print(moisture_percentage);
    lcd.print("%");
    */
      delay(1000);
    } 
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("finished watering");
  Serial.print("finished watering");
  digitalWrite(Motor,HIGH);
  digitalWrite(MuxEn, HIGH);
}
/*
Analog Output = \frac{ADCValue}{1023}

Moisture in percentage = 100 – (Analog output * 100)
*/

//--------------------------------------------//
//          Mux Stuff                         //
//--------------------------------------------//


void initMux(){
  pinMode(controlPin[0], OUTPUT);
  pinMode(controlPin[1], OUTPUT);
  pinMode(controlPin[2], OUTPUT);
  pinMode(controlPin[3], OUTPUT);
  
  pinMode(MuxEn, OUTPUT);
  digitalWrite(MuxEn, HIGH);
  
  digitalWrite(controlPin[0], LOW);
  digitalWrite(controlPin[1], LOW);
  digitalWrite(controlPin[2], LOW);
  digitalWrite(controlPin[3], LOW);
}

int readMux(int channel)
{ 
  
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  //return analogRead(SIG_pin);
  return 0;
}

