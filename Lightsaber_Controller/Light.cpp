#include "Light.h"
#include "Arduino.h"
#include "Config.h"

void fade_OffToOn(uint8_t lightpower){
    uint8_t cont = 0;
    while(cont < lightpower){
         analogWrite(pin_light,cont);
         delay(2);
         cont++;
      }
}

uint16_t fade_OnToOff(uint8_t lightpower){
    uint8_t cont = lightpower;
    while(lightpower != 0){
          analogWrite(pin_light,lightpower);
          lightpower--;
          delay(4);
      }
  digitalWrite(pin_light,LOW);
  return (cont + 1) * 4;
}

void LockUp(uint8_t lightpower){
      analogWrite(pin_light, 1);
      delay (10);
      if(lightpower < 50) analogWrite(pin_light, random(1 ,lightpower + 50));
      else if(lightpower > 205) analogWrite(pin_light, random(lightpower - 50 , 255 ));
      else analogWrite(pin_light, random(lightpower - 50 , lightpower + 50 ));
      delay (30);
  }
  
void Flicker_Effect(uint8_t lightpower){
    if(lightpower < 50) analogWrite(pin_light, random(1 ,lightpower + 60));
    else if(lightpower > 205) analogWrite(pin_light, random(lightpower - 60 , 255 ));
    else analogWrite(pin_light, random(lightpower - 60 , lightpower + 60 ));
   }

void CristalPulse(){
    analogWrite(pin_cristal,((1 + cos((millis()*3.1415926535)/CristalPeriod))/2) * 255);
  }

uint8_t clashFlash(uint8_t lightpower){
   analogWrite(pin_light,1);
   delay(10);
   analogWrite(pin_light,255); 
   delay(flash_time); 
   analogWrite(pin_light,lightpower);
   return 40;
  }
