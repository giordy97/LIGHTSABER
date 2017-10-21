#include "DfPlayer.h"
#include "Config.h"
//0x17 loop folder parameter 1-255

void Send_CMD(uint8_t CMD, uint8_t Par1, uint8_t Par2){
  //Calculating Checksum
  int16_t checksum = -(Version_Byte + Command_Length + CMD + Par1 + Par2 + Acknowledge);
  //Preparing command to send in an Array
  uint8_t Command[10] = {Start_Byte,Version_Byte,Command_Length,CMD,Acknowledge,Par1,Par2,highByte(checksum),lowByte(checksum),End_Byte};
  //Sending Array
  Serial.write(Command,10);
}

void Specify_Volume(uint8_t vol){
  Send_CMD(0x06,0x00,vol);
}

void Specify_Equalizer(uint8_t num){
  Send_CMD(0x07,0x00,num);
}

void Specify_Source(uint8_t num){
  Send_CMD(0x09,0x00,num);
}

void Play_Folder_Track(uint8_t folder,uint8_t track){
  Send_CMD(0x0F,folder,track);
}

void Loop_Current(){
  Send_CMD(0x19,0x00,0x00);
}

void Dfplayer_Reset(){
  Send_CMD(0x0c,0x00,0x00);
}

void init_DfPlayer(uint8_t volume){
//Init Serial Port
Serial.begin(9600);
while(!Serial){;}

//Reset Dfplayer
Dfplayer_Reset();
delay(2000);

//Dfplayer Config:
Specify_Equalizer(0x00);        delay(100);
Specify_Source(0x01);           delay(100);
Specify_Volume(volume);         delay(100);
}

void Play_Rand(uint8_t first,uint8_t last,uint8_t folder){
  Play_Folder_Track(folder,random(first,last + 1));
}
