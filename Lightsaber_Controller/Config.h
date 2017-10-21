// ====================================================================================
// ===                            CONFIGURE YOUR SABER                              ===
// ====================================================================================

/*    IF YOUR LIGHTSABER HAS TWO BUTTON COMMENT THE FOLLOWING LINE      */
#define ONE_BUTTON

//*************** PINS NUMBER: *********************
/* IF YOU WANT FLICKER UNCOMMENT THE FOLLOWING LINE*/
//#define FLICKER
//**************************************************

//*************** PINS NUMBER: *********************
//LED PIN:
#define pin_light     10      //PIN LED STRIPE

//CRISTAL PIN:
#define pin_cristal   11      //PIN CRISTAL LED

//---------------- BUTTONS PIN: -------------------
//BUTTON 1:
#define pin_button1   9      //PIN ON/OFF BUTTON

//BUTTON 2:
/* IGNORE IF YOUR LIGHTSABER HAS ONLY ONE BUTTON */
#define pin_button2   12      //PIN SECONDARY BUTTON
//***************************************************

//---------------- CRISTAL PERIOD ------------------
#define CristalPeriod 2000    //CRISTAL PERIOD IN MILLISECONDS

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
#define time_on            2139         //2145
#define time_off           1400
#define time_clash         665          //673
#define time_swing         635          //650
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
#define swingforce       27000  //FORCE TO DETECT SWING
#define clashforce       29000  //FORCE TO DETECT CLASH
#define clashrebound     10000  //REBOUND TO DETECT CLASH
#define rotation_force   22000  //FORCE TO SKIP IN MENU

