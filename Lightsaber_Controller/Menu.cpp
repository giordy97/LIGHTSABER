#include "Menu.h"
#include <EEPROM.h>

void get_config(uint8_t* folder,uint8_t* volume,uint8_t* lightpower,uint8_t* flicker, delays* actual_delays){
if(EEPROM[0]!= 'C'){
     EEPROM[0] = 'C';
     EEPROM[1] = (*folder);
     EEPROM[2] = (*volume); 
     EEPROM[3] = (*lightpower);
     EEPROM[4] = (*flicker);
     EEPROM[5]  = lowByte(time_on_1); EEPROM[6]  = highByte(time_on_1);
     EEPROM[7]  = lowByte(time_off_1); EEPROM[8]  = highByte(time_off_1);
     EEPROM[9]  = lowByte(time_clash_1); EEPROM[10] = highByte(time_clash_1);
     EEPROM[11] = lowByte(time_swing_1); EEPROM[12] = highByte(time_swing_1);
     EEPROM[13] = lowByte(time_swing_long_1); EEPROM[14] = highByte(time_swing_long_1);

     EEPROM[15]  = lowByte(time_on_2); EEPROM[16]  = highByte(time_on_2);
     EEPROM[17]  = lowByte(time_off_2); EEPROM[18]  = highByte(time_off_2);
     EEPROM[19]  = lowByte(time_clash_2); EEPROM[20] = highByte(time_clash_2);
     EEPROM[21] = lowByte(time_swing_2); EEPROM[22] = highByte(time_swing_2);
     EEPROM[23] = lowByte(time_swing_long_2); EEPROM[24] = highByte(time_swing_long_2);

     EEPROM[25]  = lowByte(time_on_3); EEPROM[26]  = highByte(time_on_3);
     EEPROM[27]  = lowByte(time_off_3); EEPROM[28]  = highByte(time_off_3);
     EEPROM[29]  = lowByte(time_clash_3); EEPROM[30] = highByte(time_clash_3);
     EEPROM[31] = lowByte(time_swing_3); EEPROM[32] = highByte(time_swing_3);
     EEPROM[33] = lowByte(time_swing_long_3); EEPROM[34] = highByte(time_swing_long_3);

     EEPROM[35]  = lowByte(time_on_4); EEPROM[36]  = highByte(time_on_4);
     EEPROM[37]  = lowByte(time_off_4); EEPROM[38]  = highByte(time_off_4);
     EEPROM[39]  = lowByte(time_clash_4); EEPROM[40] = highByte(time_clash_4);
     EEPROM[41] = lowByte(time_swing_4); EEPROM[42] = highByte(time_swing_4);
     EEPROM[43] = lowByte(time_swing_long_4); EEPROM[44] = highByte(time_swing_long_4);

     EEPROM[45]  = lowByte(time_on_5); EEPROM[46]  = highByte(time_on_5);
     EEPROM[47]  = lowByte(time_off_5); EEPROM[48]  = highByte(time_off_5);
     EEPROM[49]  = lowByte(time_clash_5); EEPROM[50] = highByte(time_clash_5);
     EEPROM[51] = lowByte(time_swing_5); EEPROM[52] = highByte(time_swing_5);
     EEPROM[53] = lowByte(time_swing_long_5); EEPROM[54] = highByte(time_swing_long_5);
     }
else{
     (*folder) = EEPROM[1];
     (*volume) = EEPROM[2];
     (*lightpower) = EEPROM[3];
     (*flicker) = EEPROM[4];
      actual_delays->time_on = EEPROM[(*folder-1)*10 +5] + (EEPROM[(*folder-1)*10 +6]<<8);
      actual_delays->time_off = EEPROM[(*folder-1)*10 +7] + (EEPROM[(*folder-1)*10 +8]<<8);
      actual_delays->time_clash = EEPROM[(*folder-1)*10 +9] + (EEPROM[(*folder-1)*10 +10]<<8);
      actual_delays->time_swing = EEPROM[(*folder-1)*10 +11] + (EEPROM[(*folder-1)*10 +12]<<8);
      actual_delays->time_swing_long = EEPROM[(*folder-1)*10 +13] + (EEPROM[(*folder-1)*10 +14]<<8);
     }
}

void set_config(uint8_t folder,uint8_t volume,uint8_t lightpower,uint8_t flicker){    
     if (EEPROM[1] != folder) EEPROM[1] = folder;
     if (EEPROM[2] != volume) EEPROM[2] = volume; 
     if (EEPROM[3] != lightpower) EEPROM[3] = lightpower;
     if (EEPROM[4] != flicker) EEPROM[4] = flicker;
   }

//_____________________________________MENU___________________________________________

void menu(uint8_t* folder,uint8_t* volume,uint8_t* lightpower,uint8_t* flicker){
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

     Play_Folder_Track(99,34); delay(1400);
     analogWrite(pin_light,(*lightpower));
     
     while(!digitalRead(pin_button1)){
       if( (*flicker) && !(millis()%10)) Flicker_Effect((*lightpower)); // 1 con flicker
       MPU_Get_Rotation(&GyY);
       if(GyY > rotation_force){ (*flicker) = 255; }
       else if(GyY < (-rotation_force)){ (*flicker) = 0; }
    }
     digitalWrite(pin_light,LOW);            // TURN OFF LIGHT
    
//-----------------------SELECT FONT-----------------------
     Play_Folder_Track(99,35); delay(1300);  //SELECT FONT SOUND
     while(!digitalRead(pin_button1)){
       Play_Folder_Track((*folder),track_bootup); delay(400);
       do{
        if(digitalRead(pin_button1)) break;
        MPU_Get_Rotation(&GyY);
        if((GyY > rotation_force) && ((*folder) < FN)) (*folder)+=1;
        else if((GyY < -rotation_force) && ((*folder) > 1)) (*folder)-=1;
        }while(rotation_force > abs(GyY));
     }
//-----------------------PUTTING CONFIG ON EEPROM-----------------------
Play_Folder_Track(99,36);   //SAVING 
set_config((*folder),(*volume),(*lightpower),(*flicker));
delay(9000); 
if((*volume)==0)Specify_Volume(0);
}
