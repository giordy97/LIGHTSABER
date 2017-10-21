/**********************************************************************************************************************
*   ARDUINO C CODE TO CONTROL SOUND AND LIGHT EFFECT OF A DIJ SABER WITH ARDUINO PRO MINI AND DFPLAYER                *
*                                                                                                                     *
*   DEVELOPER: COSTI GIORDANO                                                               LAST_UPDATE: 17/10/17     *                                                
*                                                                                                                     *
**********************************************************************************************************************/

#include "DfPlayer.h"
#include "Mpu6050.h"
#include "Light.h"
#include "Config.h"
#include "Menu.h"

//GLOBAL VARIABLES
bool buttonstate = LOW,lightstate = LOW,check = false;
uint8_t numfont,folder,volume,lightpower;
long long time0 = 0;

void setup(){
//INIZIALIZING ARDUINO
 pinMode(pin_button1, INPUT);  digitalWrite(pin_button1, LOW);
#ifndef ONE_BUTTON
 pinMode(pin_button2, INPUT);  digitalWrite(pin_button2, LOW);
#endif
 pinMode(pin_light,  OUTPUT);  digitalWrite(pin_light,   LOW);

//READING FROM EEPROM CONFIG
get_config(&numfont,&folder,&volume,&lightpower);

//INIZIALIZING DFPLAYER
init_DfPlayer(volume);
Play_Folder_Track(folder,track_bootup);

//INIZIALIZING MPU6050
init_mpu6050();
}

