#include <EEPROM.h>
#include "Menu.h"
#include "Config.h"
#include "Dfplayer.h"
#include "Mpu6050.h"

void get_config(uint8_t * numfont,uint8_t * folder,uint8_t * volume,uint8_t * lightpower){
if(EEPROM[0]!= 'C'){
     EEPROM[0] = 'C';
     EEPROM[1] = 4;
     EEPROM[2] = 1;
     EEPROM[3] = 15; 
     EEPROM[4] = 100;
     }
     
else{
     (*numfont) = EEPROM[1];
     (*folder) = EEPROM[2];
     (*volume) = EEPROM[3];
     (*lightpower) = EEPROM[4];   
     }
}

void set_config(uint8_t numfont,uint8_t folder,uint8_t volume,uint8_t lightpower){    
     if (EEPROM[1] != numfont) EEPROM[1] = numfont;
     if (EEPROM[2] != folder) EEPROM[2] = folder;
     if (EEPROM[3] != volume) EEPROM[3] = volume; 
     if (EEPROM[4] != lightpower) EEPROM[4] = lightpower;
   }

//_____________________________________MENU___________________________________________

void menu(uint8_t* numfont,uint8_t* folder,uint8_t* volume,uint8_t* lightpower){
     int16_t GyY = 0;
     bool next = false;
     digitalWrite(pin_cristal,HIGH); //TURN ON CRISTAL
//-----------------------SELECT VOLUME-----------------------
     if(((*volume)%5) != 0) (*volume) = 10; // Control
     Play_Folder_Track(99,32); delay(1200);//SELECT VOLUME VOICE
     while(!next){
       
       if((*volume)!=0){Specify_Volume((*volume));delay(100);}
       else{ Specify_Volume(5);delay(100); }
       Play_Folder_Track(99,(*volume));delay(150);
       GyY=0;
       while(rotation_force > abs(GyY)){
       if(digitalRead(pin_button1)){ next = true; break;}
       MPU_Get_Rotation(&GyY);
       if((GyY > 0) && (GyY > rotation_force) && ((*volume) != 30)) (*volume) += 5;
       else if((GyY < 0) && (GyY < -rotation_force) && ((*volume) != 0)) (*volume) -= 5;
       }
     }
//-----------------------SELECT LIGHT POWER-----------------------
     next = false;
     Play_Folder_Track(99,33); delay(1400);
     while(!next){
       analogWrite(pin_light,(*lightpower)); delay(500);
       GyY=0;
       while(rotation_force > abs(GyY)){
       if(digitalRead(pin_button1)){ next = true; break; digitalWrite(pin_button1,LOW); }
       MPU_Get_Rotation(&GyY);
       if((GyY > 0) && GyY > rotation_force){ if((*lightpower) >= 230){(*lightpower) = 255; Play_Folder_Track(99,30);}else (*lightpower) += 25; }
       else if((GyY < 0) && GyY < -rotation_force){ if((*lightpower) <= 25){ (*lightpower) = 0; Play_Folder_Track(99,0);}else (*lightpower) -= 25; }
     }
    }
//-----------------------SELECT FONT-----------------------
     digitalWrite(pin_light,LOW);            // TURN OFF LIGHT
     next = false;
     Play_Folder_Track(99,34); delay(1300);  //SELECT FONT SOUND
     while(!next){
       Play_Folder_Track((*folder),track_bootup);
       delay(400);
       GyY=0;
       while(rotation_force > abs(GyY)){
       if(digitalRead(pin_button1)){ next = true; break;} 
       MPU_Get_Rotation(&GyY);
       if((GyY > 0) && (GyY > rotation_force) && ((*folder) != (*numfont))) (*folder)+=1;
       else if((GyY < 0) && (GyY < -rotation_force) && ((*folder) != 1)) (*folder)-=1;
       }
     }
     
//-----------------------PUTTING CONFIG ON EEPROM-----------------------
Play_Folder_Track(99,35); delay(10000); //SAVING
if((*volume)==0)Specify_Volume(0);
set_config((*numfont),(*folder),(*volume),(*lightpower));
}

