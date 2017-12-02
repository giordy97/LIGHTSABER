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
uint8_t numfont=FN,folder=FF,volume=FV,lightpower=FL,flicker=FK;
uint32_t time0;


void setup(){
//INIZIALIZING ARDUINO
 pinMode(pin_button1, INPUT);  digitalWrite(pin_button1, LOW);
#ifndef ONE_BUTTON
 pinMode(pin_button2, INPUT);  digitalWrite(pin_button2, LOW);
#endif
 pinMode(pin_light,  OUTPUT);  digitalWrite(pin_light,   LOW);

//READING FROM EEPROM CONFIG
get_config(&numfont,&folder,&volume,&lightpower,&flicker);

//INIZIALIZING DFPLAYER
init_DfPlayer(volume);

//INIZIALIZING MPU6050
init_mpu6050();

//SABER READY
Play_Folder_Track(folder,track_bootup);
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
    while(digitalRead(pin_button1) == HIGH){                       //BUTTON 1 PRESSED FOR MORE THAN 1 SECOND --> MENU
        if((millis()-time0) > 500){
        if (volume == 0) Specify_Volume(5); delay(100);            //CONTROL ON VOLUME
        Play_Folder_Track(99,31); delay(2100);                     //MENU VOICE
        while(digitalRead(pin_button1) != LOW){;}                  //WHAITING FOR THE BUTTON TO BE RELEASED  
        menu(&numfont,&folder,&volume,&lightpower,&flicker);       //MENU
        get_config(&numfont,&folder,&volume,&lightpower,&flicker);
        check = true; buttonstate = LOW; lightstate = LOW;
        break;
        }
    }
    time0 = millis();
    delay(50); //BUTTONS REBOUND
    if(!check){
    while((millis()-time0) < 250){
        if (digitalRead(pin_button1) == HIGH){
           while(digitalRead(pin_button1) == HIGH){;}               //WHAITING FOR THE BUTTON TO BE RELEASED FOR THE SECOND TIME
           Play_Rand(track_voice_start,track_voice_end,folder);     //VOICE SOUND
           buttonstate = LOW; lightstate = LOW; check = true; delay(2000);
           break;
          }
    }
         if(!check){
         buttonstate = LOW; lightstate = HIGH;
         digitalWrite(pin_cristal,HIGH);                    // CRISTAL ON
         Play_Folder_Track(folder,track_on);                //POWER UP SOUND 
         time0 = millis();                                  //FADE LIGHT
         fade_OffToOn(lightpower);
         while((millis()-time0) < time_on ){
           if((flicker!=0) && !(millis()%10))
           Flicker_Effect(lightpower);
          }
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
      int32_t GyX,GyZ,AcX,AcZ;

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
if((flicker != 0) && (millis()%10))  Flicker_Effect(lightpower);

//------------ SWING AND CLASH DETECTION VERSION ---------------
         MPU_Get_Data(&AcX,&AcZ,&GyX,&GyZ);

         //CLASH CONTROL
         if(((AcX*AcX) + (AcZ*AcZ) > clash_force) && (((GyX*GyX) + (GyZ*GyZ)) < (700000000))){
            time0 = millis();
            Play_Rand(track_clash_start,track_clash_end,folder);
            clashFlash(lightpower);
            while((millis()-time0) < time_clash){
              if((flicker!=0) && !(millis()%10)) Flicker_Effect(lightpower);
              }            
            Play_Folder_Track(folder,track_hum);
            delay(80);
            Loop_Current();}

         //SWING CONTROL
         else if(((GyX*GyX) + (GyZ*GyZ)) > swing_force){
          time0 = millis();
          Play_Rand(track_swing_start,track_swing_end,folder);
          delay(100);   //LITTLE DELAY AFTER SWING SOUND
          while((millis()-time0) < time_swing){
          if((flicker!=0) && !(millis()%10)) Flicker_Effect(lightpower);
          MPU_Get_Acc(&AcX,&AcZ);
            //CLASH CONTROL DURING SWING
            if((AcX*AcX) + (AcZ*AcZ) > clash_force){
             time0 = millis();
             Play_Rand(track_clash_start,track_clash_end,folder);
             clashFlash(lightpower);
             while((millis()-time0) < time_clash) if((flicker!=0) && !(millis()%10)) Flicker_Effect(lightpower);
             break;
            }
          }
        Play_Folder_Track(folder,track_hum);
        delay(80);
        Loop_Current();
        delay(80);
        }
                
   }
//***************************** BUTTON 1 PRESSED LIGHT ON --> TURN OFF ************************************//
  else{
//--------------------------------------- ONE BUTTON -----------------------------------
#ifdef ONE_BUTTON
      time0 = millis();
      check = false;
      while(digitalRead(pin_button1) != LOW){                  //WHAITING FOR THE BUTTON TO BE RELEASED
        if((millis()-time0) > 200){
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