void loop(){
buttonstate = digitalRead(pin_button1);       //READ BUTTON STATE

//************************************ LIGHT OFF ************************************//
if(lightstate != HIGH){
//********************************* BUTTON 1 PRESSED *********************************//
   if(buttonstate != LOW){
//---------------------------------------- ONE BUTTON -------------------------------------
#ifdef ONE_BUTTON
    time0 = millis();
    check = false;
    while(digitalRead(pin_button1) == HIGH){              //BUTTON 1 PRESSED FOR MORE THAN 1 SECOND --> MENU
        if((millis()-time0) > 400){
        if (volume == 0) Specify_Volume(5); delay(100);   //CONTROL ON VOLUME
        Play_Folder_Track(99,31); delay(2100);            //MENU VOICE
        while(digitalRead(pin_button1) != LOW){;}         //WHAITING FOR THE BUTTON TO BE RELEASED  
        menu(&numfont,&folder,&volume,&lightpower);       //MENU
        get_config(&numfont,&folder,&volume,&lightpower);
        check = true; buttonstate = LOW; lightstate = LOW;
        break;
        }
    }
    if(!check){
    time0 = millis();
    while((millis()-time0) < 400){
        if (digitalRead(pin_button1) == HIGH){
           while(digitalRead(pin_button1) == HIGH){;}               //WHAITING FOR THE BUTTON TO BE RELEASED FOR THE SECOND TIME
           Play_Rand(track_voice_start,track_voice_end,folder);     //VOICE SOUND
           buttonstate = LOW; lightstate = LOW; check = true; delay(2000);
           break;
          }
    }
         if(!check){
         buttonstate = LOW; lightstate = HIGH;
         Play_Folder_Track(folder,track_on);                //POWER UP SOUND 
         delay(time_on - fade_OffToOn(lightpower));         //FADE LIGHT
         Play_Folder_Track(folder,track_hum);
         delay(100);
         Loop_Current();}
        }
   }
#endif
//-----------------------------------------------------------------------------------------

//---------------------------------------- TWO BUTTON -------------------------------------
/*#ifndef ONE_BUTTON

//BUTTON ONE PRESSED
    while(digitalRead(pin_button1) != LOW);   //WHAITING FOR THE BUTTON TO BE RELEASED
    Play_Track(track_on);                     //POWER UP SOUND 
    buttonstate = LOW;
    lightstate = HIGH;
    delay(time_on - fade_OffToOn(lightpower));          //FADE LIGHT
    Repeat_Track(track_hum); 

// BUTTON 2 PRESSED
  else if(digitalRead(pin_button2)!=LOW){
        time0 = millis();
        check = false;
        while (digitalRead(pin_button2)!=LOW){
          if((millis()-time0) > 1000){
          menu(&numfont,&folder,&volume,&lightpower);    //MENU
          check = true;
          break;
          }
        }
      if(!check){
         Play_Rand(track_voice_start + 1,track_voice_end);                     //POWER UP SOUND 
         buttonstate = LOW; lightstate = HIGH;
        }
  }
}
#endif
//-----------------------------------------------------------------------------------
*/

//LIGHT OFF && NO BUTTON PRESSED
else CristalPulse();

}//LIGHT OFF
//************************************ LIGHT ON && NO BUTTON 1 PRESSED  ************************************//
else{
    if (buttonstate != HIGH){

/*
******************************** TWO BUTTON PRESSED (LOCKUP) *****************************
#ifndef ONE_BUTTON
    if(digitalRead(pin_button2)!=LOW){
     Repeat_Track(track_lockup);
     while(digitalRead(pin_button2)!=LOW){
      LockUp(lightpower);
      }
     Repeat_Track(track_hum);      //RESTART HUM
     }
#endif
*******************************************************************************************/

// FLICKER EFFECT
#ifdef FLICKER
        if(!(millis()%200))
        Flicker(lightpower);
#endif

// CRISTAL ON
digitalWrite(pin_cristal,HIGH);

// SWING AND CLASH DETECTION
         int16_t GyX=0,GyZ=0;
         MPU_Get_Data(&GyX,&GyZ);
         if((abs(GyX) > swingforce) || (abs(GyZ) > swingforce)){
          time0 = millis();
          Play_Rand(track_swing_start,track_swing_end,folder);
          
          while((millis() - time0) < time_swing){
              MPU_Get_Data(&GyX,&GyZ);
              int16_t NEW_GyX=0,NEW_GyZ=0;
              delay(time_swing/6);
              MPU_Get_Data(&NEW_GyX,&NEW_GyZ);
              if( (abs(GyX) > clashforce || abs(GyZ) > clashforce) && (abs(NEW_GyX) > clashrebound || abs(NEW_GyZ) > clashrebound) && (((GyX ^ NEW_GyX) < 0) || ((GyZ ^ NEW_GyZ) < 0))){ 
                Play_Rand(track_clash_start,track_clash_end,folder);
                delay(time_clash);
                break;
              }
            }
          Play_Folder_Track(folder,track_hum);
          delay(100);
          Loop_Current();
          }
                
   }
//***************************** BUTTON 1 PRESSED LIGHT ON --> TURN OFF ************************************//
  else{
//--------------------------------------- ONE BUTTON -----------------------------------
#ifdef ONE_BUTTON
      time0 = millis();
      check = false;
      while(digitalRead(pin_button1) != LOW){                  //WHAITING FOR THE BUTTON TO BE RELEASED
        if((millis()-time0) > 300){
          Play_Folder_Track(folder,track_lockup); delay(100);
          Loop_Current();
          while(digitalRead(pin_button1)!= LOW) LockUp(lightpower);
          Play_Folder_Track(folder,track_hum); delay(100);     //RESTART HUM
          Loop_Current();
          check = true;
          break;
          }
      }
      if(!check){
       buttonstate = LOW; lightstate = LOW;
       Play_Folder_Track(folder,track_off);
       delay(time_off - fade_OnToOff(lightpower));
      }
#endif
//--------------------------------------------------------------------------------------

/*
//--------------------------------------- TWO BUTTON -----------------------------------
#ifndef ONE_BUTTON
       while(digitalRead(pin_button1) != LOW);      //WHAITING FOR THE BUTTON TO BE RELEASED
       buttonstate = LOW;
       lightstate = LOW;
       Play_Track(track_off);
       delay(time_off - fade_OnToOff(lightpower));
#endif
//--------------------------------------------------------------------------------------
 */
     }
 }//LIGHT ON
}//MAIN LOOP
