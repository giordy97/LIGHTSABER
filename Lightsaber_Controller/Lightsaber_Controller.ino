/**********************************************************************************************************************
    ARDUINO C CODE TO CONTROL SOUND AND LIGHT EFFECT OF A DIY SABER WITH ARDUINO PRO MINI AND DFPLAYER
*                                                                                                                     *
    DEVELOPER: COSTI GIORDANO                                                               LAST_UPDATE: 29/10/19
*                                                                                                                     *
**********************************************************************************************************************/

#include "DfPlayer.h"
#include "Mpu6050.h"
#include "Light.h"
#include "Config.h"
#include "Menu.h"

//GLOBAL VARIABLES
//SABER STATE
bool buttonstate = LOW, lightstate = LOW, doubleClickCheck = false, humPlaying = false, swinging = false;
//EEPROM ACTUAL CONFIG
uint8_t folder = FF, volume = FV, lightpower = FL, flicker = FK;
//TIMER VARIABLE
uint32_t time0 = 0, delayToHum = 0;

void setup() {
  //READING FROM EEPROM ACTUAL CONFIG
  get_config(&folder, &volume, &lightpower, &flicker);

  //INIZIALIZING DFPLAYER
  init_DfPlayer(volume);

  //PLAY BOOTUP SOUND
  Play_Folder_Track(folder, track_bootup);

  //INIZIALIZING MPU6050
  init_mpu6050();

  //INIZIALIZING ARDUINO
  pinMode(pin_button1, INPUT);  digitalWrite(pin_button1, LOW);
  pinMode(pin_light,  OUTPUT);  digitalWrite(pin_light,   LOW);

#ifndef ONE_BUTTON
  pinMode(pin_button2, INPUT);  digitalWrite(pin_button2, LOW);
#endif
}

