#define Motor 8
#define Motor2 9
#define MuxEn 7
#define NCHAN 16
#define SLAVE_ADDRESS 0x04

#define WAITDEFAULT 600
#define DT 1

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


//                  s0,s1,s2,s3
int controlPin[] = {5 ,4 ,3 ,2 }; //          x1              x2                  x1
//                                1  ,2  ,3  ,4  ,5  ,6  ,7  ,8  ,9  ,10 ,11 ,12 ,13 ,14 ,15 ,16 
unsigned int WaterTime[NCHAN] =  {DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT ,DT};
unsigned char Status[NCHAN] =    {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,1  ,1  ,1  ,1  ,1  ,1  ,1  ,1};
int muxChannel[16][4]={ {0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},
                        {0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},
                        {0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},
                        {0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};
void initMux();
void useMultiplexer();
int readMux(int channel);

void recieveData(int);
void sendData();

bool state;
char number; 
int water;
long waiting;
unsigned char where=0;

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
  
  pinMode(13, OUTPUT);
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  initMux();
  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  pinMode(Motor,OUTPUT);
  digitalWrite(Motor,HIGH);
  pinMode(Motor2,OUTPUT);
  digitalWrite(Motor2,HIGH);
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
  for (waiting=0;waiting<WAITDEFAULT;waiting++)
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
  for(int ic = 0; ic < NCHAN; ic++){
    if(Status[ic]==0) continue;
    where = ic+1;
    readMux(ic);
    for(water=0;water<WaterTime[ic];water++){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("watering ");
      lcd.print(ic+1);
      lcd.setCursor(0,1);
      lcd.print(WaterTime[ic]-water);
      //Serial.print("watering");
      //Serial.print(ic+1);
      //Serial.print(WaterTime[ic]-water);
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
  where=0;
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

int readMux(int channel){ 
  for(int i = 0; i < 4; i ++)
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  return 0;
}

/*
void receiveEvent(int howMany){
  while(Wire.available())
  {
    char c = Wire.read();
    Serial.println(c);
    //
    for (int i=0; i<c;i++)
      {
          char c = Wire.read();
          Serial.print(c); 
      }
    if(c == 72) digitalWrite(13,HIGH);
   else if (c == 76) digitalWrite(13,LOW);
    //
  }
}



void requestEvent(){
  char line[40];
  char dd[10];
  dd[0]=33;
  sprintf(line, "sent! dist= %u", dd[0]);
  Serial.println(line);
  Wire.write(dd); 
  delay(50);
}
*/



void receiveData(int byteCount){
  while(Wire.available()) {
    number = Wire.read();
    
    if (number == '1'){
      if (state == 0){
        digitalWrite(13, HIGH); // set the LED on
        state = 1;
      } else {
        digitalWrite(13, LOW); // set the LED off
        state = 0;
      }
    }
    if(number=='t'){
      number = (WAITDEFAULT-waiting)/60;
    }
    if(number=='w'){
      number = where;
    }
  }
}

void sendData(){
 Wire.write(number);
}


