#include "Menu.h"
#include <EEPROM.h>
#include "Config.h"
#include "Dfplayer.h"
#include "Mpu6050.h"
#include "Light.h"

void get_config(uint8_t* numfont,uint8_t* folder,uint8_t* volume,uint8_t* lightpower,uint8_t* flicker){
if(EEPROM[0]!= 'C'){
     EEPROM[0] = 'C';
     EEPROM[1] = (*numfont);
     EEPROM[2] = (*folder);
     EEPROM[3] = (*volume); 
     EEPROM[4] = (*lightpower);
     EEPROM[5] = (*flicker);
     }
     
else{
     (*numfont) = EEPROM[1];
     (*folder) = EEPROM[2];
     (*volume) = EEPROM[3];
     (*lightpower) = EEPROM[4];
     (*flicker) = EEPROM[5];   
     }
}

void set_config(uint8_t numfont,uint8_t folder,uint8_t volume,uint8_t lightpower,uint8_t flicker){    
     if (EEPROM[1] != numfont) EEPROM[1] = numfont;
     if (EEPROM[2] != folder) EEPROM[2] = folder;
     if (EEPROM[3] != volume) EEPROM[3] = volume; 
     if (EEPROM[4] != lightpower) EEPROM[4] = lightpower;
     if (EEPROM[5] != flicker) EEPROM[5] = flicker;
   }

//_____________________________________MENU___________________________________________

void menu(uint8_t* numfont,uint8_t* folder,uint8_t* volume,uint8_t* lightpower,uint8_t* flicker){
     int16_t GyY = 0;
     digitalWrite(pin_cristal,HIGH); //TURN ON CRISTAL
//-----------------------SELECT VOLUME-----------------------
     if(((*volume)%5) != 0) (*volume) = 10; // Control
     Play_Folder_Track(99,32); delay(1200);//SELECT VOLUME VOICE
     while(!digitalRead(pin_button1)){
       if((*volume)!=0){Specify_Volume((*volume));delay(100);}
       else{ Specify_Volume(5);delay(100); }
       Play_Folder_Track(99,(*volume));delay(200);
       do{
       if(digitalRead(pin_button1)) break;
       MPU_Get_Rotation(&GyY);
       if((GyY > 0) && (GyY > rotation_force) && ((*volume) != 30)) (*volume) += 5;
       else if((GyY < 0) && (GyY < -rotation_force) && ((*volume) != 0)) (*volume) -= 5;
       }while(rotation_force > abs(GyY));
     }
//-----------------------SELECT LIGHT POWER-----------------------
     Play_Folder_Track(99,33); delay(1400);
     while(!digitalRead(pin_button1)){
       analogWrite(pin_light,(*lightpower)); delay(300);
       do{
       if(digitalRead(pin_button1)) break;
       MPU_Get_Rotation(&GyY);
       if((GyY > 0) && GyY > rotation_force){ if((*lightpower) >= 230){(*lightpower) = 255; Play_Folder_Track(99,30);}else (*lightpower) += 25; }
       else if((GyY < 0) && GyY < -rotation_force){ if((*lightpower) <= 25){ (*lightpower) = 0; Play_Folder_Track(99,0);}else (*lightpower) -= 25; }
     }while(rotation_force > abs(GyY));
    }
     digitalWrite(pin_light,LOW);            // TURN OFF LIGHT
//-----------------------SELECT FLICKER EFFECT-----------------------

     Play_Folder_Track(99,33); delay(1400);
     analogWrite(pin_light,(*lightpower));
     
     while(!digitalRead(pin_button1)){
       if( (*flicker) && !(millis()%10)) Flicker_Effect((*lightpower)); // 1 con flicker
       MPU_Get_Rotation(&GyY);
       if(GyY > rotation_force){ (*flicker) = 255; }
       else if(GyY < (-rotation_force)){ (*flicker) = 0; }
    }
     digitalWrite(pin_light,LOW);            // TURN OFF LIGHT
    
//-----------------------SELECT FONT-----------------------
     Play_Folder_Track(99,34); delay(1300);  //SELECT FONT SOUND
     while(!digitalRead(pin_button1)){
       Play_Folder_Track((*folder),track_bootup); delay(400);
       do{
        if(digitalRead(pin_button1)) break;
        MPU_Get_Rotation(&GyY);
        if((GyY > rotation_force) && ((*folder) != (*numfont))) (*folder)+=1;
        else if((GyY < -rotation_force) && ((*folder) != 1)) (*folder)-=1;
        }while(rotation_force > abs(GyY));
     }
//-----------------------PUTTING CONFIG ON EEPROM-----------------------
Play_Folder_Track(99,35);   //SAVING 
set_config((*numfont),(*folder),(*volume),(*lightpower),(*flicker));
delay(9000); 
if((*volume)==0)Specify_Volume(0);
}
