/*
  SIMPLE DFPLAYER LIBRARY TO USE DFPLAYER IN A LIGHTSABER
  WRITTEN BY: GIORDANO COSTI
*/
#include <Arduino.h>
#ifndef DFPLAYER_H
#define DFPLAYER_H

//STANDARD FUNCTIONS:
extern void Send_CMD(uint8_t CMD, uint8_t Par1, uint8_t Par2);
extern void Specify_Volume(uint8_t vol);            // (0-30)
extern void Specify_Equalizer(uint8_t num);         // (0,1,2,3,4,5) (Normal/Pop/Rock/Jazz/Classic/Base)
extern void Specify_Source(uint8_t num);            // (0,1,2,3,4,5) (U/SD/AUX/SLEEP/FLASH)
extern void Play_Folder_Track(uint8_t folder,uint8_t track);
extern void Loop_Current();
extern void Dfplayer_Reset();

//CUSTOM FUNCTIONS:
extern void init_DfPlayer(uint8_t volume);
extern void Play_Rand(uint8_t first,uint8_t last,uint8_t folder);

#endif //DFPLAYER_H
