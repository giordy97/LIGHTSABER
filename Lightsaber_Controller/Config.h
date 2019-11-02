// ====================================================================================
// ===                            CONFIGURE YOUR SABER                              ===
// ====================================================================================

/*    IF YOUR LIGHTSABER HAS TWO BUTTON COMMENT THE FOLLOWING LINE      */
#define ONE_BUTTON

//=================== FIRST CONFIGURATION: ===================
//    ALL YOU ARE SETTING HERE WILL BE OVERWITTEN 
//       BY CONFIG MENU AFTER FIRST START UP:    

#define FF 1     //FONT ACTUALY  SELECTED
#define FN 5     //NUMBER OF DIFFERENT FONTS ON YOUR SD CARD
#define FV 10    //VOLUME: ONLY MULTIPLE OF 5 (0-30)
#define FL 100     //LIGHTPOWER  (0-254)
#define FK 0     //FLICKER EFFECT (1 -> YES || 0 -> NO)
//============================================================

//*************** PINS NUMBER: *********************
//MAIN LED PIN:
#define pin_light     10      //PIN LED/LED STRIPE

//CRISTAL PIN:
#define pin_cristal   11      //PIN CRISTAL LED

//BUTTON 1 PIN:
#define pin_button1   9       //MAIN BUTTON PIN

//BUTTON 2 PIN:
/* IGNORE IF YOUR LIGHTSABER HAS ONLY ONE BUTTON */
#define pin_button2   12      //PIN SECONDARY BUTTON
//***************************************************

//CRISTAL PERIOD
//TIME CRISTAL GOES LOW, HIGH, LOW.
#define CristalPeriod 1200    //TIME IN MILLISECONDS
#define flash_time     60     //TIME FLASH DURING CLASH
#define open_menu_time 400    //TIME WHILE BUTTON NEED TO BE PRESSED TO ENTER MENU
#define double_click_time 250 //TIME TO DETECT AN EFFETTIVE DOUBLE CLICK
#define lockup_time 200       //TIME TO START LOCKUP EFFECT

//******************** TRACK NUMBERS: ***************
#define track_on           1
#define track_hum          2
#define track_lockup       3 
#define track_off          4
#define track_bootup       5
#define track_swing_start  6
#define track_swing_end    15
#define track_clash_start  16
#define track_clash_end    25
#define track_voice_start  26
#define track_voice_end    30
#define track_bullet_start 31
#define track_bullet_end   35
//***************************************************

//**************** LENGHT AUDIO FILE ****************
/* LENGHT IS USED TO CALCULATE DELAYS FOR ARDUINO */
#define time_on            21690
#define time_off           1650
#define time_clash         19320
#define time_swing         613
#define time_swing_long    19270
//***************************************************

//****************** BYTE FOR DFPLAYER: *************
#define Version_Byte      0xFF
#define Command_Length    0x06
#define Acknowledge       0x00
#define Start_Byte        0x7E
#define End_Byte          0xEF
//***************************************************

//******************** CONFIG FOR MPU6050 ***********
#define MPU_addr         0x68
#define swing_force      820000000   //ROTATION FORCE TO DETECT SWING
#define clash_force      1120000000  //LINEAR FORCE TO DETECT CLASH
#define rotation_force   24000       //ROTATION FORCE TO SKIP IN MENU
#define interval_clash   10
