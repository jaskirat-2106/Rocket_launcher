#include <LiquidCrystal_I2C.h>

const int lcdI2CAddress = 0x27;
const int lcdCols = 2;
const int lcdRows = 2;

LiquidCrystal_I2C lcd(lcdI2CAddress, lcdCols, lcdRows);

const int potread = A0;
const int buzzpin = 2;

unsigned long launchT;
float temperature_C = 20.0;
int tempmax;
float altitude = 0.0;
float acceleration = 0.0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(buzzpin, OUTPUT);
  noTone(buzzpin);

  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("GROUND STATION");
  lcd.setCursor(0, 1);
  lcd.print("LAUNCH 3..2..1"); 
  delay(3000);
  launchT = millis();
}

void loop() {
  int potval = analogRead(potread);
  tempmax = map(potval, 0, 1023, 20, 80);
  Serial.print("tempmax:");
  Serial.print(tempmax);
  Serial.println(" C");    /*adjust max temp from potentiometer 
                            i hv printed in serial moniter maxtemp 
                            for refrence, by defualt i hv set it 60°C */

  unsigned long elapsedtime = millis() - launchT;
  
  
  int maxalt = 1000;
  float speedalt = 108.86;
  int altup = 0;
  if (altup == 0) {
    altup = launchT;}
  if (millis() - altup > 1000) { 
    altitude += (maxalt * (1.0 / (speedalt))); 
    altup = millis();
  }
  if (altitude >= 1000.0) { 
    altitude = 1000.0;
  }
  
  
  int accup = 0;                /* i coded so acceleration stops at critical temp
                                 and alt=1000 as mentioned in pdf*/
  if (accup == 0){   
     accup = launchT;}     
  float lastacc;
  if (acceleration < 50.0) {
    if (millis() - accup >= 1000) { 
      acceleration += 0.45;
      lastacc=acceleration;
      accup = millis();
      if (temperature_C > tempmax){
        acceleration=lastacc;
      }     
    }
  } 
  
  

  int tempup = 0;
  if (tempup == 0){
    tempup = launchT;}
  if (millis() - tempup >= 1000) {
    temperature_C += 0.55;
    tempup = millis();
  } 

                                /* i coded so acceleration stops at critical temp
                                 and alt=1000 as mentioned in pdf*/

  lcd.setCursor(0, 0);           /* in LCD i tried to save as much 
                                    space as possible since its 16X2
                                    that includes shortforms and  
                                    units not visible, everything in full 
                                    form and units is in serial moniter*/
  lcd.print("tm:");
  lcd.print(elapsedtime / 1000);
  Serial.print("Time(s):");
  Serial.print(elapsedtime / 1000);
  Serial.println("s");
  
  lcd.print("s alt:");
  lcd.print(altitude);
  lcd.print("m");
  Serial.print("Altitude(m):");
  Serial.print(altitude);
  Serial.println("m");
  
  lcd.setCursor(0, 1);
  lcd.print("a:");
  lcd.print(acceleration, 1);
  Serial.print("Acceleration(m/s^2):");
  Serial.print(acceleration);
  Serial.println(" m/s^2");
  
  lcd.print(" T:");
  lcd.print(temperature_C);
  lcd.print(" C");
  Serial.print("Temperature(°C):");
  Serial.print(temperature_C);
  Serial.println(" C");
  Serial.println("-------------------");/* in serial moniter values,
                                           are prety fast as they are
                                           updating again and again
                                           but in lcd i hv made it 
                                           constant so its easy to read*/

  if (temperature_C > tempmax) {
    tone(buzzpin, 1000);
    Serial.println("CRITICAl SYSTEM FAILURE");
  } else {
    noTone(buzzpin);
  }

  delay(200);
}
