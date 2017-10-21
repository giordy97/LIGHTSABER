#include "Light.h"
#include "Arduino.h"
#include "Config.h"

uint8_t fade_OffToOn(uint8_t lightpower){
    uint8_t cont = 0;
    while(cont < lightpower){
          analogWrite(pin_light,cont);
          delay(1);
          cont++;
      }
  return cont;
}

uint16_t fade_OnToOff(uint8_t lightpower){
    uint8_t cont = lightpower;
    while(lightpower != 0){
          analogWrite(pin_light,lightpower);
          lightpower--;
          delay(5);
      }
  digitalWrite(pin_light,LOW);
  return (cont + 1) * 5;
}

void LockUp(uint8_t lightpower){
      analogWrite(pin_light, 1);
      delay (10);
      if(lightpower < 50) analogWrite(pin_light, random(0 ,lightpower + 50));
      else if(lightpower > 205) analogWrite(pin_light, random(lightpower - 50 , 255 ));
      else analogWrite(pin_light, random(lightpower - 50 , lightpower + 50 ));
      delay (30);
  }
  
void Flicker(uint8_t lightpower){
    if(lightpower < 25) analogWrite(pin_light, random(0 ,lightpower + 25));
    else if(lightpower > 230) analogWrite(pin_light, random(lightpower - 25 , 255 ));
    else analogWrite(pin_light, random(lightpower - 25 , lightpower + 25 ));
  }

void CristalPulse(){
    analogWrite(pin_cristal,((1 + cos((millis()*3.1415926535)/CristalPeriod))/2) * 255);
  }
  