void loop() {
  buttonstate = digitalRead(pin_button1);       //READ BUTTON STATE

  //************************************ LIGHT OFF ************************************//
  if (lightstate != HIGH) {
    //********************************* BUTTON 1 PRESSED *********************************//
    if (buttonstate != LOW) {
      //---------------------------------------- ONE BUTTON -------------------------------------
#ifdef ONE_BUTTON
      time0 = millis();
      doubleClickCheck = false;
      while (digitalRead(pin_button1) == HIGH) {                     //BUTTON 1 PRESSED FOR MORE THAN 1 SECOND --> MENU
        if ((millis() - time0) > open_menu_time) {
          if (volume == 0) {
            Specify_Volume(5);  //IF VOLUME IS ZERO SET TO MINIMUM BEFORE ENTERING MENU
            delay(100);
          }
          Play_Folder_Track(99, 31); delay(2100);                    //MENU VOICE
          while (digitalRead(pin_button1) != LOW) {
            ; //WHAITING FOR THE BUTTON TO BE RELEASED BEFORE ENTER THE MENU
          }
          menu(&folder, &volume, &lightpower, &flicker);   //MENU
          get_config(&folder, &volume, &lightpower, &flicker); //SAVE SETTINGS ON EEPROM
          doubleClickCheck = true;
          buttonstate = LOW;
          lightstate = LOW;
          break;
        }
      }
      delay(50);
      time0 = millis();
      if (!doubleClickCheck) {
        while ((millis() - time0) < double_click_time) {
          if (digitalRead(pin_button1) == HIGH) {
            while (digitalRead(pin_button1) == HIGH) {
              ; //WHAITING FOR THE BUTTON TO BE RELEASED FOR THE SECOND TIME
            }
            Play_Rand(track_voice_start, track_voice_end, folder);   //VOICE SOUND
            buttonstate = LOW; lightstate = LOW;
            doubleClickCheck = true;
            delay(300);
            break;
          }
        }
        if (!doubleClickCheck) {
          Play_Folder_Track(folder, track_on);               //POWER UP SOUND
          time0 = millis();                                  //FADE LIGHT
          buttonstate = LOW; lightstate = HIGH;              //SETTING STATE VARIABLE
          digitalWrite(pin_cristal, HIGH);                   // CRISTAL ON
          delayToHum = time_on;                              //SET DELAY TO HUM AS TIME TO POWER ON TRACK
          fade_OffToOn(lightpower);                          //LIGHT UP SABER
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
              doubleClickCheck = false;
              while (digitalRead(pin_button2)!=LOW){
                if((millis()-time0) > 1000){
                menu(&folder,&volume,&lightpower);    //MENU
                doubleClickCheck = true;
                break;
                }
              }
            if(!doubleClickCheck){
               Play_Rand(track_voice_start + 1,track_voice_end);                     //POWER UP SOUND
               buttonstate = LOW; lightstate = HIGH;
              }
        }
        }
        #endif
        //-----------------------------------------------------------------------------------
      */
    }
    //LIGHT OFF && NO BUTTON PRESSED
    else CristalPulse();      //KEEP CRISTAL FLASH ALIVE

  }//LIGHT OFF
  //************************************ LIGHT ON && NO BUTTON 1 PRESSED  ************************************//
  else {
    if (buttonstate != HIGH) {

      // CHECK SWING SOUND IS ENDED
      if (swinging && (millis() - time0) > time_swing)
        swinging = false;

      // CHECK OTHER SOUNDS ARE ENDED AND RESTART HUM
      else if (!humPlaying && ((millis() - time0) >= delayToHum)) { //RESTART HUM SOUND
        Play_Folder_Track(folder, track_hum);
        delay(100);    //RESTART HUM
        Loop_Current();
        humPlaying = true;
      }

      // FLICKER EFFECT
      if ((flicker != 0) && (millis() % 10))  Flicker_Effect(lightpower);

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
      *********************************************************************************************/

      //GYRO AND ACCELLEROMETER VARIABLE
      int32_t GyX, GyZ, AcX, AcZ;

      //------------ SWING AND CLASH DETECTION VERSION ---------------
      MPU_Get_Data(&AcX, &AcZ, &GyX, &GyZ);

      //CLASH CONTROL
      if (((AcX * AcX) + (AcZ * AcZ) > clash_force) && (((GyX * GyX) + (GyZ * GyZ)) < (700000000))) {
        Play_Rand(track_clash_start, track_clash_end, folder);
        time0 = millis();
        delayToHum = time_clash;
        humPlaying = false;
        clashFlash(lightpower); //DELAY IS IN THIS FUNCTION
      }

      //SWING CONTROL
      else if (!swinging && (((GyX * GyX) + (GyZ * GyZ)) > swing_force)) {
        Play_Rand(track_swing_start, track_swing_end, folder);
        time0 = millis();
        delayToHum = time_swing_long;
        humPlaying = false;
        delay(100);   //LITTLE DELAY AFTER PLAY SOUND FOR DFPLAYER
      }

    }
    //***************************** BUTTON 1 PRESSED LIGHT ON --> TURN OFF ************************************//
    else {
      //--------------------------------------- ONE BUTTON -----------------------------------
#ifdef ONE_BUTTON
      time0 = millis();
      doubleClickCheck = false;
      while (digitalRead(pin_button1) != LOW) {                //WHAITING FOR THE BUTTON TO BE RELEASED
        if ((millis() - time0) > lockup_time) {
          Play_Folder_Track(folder, track_lockup);
          delay(100);
          Loop_Current();
          while (digitalRead(pin_button1) != LOW){
            LockUp(lightpower);
          }
          Play_Folder_Track(folder, track_hum); delay(100);    //RESTART HUM
          Loop_Current();
          humPlaying = true;
          swinging = false;
          doubleClickCheck = true;
          break;
        }
      }
      if (!doubleClickCheck) {
        buttonstate = LOW; lightstate = LOW;
        humPlaying = false, swinging = false;
        Play_Folder_Track(folder, track_off);
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
        //--------------------------------------------------------------------------------------*/
    }//BUTTON PRESSED
  }//LIGHT ON
}//MAIN LOOP
